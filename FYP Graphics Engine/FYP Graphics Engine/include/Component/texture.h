#pragma once
#include "component.h"
#include <Engine\Graphics\shader.h>
#include <glad\glad.h>
#include <stb_image.h>
#include <iostream>
namespace Engine {
	namespace Components {
		//! Texture 
		/*!
		Holds all imfomation regarding a texture for displaying on a model
		Inherits from Component.
		Contained in the Engine::Components namespace.
		*/
		class Texture : public Component
		{
		public:
			//! ModelRenderer Contructor
			/*!
			Loads and sets up Texture file
			\param filePath File path of texture
			*/
			Texture(const char* filePath) 
			{ 
				m_Path = filePath;
				glActiveTexture(GL_TEXTURE2);
				glGenTextures(1, &m_TextureID);

				int width, height, nrComponents;
				unsigned char* textureData = stbi_load(filePath, &width, &height, &nrComponents, 0);
				if (textureData)
				{
					GLenum format;
					if (nrComponents == 1)
						format = GL_RED;
					else if (nrComponents == 3)
						format = GL_RGB;
					else if (nrComponents == 4)
						format = GL_RGBA;

					glBindTexture(GL_TEXTURE_2D, m_TextureID);
					glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, textureData);
					glGenerateMipmap(GL_TEXTURE_2D);

					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

					stbi_image_free(textureData);
				}
				else
				{
					std::cout << "Texture failed to load from: " << filePath << std::endl;
					stbi_image_free(textureData);
				}
			};
			//! Texture Destructor
			/*!
			Cleans up memory in the class
			*/
			~Texture()
			{
				glDeleteTextures(1, &m_TextureID);
			};
			//! The Update member function
			/*!
			Function for Updating components each frame
			\param deltaTime float Time passed since last frame
			*/
			void Update(float deltaTime) override {};
			//! The Message member function
			/*!
			Function for Sending messages to components
			\param message const char pointer Message
			*/
			void Message(const char* message) override {};
			//! The bindTexture member function
			/*!
			Binds this texture to the given shader
			\param shader Shader pointer from material
			\param activeTex GLenum Active Texture Enum
			\param id int Active Texture ID
			\param textureName const char* Name of texture in shader
			*/
			void bindTexture(graphics::Shader* shader, GLenum activeTex, int id, const char* textureName) 
			{ 	
				GLint baseImageLoc = glGetUniformLocation(shader->getID(), textureName);
				glUniform1i(baseImageLoc, id);
				glActiveTexture(activeTex);
				glBindTexture(GL_TEXTURE_2D, m_TextureID); 
			}
			//! The getTextureID member function
			/*!
			Returns the texture ID
			*/
			GLuint getTextureID() { return m_TextureID; }
			//! The Path member function
			/*!
			Returns the file path of the texture
			*/
			const std::string Path() const { return m_Path; }
		private:
			//! Private GLuint variable.
			/*! Component Texture ID */
			GLuint m_TextureID;
			//! Private string variable.
			/*! Path to texture file */
			std::string m_Path;

		};

	}
}