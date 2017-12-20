#include <Engine\Graphics\mesh.h>

using namespace std;
using namespace Engine::graphics;

Mesh::Mesh(vector<Vertex> vertices, vector<GLuint> indices)
{
	m_VAO = new VertexArray();
	this->vertices = vertices;
	this->indices = indices;

	setupMesh();
}

void Mesh::render()
{

	m_VAO->bind();
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);
	m_VAO->unbind();

	glActiveTexture(GL_TEXTURE0);
}

void Mesh::setupMesh()
{

	

	

	// create buffers/arrays
	//glGenVertexArrays(1, &VAO);
	//glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	m_VAO->bind();
	//glBindVertexArray(VAO);
	// load data into vertex buffers
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// set the memory address as the first vertex struct
	//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	if (vertices.size() > 0)
		m_VAO->addBuffer(new Buffer(&vertices[0], vertices.size(), 3), 0);

	m_VAO->bind();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	

	// set the vertex attribute pointers
	//// positions
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	// texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoords));

	glBindVertexArray(0);

	m_VAO->unbind();

}
