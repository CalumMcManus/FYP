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
			bool m_bHasAlbedo = false;
			bool m_bHasSpecular = false;
			bool m_bHasNormal = false;
			Components::Material* m_SelectedMaterial;
			nanogui::Window* m_MaterialWindow;

			nanogui::Button* m_AbledoBtn;
			nanogui::Button* m_SpecBtn;
			nanogui::Button* m_NormBtn;

			nanogui::ColorPicker* m_MaterialColor;

		};

}	}
