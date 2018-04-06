#pragma once
#include <nanogui\nanogui.h>
#include <Engine\Engine.h>
#include <Component\material.h>
#include <Util\fileutil.h>
namespace Engine { namespace UI {

		class MaterialWindow
		{
		public:
			MaterialWindow(Engine::GLFWEngine* enginePointer);
			void SelectMaterial(Components::Material* selectedTransform);
			void Clear();

			bool IsMouseOver() { return m_MaterialWindow->focused(); }
		private:
			GLFWEngine* m_EnginePointer;

			bool m_bHasAlbedo = false;
			bool m_bHasSpecular = false;
			bool m_bHasNormal = false;
			bool m_bHasMetal = false;
			bool m_bHasRough = false;
			Components::Material* m_SelectedMaterial;
			nanogui::Window* m_MaterialWindow;

			nanogui::Button* m_AbledoBtn;
			nanogui::Button* m_SpecBtn;
			nanogui::Button* m_NormBtn;
			nanogui::Button* m_MetalBtn;
			nanogui::Button* m_RoughBtn;

			nanogui::ColorPicker* m_MaterialColor;
			nanogui::ColorPicker* m_MaterialSpecular;

			std::string GetTexture(std::string title);

			nanogui::Widget* m_ReflectWidget;
			nanogui::Slider* m_ReflectSlider;
			nanogui::TextBox* m_TextboxReflect;
			float m_fReflect = 0;
			

		};

}	}
