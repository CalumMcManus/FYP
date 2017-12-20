#pragma once
//Assimp
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp/postprocess.h>
//Engine
#include <Engine\Graphics\Mesh.h>

#include <iostream>
#include <vector>
#include <glm.hpp>

using namespace std;
using namespace glm;
namespace Engine { namespace graphics {
	class Model
	{
	public:

		Model(string filepath);

		void render();

	private:

		std::vector<Mesh> m_ChildMeshs;
		string directory;

		void loadModel(string path);
		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	};

} }