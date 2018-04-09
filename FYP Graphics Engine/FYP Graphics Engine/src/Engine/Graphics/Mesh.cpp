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

Engine::graphics::Mesh::~Mesh()
{
	delete m_VAO;
	m_VAO = nullptr;
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &EBO);
	
	vertices.clear();
	indices.clear();
}

void Mesh::render()
{

	m_VAO->bind();
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);
	m_VAO->unbind();

	//glActiveTexture(GL_TEXTURE0);
}

void Mesh::setupMesh()
{

	glGenBuffers(1, &EBO);

	m_VAO->bind();

	if (vertices.size() > 0)
		m_VAO->addBuffer(new Buffer(&vertices[0], vertices.size(), 3), 0);

	m_VAO->bind();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	// texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoords));
	// tangents
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

	glBindVertexArray(0);

	m_VAO->unbind();

}
