#pragma once

namespace Engine { namespace Components {

	class Component
	{
	public:
		virtual void Update(float deltaTime) = 0;
		virtual void Message(const char* message) = 0;
	};
} 
}

