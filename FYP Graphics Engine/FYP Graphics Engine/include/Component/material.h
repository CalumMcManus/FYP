#pragma once

#include "component.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <Engine\Graphics\shader.h>
#include <Component\texture.h>
#include <fstream>
namespace Engine {	namespace Components {

	class Material : public Component
	{
	public:
		Material(graphics::Shader* shader) : m_Shader(shader) {};
		void Update(float deltaTime) override {};
		void Message(const char* message) override {};


		void ReadShader()
		{
			std::string vertString = m_Shader->getVertString();
			std::string fragString = m_Shader->getFragString();
			std::string word;
			std::vector<std::string> uniforms;

			std::istringstream iss(vertString);

			for (std::string line; std::getline(iss, line); )
			{
				std::istringstream lineStream(line);
				lineStream >> word;

				if (word == "uniform")
				{
					lineStream >> word;
					//std::cout << "Type: " << word << " ";
					lineStream >> word;
					//std::cout << "Name: " << word << endl;
					word.erase(std::remove(word.begin(), word.end(), ';'), word.end());
					if(std::find(uniforms.begin(), uniforms.end(), word) == uniforms.end())
						uniforms.push_back(word);
				}
			}

			iss = std::istringstream(fragString);

			for (std::string line; std::getline(iss, line); )
			{
				std::istringstream lineStream(line);
				lineStream >> word;

				if (word == "uniform")
				{
					lineStream >> word;
					//std::cout << "Type: " << word << " ";
					lineStream >> word;
					//std::cout << "Name: " << word << endl;
					word.erase(std::remove(word.begin(), word.end(), ';'), word.end());
					if (std::find(uniforms.begin(), uniforms.end(), word) == uniforms.end())
						uniforms.push_back(word);
				}
			}

			for (int i = 0; i < uniforms.size(); i++)
			{
				std::cout << "Name: " << uniforms[i] << std::endl;
			}
		}
		const void ChangeShader(graphics::Shader* shader) { m_Shader = shader; };

		//Get, Set and Remove
		const void AddAlbedo(const char* filePath)
		{
			m_Albedo = new Texture(filePath);
		}
		const void RemoveAlbedo() { delete m_Albedo; m_Albedo = nullptr; }
		const Texture* GetAlbedo() const { return m_Albedo; }

		const void AddSpecular(const char* filePath)
		{
			m_Specular = new Texture(filePath);
		}
		const void RemoveSpecular() { delete m_Specular; m_Specular = nullptr; }
		const Texture* GetSpecular() const { return m_Specular; }

		const void AddNormal(const char* filePath)
		{
			m_Normal = new Texture(filePath);
		}
		const void RemoveNormal() { delete m_Normal; m_Normal = nullptr; }
		const Texture* GetNormal() const { return m_Normal; }

		const void SetColour(glm::vec3 colour) { m_MaterialColour = colour; }
		const glm::vec3 GetColour() const { return m_MaterialColour; }

		void BindTextures()
		{
			m_Shader->setUniform3f("material.Color", m_MaterialColour);
			if (m_Albedo)
			{
				m_Shader->setUniform1i("UseTexture", GL_TRUE);
				m_Albedo->bindTexture(m_Shader, GL_TEXTURE1, 1, "texture2D");
			}
			else
			{
				m_Shader->setUniform1i("UseTexture", GL_FALSE);
			}
			if (m_Specular)
			{
				m_Shader->setUniform1i("UseSpecular", GL_TRUE);
				m_Specular->bindTexture(m_Shader, GL_TEXTURE2, 2, "specular2D");

			}
			else
			{
				m_Shader->setUniform1i("UseSpecular", GL_FALSE);
			}
			if (m_Normal)
			{
				m_Shader->setUniform1i("UseNormal", GL_TRUE);
				m_Normal->bindTexture(m_Shader, GL_TEXTURE3, 3, "normal2D");
			}
			else
			{
				m_Shader->setUniform1i("UseNormal", GL_FALSE);
			}
		}

		void Save(std::ofstream& file)
		{
			file << "#Material" << std::endl;
			file << "Color: " + to_string(m_MaterialColour.x) + " " + to_string(m_MaterialColour.y) + " " + to_string(m_MaterialColour.z) << std::endl;
			
			if (m_Albedo)
				file << m_Albedo->Path();
			file << std::endl;

			if (m_Specular)
				file << m_Specular->Path();
			file << std::endl;

			if (m_Normal)
				file << m_Normal->Path();
			file << std::endl;
		}
		
	private:
		graphics::Shader* m_Shader;
		Texture* m_Albedo;
		Texture* m_Specular;
		Texture* m_Normal;

		glm::vec3 m_MaterialColour;

	};
} }