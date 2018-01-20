#version 410

layout (location = 0) in vec3 vertex_position;
layout (location = 2) in vec2 VertexUV;

uniform mat4 P;
uniform mat4 V = mat4(1.0);
uniform mat4 M = mat4(1.0);
out vec2 uv;
out vec4 position;

uniform vec3 cam_pos;
out vec3 cameraPos;
uniform vec3 ray_dir;
out vec3 rayDir;

void main () {

	
	cameraPos = vec3(V * M * vec4(cam_pos,1.0)); 
	rayDir = vec3(M * vec4(cameraPos + ray_dir,1.0));
	gl_Position = P * V * M * vec4(vertex_position, 1);
	position = V * M * vec4(vertex_position, 1.0);
	uv = VertexUV;
}
