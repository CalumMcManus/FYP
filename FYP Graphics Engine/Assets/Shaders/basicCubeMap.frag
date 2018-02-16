#version 430
in vec3 texcoords;
uniform samplerCube cube_texture;
layout (location = 0) out vec3 PositionTex;
layout (location = 1) out vec3 NormalTex;
layout (location = 2) out vec3 ColorTex;
void main() {
	PositionTex = vec3(0.0, 0.0, 0.0);
	ColorTex = texture (cube_texture, texcoords).xyz;
}
