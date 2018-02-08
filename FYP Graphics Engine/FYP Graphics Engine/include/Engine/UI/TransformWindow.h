#pragma once
#include <nanogui\nanogui.h>
#include <Engine\Engine.h>
#include <Component\transform.h>
namespace Engine { namespace UI {

	class TransformWindow
	{
	public:
		TransformWindow(Engine::GLFWEngine* enginePointer);
		void SelectTransform(Components::Transform* selectedTransform);
	private:
		Components::Transform* m_SelectedTransform;
		nanogui::Window* m_TransformWindow;

		nanogui::FloatBox<float> *posX;
		nanogui::FloatBox<float> *posY;
		nanogui::FloatBox<float> *posZ;
		nanogui::FloatBox<float> *rotX;
		nanogui::FloatBox<float> *rotY;
		nanogui::FloatBox<float> *rotZ;
		nanogui::FloatBox<float> *scaleX;
		nanogui::FloatBox<float> *scaleY;
		nanogui::FloatBox<float> *scaleZ;

	};

}}