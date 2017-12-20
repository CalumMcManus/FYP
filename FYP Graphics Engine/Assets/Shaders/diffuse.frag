#version 410

out vec4 fragment_colour; // final colour of surface

in vec4 position;
in vec2 uv;

uniform vec3 light_pos;
uniform sampler2D texture2D;

void main () {
	
	float intensity = (1.0 / length(position.xyz - light_pos))+0.25;
	intensity = clamp(intensity, 0, 1);
	fragment_colour = vec4(texture(texture2D, uv).rgb, 1.0) * intensity;
}
