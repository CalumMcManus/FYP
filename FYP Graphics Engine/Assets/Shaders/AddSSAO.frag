#version 410 core
in vec2 Texcoord;
out vec4 outColor;
uniform sampler2D texFramebuffer;
uniform sampler2D ssaoTex;


void main()
{
	//ivec2 texCoord = ivec2(floor(textureSize(texFramebuffer)* Texcoord));
	vec4 color = texture(texFramebuffer, Texcoord)*texture(ssaoTex, Texcoord).r;
	//vec4 color = textureMultisample(texFramebuffer, texCoord);
	//vec4 color = texelFetch(texFramebuffer, texCoord, gl_SampleID);
	outColor = color;
	
}
