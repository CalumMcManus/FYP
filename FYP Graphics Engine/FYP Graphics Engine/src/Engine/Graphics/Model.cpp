#include <Engine\Graphics\Model.h>

#include <iostream>
using namespace Engine::graphics;
Model::Model(string filepath)
{
	loadModel(filepath);
}

Engine::graphics::Model::~Model()
{
	for (auto mesh : m_ChildMeshs)
		delete mesh;
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

Mesh* Model::processMesh(aiMesh* mesh, const aiScene* scene)
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

	vector<vec3> tan1Accum;
	vector<vec3> tan2Accum;

	for (uint i = 0; i < vertices.size(); i++) {
		tan1Accum.push_back(vec3(0.0f));
		tan2Accum.push_back(vec3(0.0f));
	}

	// Compute the tangent vector
	for (uint i = 0; i < indices.size(); i += 3)
	{
		const vec3 &p1 = vertices[indices[i]].position;
		const vec3 &p2 = vertices[indices[i + 1]].position;
		const vec3 &p3 = vertices[indices[i + 2]].position;

		const vec2 &tc1 = vertices[indices[i]].textureCoords;
		const vec2 &tc2 = vertices[indices[i + 1]].textureCoords;
		const vec2 &tc3 = vertices[indices[i + 2]].textureCoords;

		vec3 q1 = p2 - p1;
		vec3 q2 = p3 - p1;
		float s1 = tc2.x - tc1.x, s2 = tc3.x - tc1.x;
		float t1 = tc2.y - tc1.y, t2 = tc3.y - tc1.y;
		float r = 1.0f / (s1 * t2 - s2 * t1);
		vec3 tan1((t2*q1.x - t1*q2.x) * r,
			(t2*q1.y - t1*q2.y) * r,
			(t2*q1.z - t1*q2.z) * r);
		vec3 tan2((s1*q2.x - s2*q1.x) * r,
			(s1*q2.y - s2*q1.y) * r,
			(s1*q2.z - s2*q1.z) * r);
		tan1Accum[indices[i]] += tan1;
		tan1Accum[indices[i + 1]] += tan1;
		tan1Accum[indices[i + 2]] += tan1;
		tan2Accum[indices[i]] += tan2;
		tan2Accum[indices[i + 1]] += tan2;
		tan2Accum[indices[i + 2]] += tan2;
	}
	for (uint i = 0; i < vertices.size(); ++i)
	{
		const vec3 &n = vertices[i].normal;
		vec3 &t1 = tan1Accum[i];
		vec3 &t2 = tan2Accum[i];

		// Gram-Schmidt orthogonalize
		vertices[i].tangent = vec4(glm::normalize(t1 - (glm::dot(n, t1) * n)), 0.0f);
		// Store handedness in w
		vertices[i].tangent.w = (glm::dot(glm::cross(n, t1), t2) < 0.0f) ? -1.0f : 1.0f;
	}
	return new Mesh(vertices, indices);
}

void Model::render()
{
	for(int i = 0; i < m_ChildMeshs.size(); i++)
	{
		m_ChildMeshs[i]->render();
	}
}

