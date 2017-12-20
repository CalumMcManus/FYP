#version 410

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_norm;
layout (location = 2) in vec2 VertexUV;

uniform mat4 P;
uniform mat4 V = mat4(1.0);
uniform mat4 M = mat4(1.0);
out vec2 uv;
out vec4 position;

void main () {

	gl_Position = P * V * M * vec4(vertex_position, 1);
	position = M * vec4(vertex_position, 1.0);
	uv = VertexUV;
}
