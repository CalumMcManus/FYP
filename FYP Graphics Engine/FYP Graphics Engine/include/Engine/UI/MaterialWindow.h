#pragma once
#include <nanogui\nanogui.h>
#include <Engine\Engine.h>
#include <Component\material.h>
#include <Util\fileutil.h>
namespace Engine { namespace UI {
		//! MaterialWindow 
		/*!
		UI class for displaying options for altering materials.
		Contained in the Engine::UI namespace.
		*/
		class MaterialWindow
		{
		public:
			//! MaterialWindow Contructor
			/*!
			Calls fucntion for inital set up of a UI Window
			\param enginePointer Referance to the GLFW engine
			*/
			MaterialWindow(Engine::GLFWEngine* enginePointer);
			//! The SelectMaterial member function
			/*!
			Changes UI to resemble a selected material
			\param selectedMaterial Pointer to a selected material
			*/
			void SelectMaterial(Components::Material* selectedMaterial);
			//! The Clear member function
			/*!
			Resets UI and clears selected transform pointer
			*/
			void Clear();
			//! The IsMouseOver member function
			/*!
			Returns true if the mouse is over this window
			*/
			bool IsMouseOver() { return m_MaterialWindow->focused(); }
		private:
			//! Private GLFWEngine pointer.
			/*! Pointer to the GLFWEngine class, used to access Engine variables and functions*/
			GLFWEngine* m_EnginePointer;

			//! Private bool varible.
			/*! True if the selected material has an Albedo Texture*/
			bool m_bHasAlbedo = false;
			//! Private bool varible.
			/*! True if the selected material has an Specular Texture*/
			bool m_bHasSpecular = false;
			//! Private bool varible.
			/*! True if the selected material has an Normal Texture*/
			bool m_bHasNormal = false;
			//! Private bool varible.
			/*! True if the selected material has an Metalness Texture*/
			bool m_bHasMetal = false;
			//! Private bool varible.
			/*! True if the selected material has an Roughness Texture*/
			bool m_bHasRough = false;
			//! Private Material pointer.
			/*! Pointer to the selected material */
			Components::Material* m_SelectedMaterial;

			//! The GetTexture member function
			/*!
			Opens a new dialogue to select a texture.
			\param title Title of dialogue window
			*/
			std::string GetTexture(std::string title);

			//UI Variables
			nanogui::Window* m_MaterialWindow;

			nanogui::Button* m_AbledoBtn;
			nanogui::Button* m_SpecBtn;
			nanogui::Button* m_NormBtn;
			nanogui::Button* m_MetalBtn;
			nanogui::Button* m_RoughBtn;

			nanogui::ColorPicker* m_MaterialColor;
			nanogui::ColorPicker* m_MaterialSpecular;

			nanogui::Widget* m_ReflectWidget;
			nanogui::Slider* m_ReflectSlider;
			nanogui::TextBox* m_TextboxReflect;
			float m_fReflect = 0;
			

		};

}	}
