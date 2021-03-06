#version 410
in vec2 Texcoord;
out vec4 outColor;
uniform sampler2D texFramebuffer;

//RADIUS of our vignette, where 0.5 results in a circle fitting the screen
uniform float RADIUS = 0.75;

//softness of our vignette, between 0.0 and 1.0
uniform float SOFTNESS = 0.45;

uniform vec2 resolution = vec2(1280, 720);
void main()
{
   // outColor = texture(texFramebuffer, Texcoord);
    //  outColor = texture( texFramebuffer, Texcoord+ 0.005*vec2( sin(time+resolution.x*Texcoord.x),cos(time+resolution.y*Texcoord.y)));
	
	//sample our texture
	vec4 texColor = texture2D(texFramebuffer, Texcoord);
	//determine center position
	vec2 position = (gl_FragCoord.xy / resolution.xy) - vec2(0.5);
	//determine the vector length of the center position
	float len = length(position);
	
	//use smoothstep to create a smooth vignette
	float vignette = smoothstep(RADIUS, RADIUS-SOFTNESS, len);
	
	texColor.rgb *= vignette;
	//final colour, multiplied by vertex colour
	outColor = texColor;//* vColor;

}

