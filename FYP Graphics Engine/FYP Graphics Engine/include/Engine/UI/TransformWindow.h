#pragma once
#include <nanogui\nanogui.h>
#include <Engine\Engine.h>
#include <Component\transform.h>
#include <Engine\Graphics\postProcessingStack.h>
namespace Engine { namespace UI {

	class TransformWindow
	{
	public:
		TransformWindow(Engine::GLFWEngine* enginePointer);
		void SelectTransform(Components::Transform* selectedTransform);
		void SelectLight(graphics::PostProcessingStack::Light* selectedLight);
		void Clear();

		bool IsMouseOver() { return m_TransformWindow->focused(); }
	private:
		bool m_bSelectedLight = true;
		Components::Transform* m_SelectedTransform;
		graphics::PostProcessingStack::Light* m_SelectedLight;
		nanogui::Window* m_TransformWindow;

		nanogui::Label* m_PosLable;
		nanogui::Label* m_RotLable;
		nanogui::Label* m_ScaleLable;

		nanogui::Label* m_ScaleXLable;
		nanogui::Label* m_ScaleYLable;
		nanogui::Label* m_ScaleZLable;

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