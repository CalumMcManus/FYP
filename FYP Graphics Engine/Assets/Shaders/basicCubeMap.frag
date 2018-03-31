#version 430
in vec3 texcoords;
uniform samplerCube cube_texture;
layout (location = 0) out vec3 PositionTex;
layout (location = 1) out vec3 NormalTex;
layout (location = 2) out vec4 ColorTex;
layout (location = 3) out vec3 CompTex;

void main() {
	//PositionTex = texture (cube_texture, texcoords).xyz;
	//NormalTex = texture (cube_texture, texcoords).xyz;
	ColorTex = vec4(texture (cube_texture, texcoords).xyz, 0);
	CompTex = vec3(0, 0, 0);
}
