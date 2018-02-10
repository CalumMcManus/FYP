#version 410

out vec4 fragment_colour; // final colour of surface

in vec4 position;
in vec2 uv;

uniform vec3 light_pos;
uniform vec3 light_ambient;
uniform sampler2D texture2D;

in vec3 cameraPos;
in vec3 rayDir;

void main () {
	vec4 test = vec4(0, 0, 0, 0);

	vec3 vDir = normalize(position.xyz - cameraPos);
	float cosAngle = dot(vDir, rayDir);
	float angle = degrees(acos(cosAngle));
	
	if(angle < 5)
	{
		test = vec4(1, 0, 0, 1);
	}
	

	float intensity = (1.0 / length(position.xyz - light_pos))+0.25;
	intensity = clamp(intensity, 0, 1);
	vec4 ambient = vec4(light_ambient, 1);
	fragment_colour = ((vec4(texture(texture2D, uv).rgb, 1.0)) * (ambient*1.1))+test;
}
