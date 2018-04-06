#version 410 core
in vec2 Texcoord;
out vec4 outColor;
uniform sampler2D texFramebuffer;
uniform sampler2D ssaoTex;

uniform sampler2DMS gPosition;
uniform sampler2DMS gNormal;
uniform sampler2DMS gAlbedo;
uniform sampler2DMS gDepth;
uniform sampler2DMS gComponent;

uniform sampler2D gPositionSS;
uniform sampler2D gNormalSS;
uniform sampler2D gAlbedoSS;
uniform sampler2D gDepthSS;
uniform sampler2D gComponentSS;

uniform samplerCube cubeTexture;

uniform mat4 View;
uniform mat4 Proj;
uniform mat4 invView;
uniform mat4 invProj;
struct Light {
	vec3 Pos;
	vec3 Color;
	float Inten;
	float Radius;
	float Angle;
	vec3 Direction;
};

const int MAX_LIGHTS = 32;
uniform Light lights[MAX_LIGHTS];
uniform vec3 viewPos;

uniform bool SSAO;
uniform bool Outline;
uniform int Samples;

uniform vec3 AmbientColor;
uniform float AmbientInten;

vec3 MSAA()
{
	ivec2 texSize = textureSize(gAlbedo);
	ivec2 texCoord = ivec2(Texcoord * texSize);
	float ssaoValue = texture(ssaoTex, Texcoord).r;
	
	vec3 lighting;
	
	float ol;
	vec4 color;// = texture(gAlbedoSS, Texcoord);
	vec3 norm;
	vec3 depth;
	vec3 pos;
	vec3 comp;
	for (int i = 0; i < Samples; ++i)
	{
		if(Outline)
		{
			float dx = 1.0 / texSize.x;
			float dy = 1.0 / texSize.y;	
			vec3 center;
			vec3 top;
			vec3 topRight;
			vec3 right;
			if(Samples > 1)
			{
				center   = texelFetch(gNormal, texCoord + ivec2(0, 0), i).rgb;
				top      = texelFetch(gNormal, texCoord + ivec2(0, 1), i).rgb;
				topRight = texelFetch(gNormal, texCoord + ivec2(1, 1), i).rgb;
				right    = texelFetch(gNormal, texCoord + ivec2(1, 0), i).rgb;	
			}
			else{
				center = texture(gNormalSS, Texcoord + vec2(0.0, 0.0)).rgb;
				top = texture(gNormalSS, Texcoord + vec2(0.0, dy)).rgb;
			    topRight = texture(gNormalSS, Texcoord + vec2(dx, dy)).rgb;
			    right = texture(gNormalSS, Texcoord + vec2(dx, 0.0)).rgb;
			}
			vec3 t = center - top;
			vec3 r = center - right;
			vec3 tr = center - topRight;
			
			t = abs(t);
			r = abs(r);
			tr = abs(tr);
			
			float n;
			n = max(n, t.x);
			n = max(n, t.y);
			n = max(n, t.z);
			n = max(n, r.x);
			n = max(n, r.y);
			n = max(n, r.z);
			n = max(n, tr.x);
			n = max(n, tr.y);
			n = max(n, tr.z);
			
			ol += (1.0 - clamp(clamp((n*2.0) - 0.8, 0.0, 1.0)*1.5, 0.0, 1.0))/(Samples);
		}
		
		
		if(Samples > 1)
		{		
			color = texelFetch(gAlbedo, texCoord, i);
			norm  =  texelFetch(gNormal, texCoord, i).rgb;
			depth = texelFetch(gDepth, texCoord, i).rgb;
			pos   =   texelFetch(gPosition, texCoord, i).rgb;
			comp  =  texelFetch(gComponent, texCoord, i).rgb;
		}else{
			color = texture(gAlbedoSS, Texcoord);
			pos = texture(gPositionSS, Texcoord).rgb;
			norm = texture(gNormalSS, Texcoord).rgb;
			depth = texture(gDepthSS, Texcoord).rgb;
			comp = texture(gComponentSS, Texcoord).rgb;
		}
		
		float shininess = color.a*255;
		float specularLightWeighting = 0.0;
		if(depth.r >= 1)
		{
			lighting += color.rgb/(Samples);
		}else
		{
		
			if(SSAO)
			{
				lighting += vec3(AmbientInten * (color.rgb*AmbientColor) * ssaoValue)/(Samples);
			}
			else
			{
				lighting += vec3(AmbientInten * (color.rgb*AmbientColor))/(Samples);
			}
			vec3 posWorld = pos;

			vec3 viewDir = normalize(viewPos - posWorld);
			
			for(int i = 0; i < MAX_LIGHTS; ++i)
			{
				vec4 lightPos = View * vec4(lights[i].Pos,1.0);
				vec3 normLight = normalize(lightPos.xyz);
				float distance = length(lightPos.xyz - posWorld);
				if(distance < lights[i].Radius)
				{
					vec3 lightDir = normalize(lightPos.xyz - posWorld);
					vec3 diffuse = max(dot(norm, lightDir), 0.0) * color.rgb * lights[i].Color;
					vec3 lightRange = (lightPos.xyz - posWorld)/lights[i].Radius;;
					float attenuation;
					if (shininess > 0.0) 
					{
						vec3 reflectionDirection = reflect(-lightDir, norm);
						specularLightWeighting = pow(max(dot(viewDir, reflectionDirection), 0.0), 64);
					}
					if(lights[i].Angle >= 180)
					{
						attenuation = max(0.0, 1.0 - dot(lightRange, lightRange));
						vec3 specular = (vec3(color.a) * specularLightWeighting * vec3(1))*attenuation;
						lighting += (((diffuse*lights[i].Inten)*attenuation)+specular)/(Samples);
					}
					else
					{
						vec3 normSpotDir = normalize(lights[i].Direction);
												
						float difAngle = degrees(acos(dot(normSpotDir, lightDir)));
						float feather = difAngle/(lights[i].Angle/2);
						attenuation = max(0.0, 1.0 - dot(lightRange, lightRange));
						attenuation =  attenuation * (1 - feather);
						if(difAngle > lights[i].Angle/2)
						{
							attenuation = 0;
						}
						vec3 specular = (vec3(color.a) * specularLightWeighting * vec3(1))*attenuation;
						lighting += (((diffuse*lights[i].Inten)*attenuation)+specular)/(Samples);
					}
				}
			}
		}
	}
	if(Outline)
	{
		return lighting * (ol);
	}
	else{
		return lighting;
	}
}


