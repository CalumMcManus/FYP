#version 410

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;
layout (location = 2) in vec2 vertex_uv;

out vec3 Position;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 Model;
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

void main () {

	Normal = NormalMatrix * vertex_normal;
	Position = vec3( ModelViewMatrix * vec4(vertex_position, 1.0) );
	
	//Position = (Model * vec4(vertex_position, 1.0)).xyz;
	
	gl_Position = MVP * vec4(vertex_position, 1.0);
	TexCoords = vertex_uv;
}
