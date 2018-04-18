#version 410

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;
layout (location = 2) in vec2 vertex_uv;
layout (location = 3) in vec4 vertex_tangent;

out vec3 Position;
out vec3 Normal;
out vec2 TexCoords;

out vec3 toCameraVector;

uniform mat4 Model;
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

out mat3 TBN;
out vec3 tan;
void main () {
	tan = vertex_tangent.rgb;
	Normal = NormalMatrix * vertex_normal;
	Position = vec3( ModelViewMatrix * vec4(vertex_position, 1.0) );
	
	//Position = (Model * vec4(vertex_position, 1.0)).xyz;
	gl_Position = MVP * vec4(vertex_position, 1.0);
	TexCoords = vertex_uv;
	
	vec3 tangent = normalize(NormalMatrix*vertex_tangent.xyz); 
	vec3 norm = normalize(NormalMatrix * vertex_normal); 
	vec3 bitangent =  normalize( cross(norm, tangent)) * vertex_tangent.w;
	TBN = mat3(tangent, bitangent, norm);

}
