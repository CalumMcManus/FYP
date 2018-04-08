#pragma once
//Engine
#include <Engine\Graphics\vertexArray.h>
#include <Engine\Graphics\buffer.h>

#include <iostream>
#include <vector>

using namespace std;
namespace Engine { namespace graphics {

	
	//! Mesh 
	/*!
	Contains mesh data for drawing models
	Contained in the Engine::graphics namespace.
	*/
	class Mesh
	{
	
	public:
		//! Private vector of Vertacies.
		/*! Vector containing all vertex infomation of a mesh*/
		vector<Vertex> vertices;
		//! Private vector of unsigned integers.
		/*! Vector containing the indecies of the mesh vertacies*/
		vector<unsigned int> indices;
		//! Private unsigned integer.
		/*! Vertex Array Object */
		unsigned int VAO;
		//! Mesh Contructor
		/*!
		Calls fucntion for inital set up of a model
		\param vertices Vector of vertacies
		\param indices Vector of indicies
		*/
		Mesh(vector<Vertex> vertices, vector<GLuint> indices);

		//! The Render member function
		/*!
		Draws the mesh based on vertex data
		*/
		void render();

	private:
		//! Private GLuint variable
		/*! Element Buffer Object */
		GLuint EBO;
		//! Private VertexArray pointer
		/*! Element Buffer Object */
		VertexArray* m_VAO;
		//! The setupMesh member function
		/*!
		Sets up Opengl Buffers
		*/
		void setupMesh();
		
	};
}
}