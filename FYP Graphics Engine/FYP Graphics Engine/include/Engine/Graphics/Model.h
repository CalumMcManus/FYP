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
	//! Model 
	/*!
	Contains all the meshes that make up a single model and renders them
	Contained in the Engine::graphics namespace.
	*/
	class Model
	{
	public:
		//! Model Contructor
		/*!
		Calls fucntion for inital set up of a model
		\param filepath Path to model file
		*/
		Model(string filepath);
		//! Model Decontructor
		/*!
		Cleans up memory in class
		*/
		~Model();
		//! The Render member function
		/*!
		Loops though each mesh and draws it.
		*/
		void render();

	private:
		//! Private vector of Mesh's.
		/*! Vector containing all the meshes that make up the model*/
		std::vector<Mesh*> m_ChildMeshs;
		//! Private string variable.
		/*! Model directory*/
		string directory;
		//! The loadModel member function
		/*!
		Loads a model from a given path
		\param path File path to model
		*/
		void loadModel(string path);

		//Process model data functions
		void processNode(aiNode* node, const aiScene* scene);
		Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
	};

} }