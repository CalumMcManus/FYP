#version 410
in vec2 Texcoord;
out vec4 outColor;
uniform sampler2D texFramebuffer;
uniform float time;
//void main()
//{
   // outColor = texture(texFramebuffer, Texcoord);
    //  outColor = texture( texFramebuffer, Texcoord+ 0.005*vec2( sin(time+1280.0*Texcoord.x),cos(time+720.0*Texcoord.y)));

//}

vec2 SineWave( vec2 p ){
    float pi = 3.14159;
    float A = 0.1;
    float w = 10.0 * pi;
    float t = time*pi/180.0;
    float y = sin( w*p.x + t) * A; 
    return vec2(p.x, p.y+y); 
}
void main(){
    vec2 p = Texcoord;
    vec2 uv = SineWave( p ); 
    vec4 tcolor = texture2D(texFramebuffer, uv); 
    outColor = tcolor; 
}