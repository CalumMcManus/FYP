#include <Engine\Graphics\vertexArray.h>
#include <iostream>
namespace Engine { namespace graphics {
	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &m_ArrayID);
	}
	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_ArrayID);
		for (int i = 0; i < m_Buffers.size(); i++)
		{
			std::cout << "buffer delete" << std::endl;
			delete m_Buffers[i];

		}
	}
	void VertexArray::addBuffer(Buffer * buffer, GLuint index)
	{
		m_Buffers.push_back(buffer);
		bind();
		buffer->bind();
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, buffer->getComponentCount(), GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		//buffer->unbind();
		//unbind();
	}
	void VertexArray::replaceBuffer(Buffer * buffer, GLuint index)
	{
		glDeleteBuffers(1, &m_Buffers[1]->getBufferID());
		delete m_Buffers[1];
		m_Buffers.erase(m_Buffers.begin() + 1);
		addBuffer(buffer, index);	
	}
	void VertexArray::bind() const
	{
		glBindVertexArray(m_ArrayID);
	}
	void VertexArray::unbind() const
	{
		glBindVertexArray(0);
	}
}
}