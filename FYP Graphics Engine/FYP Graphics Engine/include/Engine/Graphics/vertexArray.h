#pragma once

#include <vector>
#include<GL\glew.h>

#include <Engine\Graphics\buffer.h>

namespace Engine { namespace graphics {

	class VertexArray
	{
	private:
		GLuint m_ArrayID;
		std::vector<Buffer*> m_Buffers;
	public:
		VertexArray();
		~VertexArray();

		void addBuffer(Buffer* buffer, GLuint index);
		void replaceBuffer(Buffer * buffer, GLuint index);
		void bind() const;
		void unbind() const;
	};
}
}