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

	//! Material 
	/*!
	Holds all imfomation regarding an object's material.
	Inherits from Component.
	Contained in the Engine::Components namespace.
	*/
	class Material : public Component
	{
	public:
		//! Material Contructor
		/*!
		Sets up material shader
		\param shader Material Shader
		*/
		Material(graphics::Shader* shader) : m_Shader(shader) {};
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


		//void ReadShader()
		//{
		//	std::string vertString = m_Shader->getVertString();
		//	std::string fragString = m_Shader->getFragString();
		//	std::string word;
		//	std::vector<std::string> uniforms;

		//	std::istringstream iss(vertString);

		//	for (std::string line; std::getline(iss, line); )
		//	{
		//		std::istringstream lineStream(line);
		//		lineStream >> word;

		//		if (word == "uniform")
		//		{
		//			lineStream >> word;
		//			//std::cout << "Type: " << word << " ";
		//			lineStream >> word;
		//			//std::cout << "Name: " << word << endl;
		//			word.erase(std::remove(word.begin(), word.end(), ';'), word.end());
		//			if(std::find(uniforms.begin(), uniforms.end(), word) == uniforms.end())
		//				uniforms.push_back(word);
		//		}
		//	}

		//	iss = std::istringstream(fragString);

		//	for (std::string line; std::getline(iss, line); )
		//	{
		//		std::istringstream lineStream(line);
		//		lineStream >> word;

		//		if (word == "uniform")
		//		{
		//			lineStream >> word;
		//			//std::cout << "Type: " << word << " ";
		//			lineStream >> word;
		//			//std::cout << "Name: " << word << endl;
		//			word.erase(std::remove(word.begin(), word.end(), ';'), word.end());
		//			if (std::find(uniforms.begin(), uniforms.end(), word) == uniforms.end())
		//				uniforms.push_back(word);
		//		}
		//	}

		//	for (int i = 0; i < uniforms.size(); i++)
		//	{
		//		std::cout << "Name: " << uniforms[i] << std::endl;
		//	}
		//}

		//! The ChangeShader member function
		/*!
		Changes material shader to a given shader
		\param shader Material Shader
		*/
		const void ChangeShader(graphics::Shader* shader) { m_Shader = shader; };

		/** @defgroup GetSets For Textures
		*  Get, Add and Remove functions for all textures
		*  @{
		*/
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

		const void AddMetal(const char* filePath)
		{
			m_Metalic = new Texture(filePath);
		}
		const void RemoveMetal() { delete m_Metalic; m_Metalic = nullptr; }
		const Texture* GetMetal() const { return m_Metalic; }

		const void AddRough(const char* filePath)
		{
			m_Roughness = new Texture(filePath);
		}
		const void RemoveRough() { delete m_Roughness; m_Roughness = nullptr; }
		const Texture* GetRough() const { return m_Roughness; }

		const void SetColour(glm::vec3 colour) { m_MaterialColour = colour; }
		const glm::vec3 GetColour() const { return m_MaterialColour; }

		const void SetSpecColour(glm::vec3 colour) { m_MaterialSpecular = colour; }
		const glm::vec3 GetSpecColour() const { return m_MaterialSpecular; }

		const void SetMetalness(float metalness) { m_fMetalness = metalness; }
		const float GetMetalness() const { return m_fMetalness; }
		/** @} */ //

		//! The BindTextures member function
		/*!
		Binds all values and textures and passes them to the material shader
		*/
		void BindTextures()
		{
			m_Shader->setUniform3f("material.Color", m_MaterialColour);
			m_Shader->setUniform3f("material.Spec", m_MaterialSpecular);
			m_Shader->setUniform1f("material.Metalness", m_fMetalness);
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
			if (m_Metalic)
			{
				m_Shader->setUniform1i("UseMetalic", GL_TRUE);
				m_Metalic->bindTexture(m_Shader, GL_TEXTURE4, 4, "metalic2D");
			}
			else
			{
				m_Shader->setUniform1i("UseMetalic", GL_FALSE);
			}
			if (m_Roughness)
			{
				m_Shader->setUniform1i("UseRough", GL_TRUE);
				m_Roughness->bindTexture(m_Shader, GL_TEXTURE5, 5, "rough2D");
			}
			else
			{
				m_Shader->setUniform1i("UseRough", GL_FALSE);
			}
		
		}
		//! The Save member function
		/*!
		Saves Material settings and fiel paths to the save text file
		\param file ofstream File stream to open save file
		*/
		void Save(std::ofstream& file)
		{
			file << "#Material" << std::endl;
			file << "Color: " + std::to_string(m_MaterialColour.x) + " " + std::to_string(m_MaterialColour.y) + " " + std::to_string(m_MaterialColour.z) << std::endl;
			
			if (m_Albedo)
				file << m_Albedo->Path();
			file << std::endl;

			if (m_Specular)
				file << m_Specular->Path();
			file << std::endl;

			if (m_Normal)
				file << m_Normal->Path();
			file << std::endl;

			if (m_Metalic)
				file << m_Metalic->Path();
			file << std::endl;

			if (m_Roughness)
				file << m_Roughness->Path();
			file << std::endl;

			file << "Metalness: " << m_fMetalness << std::endl;
		}
		
	private:
		//! Private Shader pointer.
		/*! Material shader*/
		graphics::Shader* m_Shader;
		//! Private Texture pointer.
		/*! Colour Texture*/
		Texture* m_Albedo;
		//! Private Texture pointer.
		/*! Specular Texture*/
		Texture* m_Specular;
		//! Private Texture pointer.
		/*! Normal Texture*/
		Texture* m_Normal;
		//! Private Texture pointer.
		/*! Metalic Texture*/
		Texture* m_Metalic;
		//! Private Texture pointer.
		/*! Roughness Texture*/
		Texture* m_Roughness;
		//! Private glm::vec3 variable.
		/*! Material Colour*/
		glm::vec3 m_MaterialColour;
		//! Private glm::vec3 variable.
		/*! Material Specular Colour*/
		glm::vec3 m_MaterialSpecular;
		//! Private float variable.
		/*! Material Metalness value (used for reflections)*/
		float m_fMetalness;

	};
} }