#version 410 core
in vec2 Texcoord;
out vec4 outColor;
uniform sampler2D texFramebuffer;
uniform sampler2D ssaoTex;

uniform sampler2DMS gPosition;
uniform sampler2DMS gNormal;
uniform sampler2DMS gAlbedo;
uniform sampler2DMS gAlbedoUnlit;
uniform sampler2DMS gDepth;

uniform sampler2D gPositionSS;
uniform sampler2D gNormalSS;
uniform sampler2D gAlbedoSS;
uniform sampler2D gAlbedoUnlitSS;
uniform sampler2D gDepthSS;

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
uniform int Samples;

uniform vec3 AmbientColor;
uniform float AmbientInten;

vec3 MSAA()
{
	ivec2 texSize = textureSize(gAlbedo);
	ivec2 texCoord = ivec2(Texcoord * texSize);
	float ssaoValue = texture(ssaoTex, Texcoord).r;
	
	vec3 lighting;
	
	for (int i = 0; i < Samples; ++i)
	{
		vec3 color = texelFetch(gAlbedo, texCoord, i).rgb;
		vec3 norm = texelFetch(gNormal, texCoord, i).rgb;
		vec3 depth = texelFetch(gDepth, texCoord, i).rgb;
		vec3 pos = texelFetch(gPosition, texCoord, i).rgb;
		vec3 colorUnlit = texelFetch(gAlbedoUnlit, texCoord, i).rgb;
		if(depth.r >= 1)
		{
			lighting += colorUnlit/(Samples);
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
			vec3 posWorld = pos.rgb;

			vec3 viewDir = normalize(viewPos - posWorld);
			
			for(int i = 0; i < MAX_LIGHTS; ++i)
			{
			
				vec4 lightPos = View * vec4(lights[i].Pos,1.0);
				vec3 normLight = normalize(lightPos.xyz);
				float distance = length(lightPos.xyz - posWorld);
				if(distance < lights[i].Radius)
				{
					vec3 lightDir = normalize(lightPos.xyz - posWorld);
					vec3 diffuse = max(dot(norm.rgb, lightDir), 0.0) * color.rgb * lights[i].Color;
					vec3 lightRange = (lightPos.xyz - posWorld)/lights[i].Radius;
					float attenuation = max(0.0, 1.0 - dot(lightRange, lightRange));
					
					
					vec3 normSpotDir = normalize(lights[i].Direction);
					
					float difAngle = degrees(acos(dot(normSpotDir, lightDir)));
					
					
					if(difAngle > lights[i].Angle)
					{
						diffuse = vec3(0, 0, 0); 
					}
					
					
					lighting += ((diffuse*lights[i].Inten)*attenuation)/(Samples+1.5);
				}
			}
		}
	}

	return lighting;
}

vec3 SS()
{
	vec3 lighting;
	
	vec3 color = texture(gAlbedoSS, Texcoord).rgb;
	vec3 colorUnlit = texture(gAlbedoUnlitSS, Texcoord).rgb;
	vec3 pos = texture(gPositionSS, Texcoord).rgb;
	vec3 norm = normalize(texture( gNormalSS, Texcoord)).rgb;
	vec3 depth = texture(gDepthSS, Texcoord).rgb;
	float ssaoValue = texture(ssaoTex, Texcoord).r;
	
	if(depth.r >= 1)
	{
		lighting = colorUnlit;
	}else
	{
	
		if(SSAO)
		{
			lighting = vec3(AmbientInten * (color.rgb*AmbientColor) * ssaoValue); // hard-coded ambient component
		}
		else
		{
			lighting = vec3(AmbientInten * (color.rgb*AmbientColor)); // hard-coded ambient component
		}
		vec3 posWorld = pos.rgb;

		vec3 viewDir = normalize(viewPos - posWorld);
		
		for(int i = 0; i < MAX_LIGHTS; ++i)
		{
		
			vec4 lightPos = View * vec4(lights[i].Pos,1.0);
			vec3 normLight = normalize(lightPos.xyz);
			float distance = length(lightPos.xyz - posWorld);
			if(distance < lights[i].Radius)
			{
				// diffuse
				vec3 lightDir = normalize(lightPos.xyz - posWorld);
				vec3 diffuse = max(dot(norm.rgb, lightDir), 0.0) * color.rgb * lights[i].Color;
				vec3 lightRange = (lightPos.xyz - posWorld)/lights[i].Radius;
				float attenuation = max(0.0, 1.0 - dot(lightRange, lightRange));
				lighting += ((diffuse*lights[i].Inten)*attenuation);
			}
		}
	
	}
	return lighting;
}
void main()
{
	vec3 color;
	if(Samples > 1)
	{
		color = MSAA();
	}
	else
	{
		color = SS();
	}

	outColor = vec4(color, 1.0);	
}