#pragma once
#include <glad/glad.h>
#include <glm.hpp>
namespace Engine { namespace graphics {
	
	//! Vertex 
	/*!
	Struct containing all the data about each vertex
	Contained in the Engine::graphics namespace.
	*/
	struct Vertex
	{
		//! Vertex Contructor
		/*!
		Empty contructor
		*/
		Vertex() {}
		//! Vertex Contructor
		/*!
		Sets position of the vertex
		\param pos glm::vec3 vertex position
		*/
		Vertex(glm::vec3 pos)
		{
			position = pos;
		}
		//! Public glm::vec3 variable.
		/*! Position*/
		glm::vec3 position;
		//! Public glm::vec3 variable.
		/*! Normal Direction */
		glm::vec3 normal;
		//! Public glm::vec2 variable.
		/*! Texture Coordanates*/
		glm::vec2 textureCoords;
		//! Public glm::vec4 variable.
		/*! Vertex Tangents (used for normal mapping)*/
		glm::vec4 tangent;
	};
	//! Buffer 
	/*!
	Class used to pass buffer data to opengl and shaders
	Contained in the Engine::graphics namespace.
	*/
	class Buffer
	{
	private:
		//! Private GLuint variable.
		/*! Buffer Object ID */
		GLuint m_BufferID;
		//! Private GLuint variable.
		/*! Number of buffer components*/
		GLuint m_ComponentCount;
	public:
		//! Vertex Contructor
		/*!
		Sets position of the vertex
		\param data Vertex vertex data
		\param count GLsizei vertex count
		\param componentCount GLuint buffer component count
		*/
		Buffer(Vertex* data, GLsizei count, GLuint componentCount);
		//! Scene Destructor.
		/*!
		Cleans up memory in the class
		*/
		~Buffer();
		//! The Bind member function
		/*!
		Binds buffer object
		*/
		void bind() const;
		//! The UnBind member function
		/*!
		UnBinds buffer object
		*/
		void unbind() const;
		//! The getComponentCount member function
		/*!
		Returns the number of buffer components
		*/
		inline GLuint getComponentCount() const { return m_ComponentCount; }
		//! The getBufferID member function
		/*!
		Returns Buffer ID
		*/
		inline GLuint& getBufferID() { return m_BufferID; }

	private:

	};
}
}