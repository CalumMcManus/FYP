#include <Engine\UI\TransformWindow.h>

Engine::UI::TransformWindow::TransformWindow(Engine::GLFWEngine* enginePointer)
{
	//Transform Window
	m_TransformWindow = new nanogui::Window(enginePointer->m_Window, "Transform");
	m_TransformWindow->setPosition(Eigen::Vector2i(15, 55));
	m_TransformWindow->setLayout(new nanogui::GroupLayout());

	//Position
	m_TransformWindow->add<nanogui::Label>("Position", "sans-bold", 15);
	nanogui::Widget *pos = new nanogui::Widget(m_TransformWindow);
	pos->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Horizontal,
		nanogui::Alignment::Maximum, 0, 10));
	pos->setSize(nanogui::Vector2i(100, 20));
	pos->add<nanogui::Label>("X", "sans-bold", 15);
	posX = new nanogui::FloatBox<float>(pos, 0);
	posX->setCallback([&](float value) {
		if (m_SelectedTransform != nullptr)
		{
			m_SelectedTransform->setPosition(glm::vec3(value, m_SelectedTransform->getPosition().y, m_SelectedTransform->getPosition().z));
			std::cout << m_SelectedTransform->getPosition().x << " " << m_SelectedTransform->getPosition().y << " " << m_SelectedTransform->getPosition().z << std::endl;
		}
	});
	posX->setEditable(true);
	pos->add<nanogui::Label>("Y", "sans-bold", 15);
	posY = new nanogui::FloatBox<float>(pos, 0);
	posY->setCallback([&](float value) {
		if (m_SelectedTransform != nullptr)
		{
			m_SelectedTransform->setPosition(glm::vec3(m_SelectedTransform->getPosition().x, value, m_SelectedTransform->getPosition().z));
			std::cout << m_SelectedTransform->getPosition().x << " " << m_SelectedTransform->getPosition().y << " " << m_SelectedTransform->getPosition().z << std::endl;
		}
	});
	posY->setEditable(true);
	pos->add<nanogui::Label>("Z", "sans-bold", 15);
	posZ = new nanogui::FloatBox<float>(pos, 0);
	posZ->setCallback([&](float value) {
		if (m_SelectedTransform != nullptr)
		{
			m_SelectedTransform->setPosition(glm::vec3(m_SelectedTransform->getPosition().x, m_SelectedTransform->getPosition().y, value));
			std::cout << m_SelectedTransform->getPosition().x << " " << m_SelectedTransform->getPosition().y << " " << m_SelectedTransform->getPosition().z << std::endl;
		}
	});
	posZ->setEditable(true);

	//Rotation
	m_TransformWindow->add<nanogui::Label>("Rotation", "sans-bold", 15);
	nanogui::Widget *rot = new nanogui::Widget(m_TransformWindow);
	rot->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Horizontal,
		nanogui::Alignment::Maximum, 0, 10));
	rot->setSize(nanogui::Vector2i(100, 20));
	rot->add<nanogui::Label>("X", "sans-bold", 15);
	rotX = new nanogui::FloatBox<float>(rot, 0);
	rotX->setCallback([&](float value) {
		if (m_SelectedTransform != nullptr)
		{
			m_SelectedTransform->setRotation(glm::vec3(value, m_SelectedTransform->getRotation().y, m_SelectedTransform->getRotation().z));
			std::cout << m_SelectedTransform->getRotation().x << " " << m_SelectedTransform->getRotation().y << " " << m_SelectedTransform->getRotation().z << std::endl;
		}
	});
	rotX->setEditable(true);
	rot->add<nanogui::Label>("Y", "sans-bold", 15);
	rotY = new nanogui::FloatBox<float>(rot, 0);
	rotY->setCallback([&](float value) {
		if (m_SelectedTransform != nullptr)
		{
			m_SelectedTransform->setRotation(glm::vec3(m_SelectedTransform->getRotation().x, value, m_SelectedTransform->getRotation().z));
			std::cout << m_SelectedTransform->getRotation().x << " " << m_SelectedTransform->getRotation().y << " " << m_SelectedTransform->getRotation().z << std::endl;
		}
	});
	rotY->setEditable(true);
	rot->add<nanogui::Label>("Z", "sans-bold", 15);
	rotZ = new nanogui::FloatBox<float>(rot, 0);
	rotZ->setCallback([&](float value) {
		if (m_SelectedTransform != nullptr)
		{
			m_SelectedTransform->setRotation(glm::vec3(m_SelectedTransform->getRotation().x, m_SelectedTransform->getRotation().y, value));
			std::cout << m_SelectedTransform->getRotation().x << " " << m_SelectedTransform->getRotation().y << " " << m_SelectedTransform->getRotation().z << std::endl;
		}
	});
	rotZ->setEditable(true);

	//Scale
	m_TransformWindow->add<nanogui::Label>("Scale", "sans-bold", 15);
	nanogui::Widget *scale = new nanogui::Widget(m_TransformWindow);
	scale->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Horizontal,
		nanogui::Alignment::Maximum, 0, 10));
	scale->setSize(nanogui::Vector2i(100, 20));
	scale->add<nanogui::Label>("X", "sans-bold", 15);
	scaleX = new nanogui::FloatBox<float>(scale, 0.f);
	scaleX->setCallback([&](float value) {
		if (m_SelectedTransform != nullptr)
		{
			m_SelectedTransform->setSize(glm::vec3(value, m_SelectedTransform->getSize().y, m_SelectedTransform->getSize().z));
			std::cout <<  "New Scale: " << m_SelectedTransform->getSize().x << " " << m_SelectedTransform->getSize().y << " " << m_SelectedTransform->getSize().z << std::endl;
		}
	});
	scaleX->setEditable(true);
	scale->add<nanogui::Label>("Y", "sans-bold", 15);
	scaleY = new nanogui::FloatBox<float>(scale, 0.f);
	scaleY->setCallback([&](float value) {
		if (m_SelectedTransform != nullptr)
		{
			m_SelectedTransform->setSize(glm::vec3(m_SelectedTransform->getSize().x, value, m_SelectedTransform->getSize().z));
			std::cout << m_SelectedTransform->getSize().x << " " << m_SelectedTransform->getSize().y << " " << m_SelectedTransform->getSize().z << std::endl;
		}
	});
	scaleY->setEditable(true);
	scale->add<nanogui::Label>("Z", "sans-bold", 15);
	scaleZ = new nanogui::FloatBox<float>(scale, 0.f);
	scaleZ->setCallback([&](float value) {
		if (m_SelectedTransform != nullptr)
		{
			m_SelectedTransform->setSize(glm::vec3(m_SelectedTransform->getSize().x, m_SelectedTransform->getSize().y, value));
			std::cout << m_SelectedTransform->getSize().x << " " << m_SelectedTransform->getSize().y << " " << m_SelectedTransform->getSize().z << std::endl;
		}
	});
	scaleZ->setEditable(true);
}

void Engine::UI::TransformWindow::SelectTransform(Components::Transform * selectedTransform)
{
	m_SelectedTransform = selectedTransform;
	posX->setValue(m_SelectedTransform->getPosition().x);
	posY->setValue(m_SelectedTransform->getPosition().y);
	posZ->setValue(m_SelectedTransform->getPosition().z);

	rotX->setValue(m_SelectedTransform->getRotation().x);
	rotY->setValue(m_SelectedTransform->getRotation().y);
	rotZ->setValue(m_SelectedTransform->getRotation().z);

	scaleX->setValue(m_SelectedTransform->getSize().x);
	scaleY->setValue(m_SelectedTransform->getSize().y);
	scaleZ->setValue(m_SelectedTransform->getSize().z);
}