vec3 PositionFromDepth(float depth);
vec3 BinarySearch(inout vec3 dir, inout vec3 hitCoord, inout float dDepth);
vec4 RayCast(vec3 dir, inout vec3 hitCoord, out float dDepth);
vec3 fresnelSchlick(float cosTheta, vec3 F0);
vec3 hash(vec3 a);
vec4 RayMarch(vec3 dir, inout vec3 hitCoord, out float dDepth);
vec4 Reflection();
float Metallic;
void main()
{
	vec3 color;
	color = MSAA();

	color = mix(color, Reflection().rgb, Metallic);
	outColor = vec4(color, 0.0);
}

#define Scale vec3(0.8, 0.8, 0.8)
#define K 19.19
const float step = 0.1;
const float minRayStep = 0.1;
const float maxSteps = 60;
const int numBinarySearchSteps = 10;
const float reflectionSpecularFalloffExponent = 3.0;

vec4 Reflection()
{
	Metallic = texture2D(gComponentSS, Texcoord).r;
	float Metalness = texture(gComponentSS, Texcoord).z;
	vec3 albedo = texture(gAlbedoSS, Texcoord).rgb;
	Metallic = Metallic * Metalness;
	//if(Metallic < 0.01)
	//	return vec4(1, 1, 1, 1);
		
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
	
    vec3 SSR = textureLod(gAlbedoSS, coords.xy, 0).rgb * clamp(ReflectionMultiplier, 0.0, 0.9) * Fresnel;  
	float av = (SSR.x + SSR.y + SSR.z)/3;
	vec3 colour = mix(texture(cubeTexture, rc).rgb, SSR , clamp(ReflectionMultiplier, 0.0, 0.9));

	//if(SSR.r < 0.01 && SSR.g < 0.01 && SSR.b < 0.01)
	//	return texture(cubeTexture, rc);
	
	return vec4(colour, Metallic);
}
vec3 PositionFromDepth(float depth) {
    float z = depth * 2.0 - 1.0;
    vec4 clipSpacePosition = vec4(Texcoord * 2.0 - 1.0, z, 1.0);
    vec4 viewSpacePosition = invProj * clipSpacePosition;

    viewSpacePosition /= viewSpacePosition.w;

    return viewSpacePosition.xyz;
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