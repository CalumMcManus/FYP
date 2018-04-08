#pragma once

#include <vector>
#include<glad/glad.h>

#include <Engine\Graphics\buffer.h>

namespace Engine { namespace graphics {
	//! VertexArray 
	/*!
	Class used to store and pass vertex infomation to a shader
	Contained in the Engine::graphics namespace.
	*/
	class VertexArray
	{
	private:
		//! Private GLuint variable.
		/*! Vertex Array ID */
		GLuint m_ArrayID;
		//! Private vector of Buffer Pointers.
		/*! Contains all the buffers attached to the vertex array object */
		std::vector<Buffer*> m_Buffers;
	public:
		//! VertexArray Contructor
		/*!
		Sets up the Vertex Array Object
		*/
		VertexArray();
		//! Scene Destructor.
		/*!
		Cleans up memory in the class
		*/
		~VertexArray();
		//! The addBuffer member function
		/*!
		Function for adding a buffer to the vertex array object
		\param buffer Buffer pointer to add to vertex array
		\param index GLuint Index to add buffer to
		*/
		void addBuffer(Buffer* buffer, GLuint index);
		//! The replaceBuffer member function
		/*!
		Function for replacing an exsisting buffer with a new one
		\param buffer Buffer pointer to add to vertex array
		\param index GLuint Index to replace buffer at
		*/
		void replaceBuffer(Buffer * buffer, GLuint index);
		//! The bind member function
		/*!
		Binds vertex array
		*/
		void bind() const;
		//! The unbind member function
		/*!
		Unbinds vertex array
		*/
		void unbind() const;
	};
}
}