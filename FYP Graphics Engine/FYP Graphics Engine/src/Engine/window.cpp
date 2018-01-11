#include <Engine\window.h>
namespace Engine { namespace graphics {

	void window_resize(GLFWwindow *window, int width, int height);
	
	Window::Window(const char * title, int width, int height)
	{
		m_Title = title;
		m_Width = width;
		m_Height = height;
		if (!init())
		{
			glfwTerminate();
		}

		for (int i = 0; i < MAX_KEYS; i++)
		{
			m_Keys[i] = false;
		}

		for (int i = 0; i < MAX_BUTTONS; i++)
		{
			m_MouseButtons[i] = false;
		}

		//Init nanogui
		Screen();
		initialize(m_Window, false);

	}

	Window::~Window()
	{
		if(m_Window != NULL)
			delete m_Window;
	}

	void Window::Clear() const
	{
		//Set Background clear colour of window
		glClearColor(0.35f, 0.35f, 0.35f, 1);
		//Clear bit buffers
		glClear(GL_STENCIL_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Window::Update()
	{
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			std::cout << "OpenGL Enum Error: " << error << " " << glad_glGetString(error) << std::endl;
		}

		glfwPollEvents();
		glfwSwapBuffers(m_Window);

	}

	bool Window::Closed() const
	{
		return glfwWindowShouldClose(m_Window) == 1;
	}

	bool Window::isKeyPressed(unsigned int keyCode) const
	{
		if (keyCode >= MAX_KEYS)
		{
			std::cout << "Window.cpp, isKeyPressed: keyCode is out of range" << std::endl;
			return false;
		}
		return m_Keys[keyCode];
	}

	bool Window::isMouseButtonPressed(unsigned int button) const
	{
		if (button >= MAX_BUTTONS)
		{
			std::cout << "Window.cpp, isMouseButtonPressed: keyCode is out of range" << std::endl;
			return false;
		}
		return m_MouseButtons[button];
	}

	void Window::getMousePosition(double& xPos, double& yPos) const
	{
		xPos = mx;
		yPos = my;
	}

	void Window::drawContents()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ZERO);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LEQUAL);
	}

	bool Window::init()
	{
		if (!glfwInit())
		{
			std::cout << "Window.cpp: init: Failed to initialise GLFW" << std::endl;
			return false;
		}
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_SAMPLES, 16);
		m_Window = glfwCreateWindow(m_Width, m_Height, m_Title, NULL, NULL);
		if (!m_Window)
		{
			glfwTerminate();
			std::cout << "Window.cpp: init: Failed to create window" << std::endl;
			return false;
		}

		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, this);
		glfwSetWindowSizeCallback(m_Window, window_resize);
		glfwSetKeyCallback(m_Window, key_callback);
		glfwSetMouseButtonCallback(m_Window, mouse_button_callback);
		glfwSetCursorPosCallback(m_Window, cursor_position_callback);
		glfwSetCharCallback(m_Window, char_callback);
		glfwSetDropCallback(m_Window, drop_callback);
		glfwSetScrollCallback(m_Window, scroll_backback);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialise GLAD" << std::endl;
			return false;
		}

		glEnable(GL_MULTISAMPLE);
		

		

		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LEQUAL);

		glEnable(GL_CULL_FACE); // cull face
		glCullFace(GL_BACK);	// cull back face
		glFrontFace(GL_CCW);	// GL_CCW for counter clock-wise

		

		std::cout << "Calum McManus's CMEngine" << std::endl;
		std::cout << "Running OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
		return true;
	}

	void window_resize(GLFWwindow *window, int width, int height)
	{
		Window* win = (Window*)glfwGetWindowUserPointer(window);
		glViewport(0, 0, width, height);
		win->resizeCallbackEvent(width, height);
	}

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Window* win = (Window*)glfwGetWindowUserPointer(window);
		win->m_Keys[key] = action != GLFW_RELEASE;
		win->keyCallbackEvent(key, scancode, action, mods);
	}

	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
	{
		Window* win = (Window*)glfwGetWindowUserPointer(window);
		win->m_MouseButtons[button] = action != GLFW_RELEASE;
		win->mouseButtonCallbackEvent(button, action, mods);
	}
	void cursor_position_callback(GLFWwindow* window, double xPos, double yPos) 
	{
		Window* win = (Window*)glfwGetWindowUserPointer(window);
		win->mx = xPos;
		win->my = yPos;
		win->cursorPosCallbackEvent(xPos, yPos);
	}
	void char_callback(GLFWwindow * window, unsigned int codepoint)
	{
		Window* win = (Window*)glfwGetWindowUserPointer(window);
		win->charCallbackEvent(codepoint);
	}
	void drop_callback(GLFWwindow * window, int count, const char ** filenames)
	{
		Window* win = (Window*)glfwGetWindowUserPointer(window);
		win->dropCallbackEvent(count, filenames);
	}
	void scroll_backback(GLFWwindow * window, double x, double y)
	{
		Window* win = (Window*)glfwGetWindowUserPointer(window);
		win->scrollCallbackEvent(x, y);
	}
}
}