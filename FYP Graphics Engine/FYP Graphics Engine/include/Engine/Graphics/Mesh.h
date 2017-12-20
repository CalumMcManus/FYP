#pragma once
//Engine
#include <Engine\Graphics\vertexArray.h>
#include <Engine\Graphics\buffer.h>

#include <iostream>
#include <vector>

using namespace std;
namespace Engine { namespace graphics {

	

	class Mesh
	{
	
	public:
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		
		unsigned int VAO;

		Mesh(vector<Vertex> vertices, vector<GLuint> indices);

		// render the mesh with a given shader program
		void render();

	private:
		//unsigned int VBO, 
		GLuint EBO;

		VertexArray* m_VAO;

		void setupMesh();
		
	};
}
}