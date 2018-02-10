#version 410
in vec2 Texcoord;
out vec4 outColor;
uniform sampler2D texFramebuffer;

const float threashHold = 0.7f;

void main()
{
	vec4 color = texture(texFramebuffer, Texcoord);

	
	outColor = color;
	
}
