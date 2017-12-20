#pragma once
#include <GL\glew.h>
#include <glm.hpp>
namespace Engine { namespace graphics {
	
	//Data Types for buffers
	struct Vertex
	{
		Vertex() {}
		Vertex(glm::vec3 pos)
		{
			position = pos;
		}
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 textureCoords;
	};
	class Buffer
	{
	private:
		GLuint m_BufferID;
		GLuint m_ComponentCount;
	public:
		Buffer(Vertex* data, GLsizei count, GLuint componentCount);
		~Buffer();

		void bind() const;
		void unbind() const;

		inline GLuint getComponentCount() const { return m_ComponentCount; }
		inline GLuint& getBufferID() { return m_BufferID; }

	private:

	};
}
}