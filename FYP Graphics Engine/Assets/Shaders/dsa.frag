#version 430

in vec3 vertPos;
in vec3 N;
in vec3 lightPos;
in vec2 uv;

uniform vec3 Ka;
uniform vec3 La;
uniform vec3 Kd;
uniform vec3 Ld;
uniform vec3 Ks;
uniform vec3 Ls;
            
uniform sampler2D texture2D;

layout( location = 0 ) out vec4 FragColour;

void main() {
	
   vec4 Ia = vec4(La,1.0) * vec4(Ka,1.0); 
   vec3 L = normalize(lightPos - vertPos);   
   float theta = dot(N,L);
   //theta = ceil(theta * 4)/4;
   vec4 Id = vec4(Ld,1.0) * max(theta, 0.0);
   Id = clamp(Id , 0.0, 1.0);
   Id =  vec4(Kd,1.0) * Id;
   vec3 Dist = lightPos - vertPos;
   float d = sqrt((Dist.x * Dist.x) + (Dist.y * Dist.y) + (Dist.z * Dist.z));
   d = d / 15;
   float Fatt = 1/(d * d);
   Fatt = clamp(Fatt , 0.0 , 1.0);
   float n = 2;
   vec4 R = vec4(normalize(reflect(L,normalize(N))),1.0); 
   vec4 V = vec4(normalize(vertPos),1.0); 
   vec4 Is = vec4(Ls,1.0) *  vec4(Ks,1.0) * pow(max(dot(R,V),0.0),n); 
   vec4 colour = Ia  +  (Fatt * Id)  + (Fatt  * Is); 
   
   FragColour = colour;
}
