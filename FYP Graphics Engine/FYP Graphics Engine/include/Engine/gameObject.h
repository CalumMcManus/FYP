///Taken from Games Programming Module
#pragma once
#include <Component\component.h>
#include <unordered_map>
#include <typeindex>

using namespace Engine::Components;
//! GameObject 
/*!
GameObject class that stores Component classes.
Contained in the Engine::Components namespace.
*/
class GameObject
{
public:
	template <typename T>
	//! The getComponent member function
	/*!
	Returns requested component
	*/
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
	//! The addComponent member function
	/*!
	Adds requested component to GameObject
	*/
	void addComponent(T* comp)
	{
		m_components[typeid(T)] = comp;
	}

private:
	//! Private unordered_map of Component pointers.
	/*! unordered map containing all Components added to this GameObject*/
	std::unordered_map<std::type_index, Component*> m_components;
};
