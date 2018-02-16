#version 410
in vec2 Texcoord;
out vec4 outColor;
uniform sampler2D texFramebuffer;
uniform sampler2D ssaoTex;
uniform sampler2D skyTex;
const float threashHold = 0.7f;

void main()
{
	vec4 color = texture(texFramebuffer, Texcoord)*texture(ssaoTex, Texcoord).r;

	
	outColor = color;
	
}
