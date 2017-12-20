#pragma once
#include "component.h"
#include <GL\glew.h>
#include <stb_image.h>
#include <iostream>
namespace Engine {
	namespace Components {

		class Texture : public Component
		{
		public:
			Texture(const char* filePath) 
			{ 
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
			void Update(float deltaTime) override {};
			void Message(const char* message) override {};

			void bindTexture() { glBindTexture(GL_TEXTURE_2D, m_TextureID); }
			GLuint getTextureID() { return m_TextureID; }
		private:
			GLuint m_TextureID;


		};

	}
}