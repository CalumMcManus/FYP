#pragma once
#include <glad\glad.h>
#include <Engine\window.h>
#include <Engine\Graphics\shader.h>
#include <iostream>
namespace Engine { namespace graphics {
	//! FrameBuffer 
	/*!
	Creates and stores screen space buffer textures
	Contained in the Engine::graphics namespace.
	*/
	class FrameBuffer
	{
	public:
		//! FrameBuffer Contructor
		/*!
		Empty contructor
		*/
		FrameBuffer() {};
		//! FrameBuffer Contructor
		/*!
		Creates screen space buffer texture
		\param window Window pointer
		\param shader Shader pointer
		*/
		FrameBuffer(Window* window, Shader* shader);
		//! FrameBuffer Destructor.
		/*!
		Cleans up memory in the class
		*/
		~FrameBuffer();
		//! The Bind member function
		/*!
		Binds frame buffer
		*/
		void Bind();
		//! The Unbind member function
		/*!
		Unbinds frame buffer
		*/
		void Unbind();
		//! The Render member function
		/*!
		Draws buffer texture to screen
		*/
		void Render();
		//! The GetBufferID member function
		/*!
		Returns frame buffer ID
		*/
		GLuint GetBufferID() { return m_FrameBuffer; };
		//! The GetTextureID member function
		/*!
		Returns Texture ID
		*/
		GLuint GetTextureID() { return m_Texture; };
		//! The GetShader member function
		/*!
		Returns shader pointer
		*/
		Shader* GetShader() { return m_ScreenShader; };
	protected:
		//! The GetShader member function
		/*!
		Cleans up opengl data
		*/
		void CleanUp();
		//! Private GLuint variable.
		/*! Vertex Array Object*/
		GLuint m_QuadVAO;
		//! Private GLuint variable.
		/*! Vertex Buffer Object */
		GLuint m_QuadVBO;
		//! Private Float Array.
		/*! Array of floats representing vertex positions */
		float points[24] = {
			-1.0f,  1.0f,  0.0f, 1.0f,
			1.0f,  1.0f,  1.0f, 1.0f,
			1.0f, -1.0f,  1.0f, 0.0f,

			1.0f, -1.0f,  1.0f, 0.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			-1.0f,  1.0f,  0.0f, 1.0f
		};
		//! Private GLuint variable.
		/*! Buffer ID */
		GLuint m_FrameBuffer;
		//! Private GLuint variable.
		/*! Texture Buffer ID */
		GLuint m_TexColorBuffer;
		//! Private GLuint variable.
		/*! Render Buffer Objet (Depth) */
		GLuint m_RBODepthBuffer;

		//! Private GLuint variable.
		/*! Colour Texture ID */
		GLuint m_Texture;
		//! Private GLuint variable.
		/*! Normal Texture ID */
		GLuint m_NormalTexture;
		//! Private GLuint variable.
		/*! Position Texture ID */
		GLuint m_PositionTexture;

		//! Private int variable.
		/*! AA Samples */
		int m_Samples;
		//! Private Shader Pointer.
		/*! Pointer to screen space shader */
		Shader* m_ScreenShader;

	};


} }