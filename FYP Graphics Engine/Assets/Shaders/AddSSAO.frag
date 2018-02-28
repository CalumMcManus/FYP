#version 410 core
in vec2 Texcoord;
out vec4 outColor;
uniform sampler2D texFramebuffer;
uniform sampler2D ssaoTex;


void main()
{
	vec4 color = texture(texFramebuffer, Texcoord)*texture(ssaoTex, Texcoord).r;

	outColor = color;
	
}
