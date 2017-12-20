#pragma once
#include <Component\component.h>
#include <unordered_map>
#include <typeindex>

using namespace Engine::Components;
class GameObject
{
public:
	template <typename T>
	T* getComponent()
	{

		auto iter = m_components.find(typeid(T));

		if (iter != std::end(m_components))
		{

			return dynamic_cast<T*>(iter->second);
		}

		return nullptr;

	}

	template <typename T>
	void addComponent(T* comp)
	{
		m_components[typeid(T)] = comp;
	}

private:
	std::unordered_map<std::type_index, Component*> m_components;
};
