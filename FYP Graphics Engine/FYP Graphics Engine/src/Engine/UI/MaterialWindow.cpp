#include <Engine\UI\MaterialWindow.h>

Engine::UI::MaterialWindow::MaterialWindow(Engine::GLFWEngine * enginePointer)
{
	m_MaterialWindow = new nanogui::Window(enginePointer->m_Window, "Material");
	
	nanogui::GroupLayout* layout = new nanogui::GroupLayout(15, 6, 14, 20);
	m_MaterialWindow->setLayout(layout);
	
	m_AbledoBtn = new nanogui::Button(m_MaterialWindow, "Add Abedo Texture", 0);
	m_AbledoBtn->setCallback([&]
	{
		if (m_SelectedMaterial != nullptr)
		{	
			m_bHasAlbedo = !m_bHasAlbedo;
			if (m_bHasAlbedo) {
				m_SelectedMaterial->AddAlbedo(FileUtils::BrowseFiles("Select Albedo Texture").c_str());
				m_AbledoBtn->setCaption("Remove Albedo Texture");
			}
			else {
				m_SelectedMaterial->RemoveAlbedo();
				m_AbledoBtn->setCaption("Add Albedo Texture");
			}
		}
	});
	m_SpecBtn = new nanogui::Button(m_MaterialWindow, "Add Specular Texture", 0);
	m_SpecBtn->setCallback([&]
	{
		if (m_SelectedMaterial != nullptr)
		{	
			m_bHasSpecular = !m_bHasSpecular;
			if (m_bHasSpecular) {
				m_SelectedMaterial->AddSpecular(FileUtils::BrowseFiles("Select Specular Texture").c_str());
				m_SpecBtn->setCaption("Remove Specular Texture");
			}
			else {
				m_SelectedMaterial->RemoveSpecular();
				m_SpecBtn->setCaption("Add Specular Texture");
			}
		}
	});
	m_NormBtn = new nanogui::Button(m_MaterialWindow, "Add Normal Texture", 0);
	m_NormBtn->setCallback([&]
	{
		if (m_SelectedMaterial != nullptr)
		{
			m_bHasNormal = !m_bHasNormal;
			if (m_bHasNormal) {
				m_SelectedMaterial->AddNormal(FileUtils::BrowseFiles("Select Normal Texture").c_str());
				m_NormBtn->setCaption("Remove Normal Texture");
			}
			else {
				m_SelectedMaterial->RemoveNormal();
				m_NormBtn->setCaption("Add Normal Texture");
			}
		}
	});

	nanogui::Widget *materialColor = new nanogui::Widget(m_MaterialWindow);
	materialColor->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Horizontal,
		nanogui::Alignment::Maximum, 0, 10));
	materialColor->setSize(nanogui::Vector2i(100, 20));
	materialColor->add<nanogui::Label>("Material Color", "sans-bold", 15);
	m_MaterialColor = new nanogui::ColorPicker(materialColor);
	m_MaterialColor->setCallback([&](nanogui::Color value)
	{
		if (m_SelectedMaterial != nullptr)
		{
			m_SelectedMaterial->SetColour(glm::vec3(value.x(), value.y(), value.z()));
		}
	});
}

void Engine::UI::MaterialWindow::SelectMaterial(Components::Material * selectedMaterial)
{
	m_SelectedMaterial = selectedMaterial;

	m_bHasAlbedo = selectedMaterial->GetAlbedo() != nullptr;
	m_bHasSpecular = selectedMaterial->GetSpecular() != nullptr;
	m_bHasNormal = selectedMaterial->GetNormal() != nullptr;

	if (m_bHasAlbedo) m_AbledoBtn->setCaption("Remove Albedo Texture"); else m_AbledoBtn->setCaption("Add Albedo Texture");
	if (m_bHasSpecular) m_SpecBtn->setCaption("Remove Specular Texture"); else m_SpecBtn->setCaption("Add Specular Texture");
	if (m_bHasNormal) m_NormBtn->setCaption("Remove Normal Texture"); else m_NormBtn->setCaption("Add Normal Texture");
	glm::vec3 tempColour = selectedMaterial->GetColour();
	m_MaterialColor->setColor(nanogui::Color(tempColour.x, tempColour.y, tempColour.z, 1.f));
}

void Engine::UI::MaterialWindow::Clear()
{
	m_SelectedMaterial = nullptr;
}
