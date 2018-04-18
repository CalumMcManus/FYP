#version 410 core
in vec2 Texcoord;
out vec4 outColor;
uniform sampler2D texFramebuffer;
uniform sampler2D ssaoTex;

uniform sampler2DMS gPosition;
uniform sampler2DMS gNormal;
uniform sampler2DMS gAlbedo;
uniform sampler2DMS gDepth;

uniform sampler2D gPositionSS;
uniform sampler2D gNormalSS;
uniform sampler2D gAlbedoSS;
uniform sampler2D gDepthSS;

uniform samplerCube cubeTexture;

uniform mat4 View;
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

uniform bool CellShading;
uniform int Cells;

uniform float DirInten;
uniform vec3 DirDirectection;
uniform vec3 DirColour;

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
		}else{
			color = texture(gAlbedoSS, Texcoord);
			pos = texture(gPositionSS, Texcoord).rgb;
			norm = texture(gNormalSS, Texcoord).rgb;
			depth = texture(gDepthSS, Texcoord).rgb;
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
			
			//Directional Light
			float intencity = DirInten;
			vec3 dLightDir = mat3(View) * normalize(-DirDirectection);
			float diff = max(dot(norm, dLightDir), 0.0);
			if(CellShading){
				intencity = intencity/Cells;
				diff = floor(diff*Cells);
			}
			lighting += color.rgb * (DirColour * intencity * diff)/Samples;
			
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
						specularLightWeighting = pow(max(dot(reflectionDirection, viewDir), 0.0), 64);
						specularLightWeighting = clamp(specularLightWeighting, 0.0, 1.0);
						if(CellShading){
							specularLightWeighting = floor(specularLightWeighting*Cells);
						}
					}
					float Intencity = lights[i].Inten;
					if(lights[i].Angle >= 180)
					{
						attenuation = max(0.0, 1.0 - dot(lightRange, lightRange));
						attenuation = clamp(attenuation, 0.0, 1.0);
						if(CellShading){
							Intencity = Intencity/Cells;
							attenuation = ceil(attenuation*Cells);
						}
							
						vec3 specular = (vec3(color.a) * specularLightWeighting * lights[i].Color)*attenuation;
						lighting += (((diffuse*Intencity)+specular)*(attenuation))/(Samples);
					}
					else
					{
						vec3 normSpotDir = normalize(lights[i].Direction);
												
						float difAngle = degrees(acos(dot(normSpotDir, lightDir)));
						float feather = difAngle/(lights[i].Angle/2);
						attenuation = max(0.0, 1.0 - dot(lightRange, lightRange));
						attenuation =  attenuation * (1 - feather);
						if(CellShading){
							Intencity = Intencity/Cells;
							attenuation = ceil(attenuation*Cells);
						}
						if(difAngle > lights[i].Angle/2)
						{
							attenuation = 0;
						}
						vec3 specular = (vec3(color.a) * specularLightWeighting * lights[i].Color)*attenuation;
						lighting += (((diffuse*Intencity)+specular)*(attenuation))/(Samples);
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


void main()
{
	outColor = vec4(MSAA(), 1.0);
}