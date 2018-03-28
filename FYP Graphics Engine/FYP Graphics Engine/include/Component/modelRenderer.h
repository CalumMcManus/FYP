#pragma once
#include "component.h"
#include <Engine\Graphics\Model.h>
namespace Engine { namespace Components {
		
	class ModelRenderer : public Component
	{
	public:
		ModelRenderer(const char* meshPath) 
		{ 
			m_Model = new graphics::Model(meshPath);
			m_Path = meshPath;
		};
		void Update(float deltaTime) override {};
		void Message(const char* message) override {};

		graphics::Model& getModel() { return *m_Model; }
		const std::string Path() const { return m_Path; }
	private:
		graphics::Model* m_Model;
		std::string m_Path;


	};

} 
}