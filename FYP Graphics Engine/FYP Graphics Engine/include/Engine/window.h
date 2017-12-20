#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GLFW\glfw3.h>

namespace Engine { namespace graphics {

#define MAX_KEYS 1024
#define MAX_BUTTONS 32
	//! Window 
	/*!
	This class deals with displaying the base window as well as getting callbacks from mouse and keyboard inputs
	Contained in the Engine::graphics namespace.
	*/
	class Window
	{
	private:
		//! Private const char pointer.
		/*! Holds the memory allocation of the title for the window*/
		const char *m_Title;
		//! Private int variables
		/*! Holds width and height of the window */
		int m_Width, m_Height;
		//! Private GLFWwindow pointer.
		/*! Holds the memory allocation of window*/
		GLFWwindow * m_Window;

		//! Private int variable
		/*! Holds boolean value of wether or not the window is closed*/
		bool m_bClosed;

		//! Private bool array variable
		/*! Holds boolean values of wether or not diffrent keys are pressed*/
		bool m_Keys[MAX_KEYS];
		//! Private bool array variable
		/*! Holds boolean values of wether or not diffrent mouse buttons are pressed*/
		bool m_MouseButtons[MAX_BUTTONS];
		//! Private int variables
		/*! Holds x and y position of the cursor */
		double mx, my;
	public:
		//! Window Contructor
		/*!
		The contructor takes in the window title (the name displayed at the top of the window),
		window width and window height.
		\param title The name of the program/window
		\param width The Width of the window in pixels
		\param height The Height of the window in pixels
		*/
		Window(const char *title, int width, int height);
		//! Window Destructor.
		/*!
		Cleans up pointer memory in the class
		*/
		~Window();

		//! The Clear member function
		/*!
		Clears the window to a solid colour then clears the buffers
		*/
		void Clear() const;
		//! The Update member function
		/*!
		Checks for errors and prints them to the console.
		Poll the input events.
		Swap the buffers to view the updated window
		*/
		void Update();
		//! The Closed member function
		/*!
		Returns a boolean value of wether or not the window should close (e.g. the close button is clicked on)
		*/
		bool Closed() const;

		//! The getWidth member function
		/*!
		Returns m_Width.
		*/
		inline int getWidth() const { return m_Width; }
		//! The getHeight member function
		/*!
		Returns m_Height.
		*/
		inline int getHeight() const { return m_Height; }

		//! The isKeyPressed member function
		/*!
		Returns the boolean value of wether or not a key is pressed based on the keycode passed in.
		\param keyCode The unsigned integer that representants a key
		*/
		bool isKeyPressed(unsigned int keyCode) const;
		//! The isKeyPressed member function
		/*!
		Returns the boolean value of wether or not a mouse button is pressed based on the keycode passed in.
		\param button The unsigned integer that representants a button on the mouse
		*/
		bool isMouseButtonPressed(unsigned int button) const;
		//! The getMousePosition member function
		/*!
		Passes out the x and y positions of the cursor
		\param xPos a reference to a double that will be the X-Axis value of the cursor position
		\param yPos a reference to a double that will be the Y-Axis value of the cursor position
		*/
		void getMousePosition(double& xPos, double& yPos) const;

	private:
		//! The init member function
		/*!
		Sets up all the OpenGL backend and initilizes the GLFW and GLEW libraries while checking for errors.
		*/
		bool init();
		//! The key_callback static member function
		/*!
		Tells GLFW to give infomation about keys being pressed on a keyboard
		*/
		static friend void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		//! The mouse_button_callback static member function
		/*!
		Tells GLFW to give infomation about mouse buttons being pressed on a keyboard
		*/
		static friend void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
		//! The cursor_position_callback static member function
		/*!
		Tells GLFW to give infomation about mouse being moved and the cursors new position
		*/
		static friend void cursor_position_callback(GLFWwindow* window, double xPos, double yPos);

	};


	}
}