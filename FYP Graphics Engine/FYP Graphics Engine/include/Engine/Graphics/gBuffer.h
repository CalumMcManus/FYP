#pragma once
#include <Engine\Graphics\frameBuffer.h>

namespace Engine { namespace graphics {
	//! GBuffer 
	/*!
	GBuffer used for storing infomation textures for defered rendering
	Inherits from FrameBuffer.
	Contained in the Engine::graphics namespace.
	*/
	class GBuffer : public FrameBuffer
	{
	public:
		//! SSAOFilter Contructor
		/*!
		Overrides Framebuffer contructor.
		Sets up a multi-component frame buffer that contains screen space textures of-
		Colour, Normals, Position, Depth and Components (Metalness/Roughness) for PBR
		\param window Window pointer
		\param samples Number of Anti-Ailasing Samples
		*/
		GBuffer(Window* window, int samples, Shader* shader);
		//! GBuffer Destructor.
		/*!
		Cleans up memory in the class
		*/
		~GBuffer();

		//! The GetNormal member function
		/*!
		Returns Normal Texture ID
		*/
		GLuint GetNormal() { return m_NormalTexture; }
		//! The GetPos member function
		/*!
		Returns Position Texture ID
		*/
		GLuint GetPos() { return m_PositionTexture; }
		//! The GetDepth member function
		/*!
		Returns Depth Texture ID
		*/
		GLuint GetDepth() { return m_DepthTexture; }
		//! The GetAdditional member function
		/*!
		Returns Additional Component Texture ID
		*/
		GLuint GetAdditional() { return m_AdditioalComp; }

		//! Private GLuint variable
		/*! Depth Texture ID */
		GLuint m_DepthTexture;
		//! Private GLuint variable
		/*! Additional Components Texture ID */
		GLuint m_AdditioalComp;

		//! The Render member function
		/*!
		Renders textures to screen space quad
		*/
		void Render();
	private:
		//! The createBuffer member function
		/*!
		Creates a single sample texture buffer
		\param texUnit Active Texture ID
		\param format Colour Format Enum (e.g. RGBA)
		\param texid Texture ID
		*/
		void createBuffer(GLenum texUnit, GLenum format, GLuint& texid);
		//! The createBufferMultisample member function
		/*!
		Creates a multis sample texture buffer
		\param texUnit Active Texture ID
		\param format Colour Format Enum (e.g. RGBA)
		\param texid Texture ID
		*/
		void createBufferMultisample(GLenum texUnit, GLenum format, GLuint& texid);
		//! Private Window pointer.
		/*! Pointer to the Window class */
		Window* windowPointer;


	};

}	}