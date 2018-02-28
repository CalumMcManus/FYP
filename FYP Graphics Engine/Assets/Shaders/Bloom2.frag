#version 410
in vec2 Texcoord;
out vec4 outColor;
uniform sampler2D texFramebuffer;

uniform float sigmaValue = 3;
uniform int kernalSize = 25;

const vec2 resolution = vec2(1280, 720);
const float radius = 2.0;
const vec2 dir = vec2(0.0, 1.0);

float Guassian(int x, float sigma)
{
	float c = 2.0 * sigma * sigma;
	float e = -x*x/c;
	return exp(e) / sqrt(c * 3.14159265359);
}
void main()
{
  //this will be our RGBA sum
	vec4 sum = vec4(0.0);
	
	//our original texcoord for this fragment
	vec2 tc = Texcoord;
	
	//the amount to blur, i.e. how far off center to sample from 
	//1.0 -> blur by one pixel
	//2.0 -> blur by two pixels, etc.
	float blur = radius/resolution.x; 
    
	//the direction of our blur
	//(1.0, 0.0) -> x-axis blur
	//(0.0, 1.0) -> y-axis blur
	float hstep = dir.x;
	float vstep = dir.y;
	
	for (int i = 0; i < kernalSize; ++i) {
		sum += texture2D(texFramebuffer, vec2(tc.x + (i-((kernalSize/2)-1))*blur*hstep, tc.y + (i-((kernalSize/2)-1))*blur*vstep)) * Guassian(i - kernalSize / 2, sigmaValue);
	}
	
	//discard alpha for our simple demo, multiply by vertex color and return
	outColor =   vec4(sum.rgb, 1.0);//texture(texFramebuffer, Texcoord);//
	
}
