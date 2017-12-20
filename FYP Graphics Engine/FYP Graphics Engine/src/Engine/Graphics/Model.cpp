#include <Engine\Graphics\Model.h>

#include <iostream>
using namespace Engine::graphics;
Model::Model(string filepath)
{
	loadModel(filepath);
}


void Model::loadModel(string filepath)
{
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
		return;
	}
	directory = filepath.substr(0, filepath.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}




void Model::processNode(aiNode* node, const aiScene* scene)
{
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		int sceneMeshIndex = node->mMeshes[i];
		aiMesh* mesh = scene->mMeshes[sceneMeshIndex];
		m_ChildMeshs.push_back(processMesh(mesh, scene));
	}

	for (int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	vector<Vertex> vertices;
	vector<unsigned int> indices;

	vertices.resize(mesh->mNumVertices);
	indices.resize(mesh->mNumFaces * 3); 
								
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{

		vertices[i].position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertices[i].normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

		if (mesh->mTextureCoords[0])
		{
			vertices[i].textureCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		}
		else
		{
			vertices[i].textureCoords = glm::vec2(0.0f, 0.0f);
		}
	}

	for (int i = 0; i < mesh->mNumFaces; i++)
	{
		for (int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
			indices[3 * i + j] = mesh->mFaces[i].mIndices[j];
	}

	return Mesh(vertices, indices);
}

void Model::render()
{
	for(int i = 0; i < m_ChildMeshs.size(); i++)
	{
		m_ChildMeshs[i].render();
	}
}

