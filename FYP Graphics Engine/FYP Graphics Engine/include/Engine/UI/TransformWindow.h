#pragma once
#include <nanogui\nanogui.h>
#include <Engine\Engine.h>
#include <Component\transform.h>
#include <Engine\Graphics\postProcessingStack.h>
namespace Engine { namespace UI {
	//! TransformWindow 
	/*!
	UI class for displaying options for altering transforms.
	Contained in the Engine::UI namespace.
	*/
	class TransformWindow
	{
	public:
		//! MaterialWindow Contructor
		/*!
		Calls fucntion for inital set up of a UI Window
		\param enginePointer Referance to the GLFW engine
		*/
		TransformWindow(Engine::GLFWEngine* enginePointer);

		//! The SelectTransform member function
		/*!
		Changes UI to resemble a selected transform
		\param selectedTransform Pointer to a selected transform
		*/
		void SelectTransform(Components::Transform* selectedTransform);
		//! The SelectLight member function
		/*!
		Changes UI to resemble a selected light
		\param selectedLight Pointer to a selected light
		*/
		void SelectLight(graphics::PostProcessingStack::Light* selectedLight);
		//! The Clear member function
		/*!
		Resets UI and clears selected transform pointer
		*/
		void Clear();
		//! The IsMouseOver member function
		/*!
		Returns true if the mouse is over this window
		*/
		bool IsMouseOver() { return m_TransformWindow->focused(); }
	private:
		//! Private bool varible.
		/*! True if the slected object is a light*/
		bool m_bSelectedLight = true;
		//! Private Transform pointer.
		/*! Pointer to the selected Transform */
		Components::Transform* m_SelectedTransform;
		//! Private Light pointer.
		/*! Pointer to the selected Light */
		graphics::PostProcessingStack::Light* m_SelectedLight;

		//UI Variables
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