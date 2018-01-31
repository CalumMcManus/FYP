#version 410
in vec2 Texcoord;
out vec4 outColor;
uniform sampler2D texFramebuffer;
uniform float time;
void main()
{
   // outColor = texture(texFramebuffer, Texcoord) + vec4(1, 0,0, 1);
      outColor = texture( texFramebuffer, Texcoord+ 0.005*vec2( sin(time+1024.0*Texcoord.x),cos(time+768.0*Texcoord.y)));
}
