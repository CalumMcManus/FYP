#include <Engine\Graphics\SkyBox.h>

Engine::graphics::SkyBox::SkyBox(const char * texturePath, const char *shaderPath)
{
	m_Shader = new Shader("../Assets/Shaders/basicCubeMap.vert", "../Assets/Shaders/basicCubeMap.frag");
	m_cCubemapPath = texturePath;

	Vertex vertices[8] = {
		Vertex(glm::vec3(-1, -1, 1)),
		Vertex(glm::vec3(1, -1, 1)),
		Vertex(glm::vec3(1, 1, 1)),
		Vertex(glm::vec3(-1, 1, 1)),

		Vertex(glm::vec3(-1, -1, -1)),
		Vertex(glm::vec3(1, -1, -1)),
		Vertex(glm::vec3(1, 1, -1)),
		Vertex(glm::vec3(-1, 1, -1)),

	};
	m_Indecies =
	{
		// front
		0, 1, 2,
		2, 3, 0,
		// top
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// bottom
		4, 0, 3,
		3, 7, 4,
		// left
		4, 5, 1,
		1, 0, 4,
		// right
		3, 2, 6,
		6, 7, 3
	};

	m_VertexArray = new VertexArray();
	m_VertexArray->addBuffer(new Buffer(&vertices[0], 8, 3), 0);

	glGenBuffers(1, &EBO);
	m_VertexArray->bind();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indecies.size() * sizeof(GLuint), &m_Indecies[0], GL_STATIC_DRAW);
	
	glGenTextures(1, &m_Texture);

	GLuint sides[] =
	{
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

	for (int i = 0; i < 6; i++)
	{
		std::string TextureDir = m_cCubemapPath + std::to_string(i + 1) + ".bmp";
		int width, height, nrComponents;
		unsigned char* textureData = stbi_load(TextureDir.c_str(), &width, &height, &nrComponents, 0);
		if (textureData)
		{			
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_Texture);
			glTexImage2D(sides[i],
				0,
				GL_RGB,
				width,
				height,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				textureData);

			stbi_image_free(textureData);
		}
		else
		{
			std::cout << "Texture failed to load from: " << m_cCubemapPath << std::endl;
			stbi_image_free(textureData);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

Engine::graphics::SkyBox::~SkyBox()
{
	delete m_VertexArray;
}

void Engine::graphics::SkyBox::Draw(glm::mat4 proj, glm::mat4 viewRot) const
{
	if (glIsTexture(m_Texture))
	{
		glDepthMask(0);
		glCullFace(GL_FRONT);
		m_Shader->enable();

		m_Shader->setUniformMat4("V", viewRot);
		m_Shader->setUniformMat4("P", proj);

		m_VertexArray->bind();
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_Texture);

		glDrawElements(GL_TRIANGLES, m_Indecies.size(), GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);
		m_VertexArray->unbind();
		//m_Shader->disable();
		glDepthMask(1);
		glCullFace(GL_BACK);
	}
}
