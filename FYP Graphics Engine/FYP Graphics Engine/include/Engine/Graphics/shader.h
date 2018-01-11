#pragma once
#include <vector>
#include <iostream>
#include <glad/glad.h>
#include <Util\fileutil.h>

#include <glm.hpp>
namespace Engine {
	namespace graphics {

		//! Shader
		/*!
		This class deals loading and passing infomation to a shader
		Contained in the Engine::graphics namespace.
		*/
		class Shader
		{
		private:
			//! Private GLuint variable.
			/*! Holds the id referance of the shader */
			GLuint m_ShaderID;
			//! Private const char pointer variables.
			/*! Holds the paths to the vertex and fragment shaders */
			const char *m_VertPath, *m_FragPath;

			//! Holds the file strings
			/*! Used for checking uniform stypes */
			std::string m_VertSourceString;
			std::string m_FragSourceString;
		public:
			//! Shader Contructor
			/*!
			The contructor takes in the file paths to the vertex and fragment shaders.
			Calls load to load in data from the shaders.
			*/
			Shader(const char* vertPath, const char* fragPath);
			//! Shader Destructor.
			/*!
			Cleans up memory in the class
			*/
			~Shader();

			//! The set member functions
			/*!
			These function are used to pass the relevent data to the shader (e.g. floats, intergers, vectors and matrices
			*/
			void setUniform1f(const GLchar* name, float value);
			void setUniform1i(const GLchar* name, int value);
			void setUniform2f(const GLchar* name, glm::vec2& vector);
			void setUniform3f(const GLchar* name, glm::vec3& vector);
			void setUniform4f(const GLchar* name, glm::vec4& vector);
			void setUniformMat3(const GLchar* name, const glm::mat3& matrix);
			void setUniformMat4(const GLchar* name, const glm::mat4& matrix);

			//! The enable member function
			/*!
			Binds the shader for use
			*/
			void enable() const;
			//! The disable member function
			/*!
			Unbinds the shader
			*/
			void disable() const;
			//! The getID member function
			/*!
			Returns the ID value for referencing it
			*/
			const GLuint getID() const { return m_ShaderID; }

			const std::string getVertString() const { return m_VertSourceString; }
			const std::string getFragString() const { return m_FragSourceString; }
		private:
			//! The load member function
			/*!
			Loads in shader data from the .vert and .frag files and creates opengl programs based on that data
			*/
			GLuint load();
			//! The getUniformLocation member function
			/*!
			Returns a GLint location of a uniform variable
			*/
			GLint getUniformLocation(const GLchar* name);
		};
	}
}
