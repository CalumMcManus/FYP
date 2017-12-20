#version 430
layout (location=0) in vec3 VertexPosition;

out vec3 texcoords;

uniform mat4 P, V;

void main()
{
	texcoords = VertexPosition;
	gl_Position = P * V * vec4(VertexPosition,1.0);
}