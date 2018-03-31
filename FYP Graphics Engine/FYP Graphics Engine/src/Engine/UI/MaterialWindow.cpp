#include <Engine\UI\MaterialWindow.h>

Engine::UI::MaterialWindow::MaterialWindow(Engine::GLFWEngine * enginePointer)
{
	m_MaterialWindow = new nanogui::Window(enginePointer->m_Window, "Material");
	
	nanogui::GroupLayout* layout = new nanogui::GroupLayout(15, 6, 14, 20);
	m_MaterialWindow->setLayout(layout);
	
	m_AbledoBtn = new nanogui::Button(m_MaterialWindow, "Add Albedo Texture", 0);
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
	m_MetalBtn = new nanogui::Button(m_MaterialWindow, "Add Metallic Texture", 0);
	m_MetalBtn->setCallback([&]
	{
		if (m_SelectedMaterial != nullptr)
		{
			m_bHasMetal = !m_bHasMetal;
			if (m_bHasMetal) {
				m_SelectedMaterial->AddMetal(FileUtils::BrowseFiles("Select Metallic Texture").c_str());
				m_MetalBtn->setCaption("Remove Metallic Texture");
			}
			else {
				m_SelectedMaterial->RemoveMetal();
				m_MetalBtn->setCaption("Add Metallic Texture");
			}
		}
	});
	m_RoughBtn = new nanogui::Button(m_MaterialWindow, "Add Rough Texture", 0);
	m_RoughBtn->setCallback([&]
	{
		if (m_SelectedMaterial != nullptr)
		{
			m_bHasRough = !m_bHasRough;
			if (m_bHasRough) {
				m_SelectedMaterial->AddRough(FileUtils::BrowseFiles("Select Roughness Texture").c_str());
				m_RoughBtn->setCaption("Remove Rough Texture");
			}
			else {
				m_SelectedMaterial->RemoveRough();
				m_RoughBtn->setCaption("Add Rough Texture");
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

	nanogui::Widget *materialSpec = new nanogui::Widget(m_MaterialWindow);
	materialSpec->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Horizontal,
		nanogui::Alignment::Maximum, 0, 10));
	materialSpec->setSize(nanogui::Vector2i(100, 20));
	materialSpec->add<nanogui::Label>("Material Spec", "sans-bold", 15);
	m_MaterialSpecular = new nanogui::ColorPicker(materialSpec);
	m_MaterialSpecular->setCallback([&](nanogui::Color value)
	{
		if (m_SelectedMaterial != nullptr)
		{
			m_SelectedMaterial->SetSpecColour(glm::vec3(value.x(), value.y(), value.z()));
		}
	});
}

void Engine::UI::MaterialWindow::SelectMaterial(Components::Material * selectedMaterial)
{
	m_SelectedMaterial = selectedMaterial;

	m_bHasAlbedo = selectedMaterial->GetAlbedo() != nullptr;
	m_bHasSpecular = selectedMaterial->GetSpecular() != nullptr;
	m_bHasNormal = selectedMaterial->GetNormal() != nullptr;
	m_bHasMetal = selectedMaterial->GetMetal() != nullptr;
	m_bHasRough = selectedMaterial->GetRough() != nullptr;

	if (m_bHasAlbedo) m_AbledoBtn->setCaption("Remove Albedo Texture"); else m_AbledoBtn->setCaption("Add Albedo Texture");
	if (m_bHasSpecular) m_SpecBtn->setCaption("Remove Specular Texture"); else m_SpecBtn->setCaption("Add Specular Texture");
	if (m_bHasNormal) m_NormBtn->setCaption("Remove Normal Texture"); else m_NormBtn->setCaption("Add Normal Texture");
	if (m_bHasMetal) m_MetalBtn->setCaption("Remove Metallic Texture"); else m_MetalBtn->setCaption("Add Metallic Texture");
	if (m_bHasRough) m_RoughBtn->setCaption("Remove Rough Texture"); else m_RoughBtn->setCaption("Add Rough Texture");
	glm::vec3 tempColour = selectedMaterial->GetColour();
	m_MaterialColor->setColor(nanogui::Color(tempColour.x, tempColour.y, tempColour.z, 1.f));
	tempColour = selectedMaterial->GetSpecColour();
	m_MaterialSpecular->setColor(nanogui::Color(tempColour.x, tempColour.y, tempColour.z, 1.f));
}

void Engine::UI::MaterialWindow::Clear()
{
	m_SelectedMaterial = nullptr;
}
