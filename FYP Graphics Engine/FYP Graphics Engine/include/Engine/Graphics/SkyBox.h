#pragma once
#include "buffer.h"
#include "vertexArray.h"
#include "shader.h"
#include <stb_image.h>
#include <glm.hpp>
#include <vector>
namespace Engine {
	namespace graphics {

		//! SkyBox Class 
		/*!
		Contains infomation used for creating and drawing a cubemap based skybox.
		Contained within the Engine::graphics namespace.
		*/
		class SkyBox
		{
		public:
			//! SkyBox Contructor
			/*!
			Takes in the path to the textures and the path to the skybox shaders.
			Skyboxes do not receive light and also require the use of a samplerCube in the shader,
			due to this the skybox holds its own shader.
			*/
			SkyBox(std::string texturePath, const char* shaderPath);
			//! SkyBox Destructor.
			/*!
			Cleans up memory in the class
			*/
			~SkyBox();
			//! The Draw member function
			/*!
			Used to render the skybox
			*/
			void Draw(glm::mat4 proj, glm::mat4 viewRot) const;
			//! The ChangeTexture member function
			/*!
			Opens dialogue to select a file. Uses file path to select new textures.
			\param texturePath Path to new textures
			*/
			void ChangeTexture(std::string texturePath);
			//! The Path member function
			/*!
			Returns the current texture path as a string.
			*/
			std::string Path() { return m_cCubemapPath; };
			//! The GetTexture member function
			/*!
			Returns the texture ID of the cubemap texture
			*/
			GLuint GetTexture() { return m_Texture; }
		private:
			//! Private VertexArray pointer variable.
			/*! Holds the memory alocation of the VertexArray for drawing the skybox */
			VertexArray* m_VertexArray = nullptr;
			//! Private GLuint variable.
			/*! Holds the GLuint texture  */
			GLuint m_Texture;
			//! Private Shader pointer variable.
			/*! Holds the memory alocation of the Shader used for drawing the skybox */
			Shader* m_Shader;
			//! Private const char pointer variable.
			/*! Holds the memory alocation of the file path to the cubemap folder */
			std::string m_cCubemapPath;
			//! Private GLuint vector
			/*! Contains the indecies of the cubes vertacies */
			std::vector<GLuint> m_Indecies;
			//! Private GLuint variable
			/*! Element Buffer Object */
			GLuint EBO;


		};
	}
}