#version 410
in vec2 Texcoord;
out vec4 outColor;
uniform sampler2D texFramebuffer;

const float threashHold = 0.7f;

void main()
{
	vec4 color = texture(texFramebuffer, Texcoord);
//Luma greyscale converion
	float bright = (color.x * 0.2126) + (color.y * 0.7152) + (color.z * 0.0722);
	if(bright <= threashHold)
	{
		color = vec4(0, 0, 0, 1);
	}
	
	outColor = color;
	
}
