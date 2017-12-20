#pragma once

#include "component.h"
#include <Engine\Graphics\shader.h>
namespace Engine {	namespace Components {

	class Material : public Component
	{
	public:
		Material(const char* shaderPath);
		void Update(float deltaTime) override {};
		void Message(const char* message) override {};
	private:
		graphics::Shader* m_Shader;
	};
} }