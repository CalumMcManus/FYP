#pragma once

#include "component.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <Engine\Graphics\shader.h>
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
	private:
		graphics::Shader* m_Shader;

	};
} }