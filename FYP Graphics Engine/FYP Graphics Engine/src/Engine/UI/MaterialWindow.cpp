#include <Engine\UI\MaterialWindow.h>
#include <Engine\project.h>
Engine::UI::MaterialWindow::MaterialWindow(Engine::GLFWEngine * enginePointer)
{
	m_MaterialWindow = new nanogui::Window(enginePointer->m_Window, "Material");
	
	m_EnginePointer = enginePointer;

	nanogui::GroupLayout* layout = new nanogui::GroupLayout(15, 6, 14, 20);
	m_MaterialWindow->setLayout(layout);
	
	m_AbledoBtn = new nanogui::Button(m_MaterialWindow, "Add Albedo Texture", 0);
	m_AbledoBtn->setCallback([&]
	{
		if (m_SelectedMaterial != nullptr)
		{	
			m_bHasAlbedo = !m_bHasAlbedo;
			if (m_bHasAlbedo) {
				m_SelectedMaterial->AddAlbedo(GetTexture("Select Albedo Texture").c_str());
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
				m_SelectedMaterial->AddSpecular(GetTexture("Select Specular Texture").c_str());
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
				m_SelectedMaterial->AddNormal(GetTexture("Select Normal Texture").c_str());
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
				m_SelectedMaterial->AddMetal(GetTexture("Select Metallic Texture").c_str());
				m_MetalBtn->setCaption("Remove Metallic Texture");
			}
			else {
				m_SelectedMaterial->RemoveMetal();
				m_MetalBtn->setCaption("Add Metallic Texture");
			}
		}
	});

	nanogui::Widget *m_ReflectWidget = new nanogui::Widget(m_MaterialWindow);
	m_ReflectWidget->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Horizontal,
		nanogui::Alignment::Maximum, 0, 20));

	m_ReflectSlider = new nanogui::Slider(m_ReflectWidget);
	m_ReflectSlider->setValue(m_fReflect);
	m_TextboxReflect = new nanogui::TextBox(m_ReflectWidget);
	m_TextboxReflect->setFixedSize(Eigen::Vector2i(60, 25));
	m_TextboxReflect->setValue(std::to_string((int)(m_fReflect*100)).c_str());
	m_TextboxReflect->setUnits("%");
	m_ReflectSlider->setCallback([&] (float value)
	{
		m_fReflect = value;
		m_TextboxReflect->setValue(std::to_string((int)(m_fReflect*100)).c_str());

		if (m_SelectedMaterial != nullptr)
		{
			m_SelectedMaterial->SetMetalness(m_fReflect);
		}
	});
	m_TextboxReflect->setFixedSize(Eigen::Vector2i(60, 25));
	m_TextboxReflect->setFontSize(20);
	m_TextboxReflect->setAlignment(nanogui::TextBox::Alignment::Right);
	//m_ReflectWidget->setVisible(false);

	m_RoughBtn = new nanogui::Button(m_MaterialWindow, "Add Rough Texture", 0);
	m_RoughBtn->setCallback([&]
	{
		if (m_SelectedMaterial != nullptr)
		{
			m_bHasRough = !m_bHasRough;
			if (m_bHasRough) {
				m_SelectedMaterial->AddRough(GetTexture("Select Roughness Texture").c_str());
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
	materialSpec->setVisible(false);
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
	m_fReflect = selectedMaterial->GetMetalness();
	m_ReflectSlider->setValue(m_fReflect);
	m_TextboxReflect->setValue(std::to_string((int)(m_fReflect * 100)).c_str());
}

void Engine::UI::MaterialWindow::Clear()
{
	m_SelectedMaterial = nullptr;
}

std::string Engine::UI::MaterialWindow::GetTexture(std::string title)
{
	std::string texturePath = FileUtils::BrowseFiles("Select Texture");
	std::set<char> delims{ '\\', '/' };
	std::string fileName = FileUtils::splitpath(texturePath, delims).back();
	std::string newPath = m_EnginePointer->GetProject()->GetProjectDirectory() +"Assets/Textures/" + fileName;
	std::cout << newPath << std::endl;
	glfwRestoreWindow(m_EnginePointer->m_Window->getGLFWWindow());
	if (FileUtils::Exists(newPath))
	{
		return newPath;
	}
	else
	{
		//Copy file over
		FileUtils::TransferFile(texturePath, newPath);
		return newPath;
	}
}
