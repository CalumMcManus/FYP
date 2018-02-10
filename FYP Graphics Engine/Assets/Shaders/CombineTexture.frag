#version 410
in vec2 Texcoord;
out vec4 outColor;
uniform sampler2D texOne;
uniform sampler2D texTwo;

const float threashHold = 0.7f;

void main()
{
	vec4 colorOne = texture(texOne, Texcoord);
	vec4 colorTwo = texture(texTwo, Texcoord);
	vec4 color = colorOne + colorTwo;
	outColor = color;
	
}
