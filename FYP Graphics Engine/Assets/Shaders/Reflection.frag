#version 410 core
in vec2 Texcoord;
out vec4 outColor;
uniform sampler2D LightPass;
uniform sampler2D gAlbedoSS;
uniform sampler2D gPositionSS;
uniform sampler2D gNormalSS;
uniform sampler2D gComponentSS;

uniform samplerCube cubeTexture;

uniform mat4 Proj;
uniform mat4 invView;
uniform mat4 invProj;

uniform vec3 viewPos;

vec3 PositionFromDepth(float depth);
vec3 BinarySearch(inout vec3 dir, inout vec3 hitCoord, inout float dDepth);
vec4 RayCast(vec3 dir, inout vec3 hitCoord, out float dDepth);
vec3 fresnelSchlick(float cosTheta, vec3 F0);
vec3 hash(vec3 a);
vec4 RayMarch(vec3 dir, inout vec3 hitCoord, out float dDepth);
vec4 Reflection();
float Metallic;
float Metalness;
float Roughness;

void main()
{
	vec3 color = texture(LightPass, Texcoord).rgb;
	vec3 component = texture2D(gComponentSS, Texcoord).rgb;
	
	if(component.r < 0.01)
	{
		outColor = vec4(color, 1.0);
		return;
	}
	Metallic = component.r;
	Metalness = component.z;
	Roughness = component.y;
	vec3 reflection = Reflection().rgb;
	color = mix(color, reflection, Metallic);
	outColor = vec4(color, 1.0);
}

#define Scale vec3(0.8, 0.8, 0.8)
#define K 19.19
const float step = 0.05;
const float minRayStep = 0.01;
const float maxSteps = 50;
const int numBinarySearchSteps = 30;
const float reflectionSpecularFalloffExponent = 4.0;

vec4 Reflection()
{
	float level = clamp(log2(Roughness * 1024), 0, 10);
	vec3 albedo = texture(LightPass, Texcoord).rgb;
	Metallic = Metallic * Metalness;
	if(Metallic < 0.01)
		return vec4(0, 0, 0, 0);
		
	vec3 viewNormal = texture(gNormalSS, Texcoord).rgb;
    vec3 viewPos = texture(gPositionSS, Texcoord).xyz;
    
	float spec = texture(gAlbedoSS, Texcoord).w;
	
	vec3 F0 = vec3(0.04); 
    F0      = mix(F0, albedo, Metallic);
    vec3 Fresnel = fresnelSchlick(max(dot(normalize(viewNormal), normalize(viewPos)), 0.0), F0);
	
    vec3 reflected = normalize(reflect(normalize(viewPos), normalize(viewNormal)));
	
	vec3 hitPos = viewPos;
    float dDepth;
 
    vec3 wp = vec3(vec4(viewPos, 1.0));
    vec3 jitt = mix(vec3(0.0), vec3(hash(wp)), spec);
    vec4 coords = RayMarch((vec3(jitt) + reflected * max(minRayStep, -viewPos.z)), hitPos, dDepth);
	
	vec2 dCoords = smoothstep(0.2, 0.6, abs(vec2(0.5, 0.5) - coords.xy));

    float screenEdgefactor = clamp(1.0 - (dCoords.x + dCoords.y), 0.0, 1.0);

    float ReflectionMultiplier = pow(Metallic, reflectionSpecularFalloffExponent) * 
                screenEdgefactor * 
                -reflected.z;
			
	vec3 eye = normalize(viewPos);
	vec3 norm = normalize(texture2D(gNormalSS, Texcoord).rgb);
	
	vec3 rc = reflect(eye, norm);
	rc = vec3(invView * vec4(rc, 0.0));
	
    vec3 SSR = textureLod(LightPass, coords.xy, 0).rgb * clamp(ReflectionMultiplier, 0.0, 0.9) * Fresnel;  
	float av = (SSR.x + SSR.y + SSR.z)/3;
	vec3 colour = mix(textureLod(cubeTexture, rc, level).rgb, SSR , clamp(ReflectionMultiplier, 0.0, 0.9));

	//if(SSR.r < 0.01 && SSR.g < 0.01 && SSR.b < 0.01)
	//	return textureLOD(cubeTexture, rc, 3);
	
	return vec4(textureLod(cubeTexture, rc, level).rgb, Metallic);
}
vec3 PositionFromDepth(float depth) {
    float z = depth * 2.0 - 1.0;
    vec4 clipPosition = vec4(Texcoord * 2.0 - 1.0, z, 1.0);
    vec4 viewPosition = invProj * clipPosition;

    viewPosition /= viewPosition.w;

    return viewPosition.xyz;
}
vec3 BinarySearch(inout vec3 dir, inout vec3 hitCoord, inout float dDepth)
{
    float depth;
    vec4 projectedCoord;
 
    for(int i = 0; i < numBinarySearchSteps; i++)
    {
        projectedCoord = Proj * vec4(hitCoord, 1.0);
        projectedCoord.xy /= projectedCoord.w;
        projectedCoord.xy = projectedCoord.xy * 0.5 + 0.5;
 
        depth = textureLod(gPositionSS, projectedCoord.xy, 2).z;
        dDepth = hitCoord.z - depth;

        dir *= 0.5;
        if(dDepth > 0.0)
            hitCoord += dir;
        else
            hitCoord -= dir;    
    }
        projectedCoord = Proj * vec4(hitCoord, 1.0);
        projectedCoord.xy /= projectedCoord.w;
        projectedCoord.xy = projectedCoord.xy * 0.5 + 0.5;
 
    return vec3(projectedCoord.xy, depth);
}
vec4 RayMarch(vec3 dir, inout vec3 hitCoord, out float dDepth)
{
    dir *= step;

    float depth;
    int steps;
    vec4 projectedCoord;

    for(int i = 0; i < maxSteps; i++)
    {
        hitCoord += dir;
 
        projectedCoord = Proj * vec4(hitCoord, 1.0);
        projectedCoord.xy /= projectedCoord.w;
        projectedCoord.xy = projectedCoord.xy * 0.5 + 0.5;
 
        depth = textureLod(gPositionSS, projectedCoord.xy, 2).z;
        if(depth > 1000.0)
            continue;
 
        dDepth = hitCoord.z - depth;

        if((dir.z - dDepth) < 1.2)
        {
            if(dDepth <= 0.0)
            {   
                vec4 Result;
                Result = vec4(BinarySearch(dir, hitCoord, dDepth), 1.0);

                return Result;
            }
        }
        steps++;
    }
    return vec4(projectedCoord.xy, depth, 0.0);
}
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}
vec3 hash(vec3 a)
{
    a = fract(a * Scale);
    a += dot(a, a.yxz + K);
    return fract((a.xxy + a.yxx)*a.zyx);
}