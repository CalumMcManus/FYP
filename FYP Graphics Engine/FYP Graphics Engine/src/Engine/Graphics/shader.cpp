#include <Engine/Graphics/shader.h>

namespace Engine {
	namespace graphics {

		Shader::Shader(const char * vertPath, const char * fragPath)
			: m_VertPath(vertPath), m_FragPath(fragPath)
		{
			m_ShaderID = load();
			GLint isLinked = 0;
			glGetProgramiv(m_ShaderID, GL_LINK_STATUS, &isLinked);
			if (isLinked == GL_FALSE)
			{
				std::cout << "Shader: Contructor: Failed to link program." << std::endl;
			}
		}

		Shader::~Shader()
		{
			glDeleteProgram(m_ShaderID);
		}

		void Shader::setUniform1f(const GLchar * name, float value)
		{
			glUniform1f(getUniformLocation(name), value);
		}

		void Shader::setUniform1i(const GLchar * name, int value)
		{
			glUniform1i(getUniformLocation(name), value);
		}

		void Shader::setUniform2f(const GLchar * name, glm::vec2 & vector)
		{
			glUniform2f(getUniformLocation(name), vector.x, vector.y);
		}

		void Shader::setUniform3f(const GLchar * name, glm::vec3 & vector)
		{
			glUniform3f(getUniformLocation(name), vector.x, vector.y, vector.z);
		}

		void Shader::setUniform4f(const GLchar * name, glm::vec4 & vector)
		{
			glUniform4f(getUniformLocation(name), vector.x, vector.y, vector.z, vector.w);
		}

		void Shader::setUniformMat4(const GLchar * name, const glm::mat4 & matrix)
		{
			glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
		}

		void Shader::setUniformMat3(const GLchar * name, const glm::mat3 & matrix)
		{
			glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
		}

		void Shader::enable() const
		{
			glUseProgram(m_ShaderID);
		}

		void Shader::disable() const
		{
			glUseProgram(0);
		}

		GLuint Shader::load()
		{
			GLuint program = glCreateProgram();
			GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
			GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);

			std::string vertSourceString = FileUtils::read_file(m_VertPath);
			std::string fragSourceString = FileUtils::read_file(m_FragPath);

			const char* vertSource = vertSourceString.c_str();
			const char* fragSource = fragSourceString.c_str();

			glShaderSource(vertex, 1, &vertSource, NULL);
			glCompileShader(vertex);

			//Handle failed loading vertex
			GLint result;
			glGetShaderiv(vertex, GL_COMPILE_STATUS, &result);

			if (result == GL_FALSE)
			{
				GLint length;
				glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &length);
				std::vector<char> error(length);
				glGetShaderInfoLog(vertex, length, &length, &error[0]);
				std::cout << "Shader.cpp: load: Failed to load vertex shader: " << &error[0] << std::endl;
				glDeleteShader(vertex);
				return 0;
			}

			glShaderSource(fragment, 1, &fragSource, NULL);
			glCompileShader(fragment);

			//Handle failed loading fragment
			result = NULL;
			glGetShaderiv(fragment, GL_COMPILE_STATUS, &result);

			if (result == GL_FALSE)
			{
				GLint length;
				glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &length);
				std::vector<char> error(length);
				glGetShaderInfoLog(fragment, length, &length, &error[0]);
				std::cout << "Shader.cpp: load: Failed to load fragment shader: " << &error[0] << std::endl;
				glDeleteShader(fragment);
				return 0;
			}

			glAttachShader(program, vertex);
			glAttachShader(program, fragment);

			glLinkProgram(program);
			glValidateProgram(program);

			//Clean up
			glDeleteShader(vertex);
			glDeleteShader(fragment);

			return program;
		}

		GLint Shader::getUniformLocation(const GLchar * name)
		{
			return glGetUniformLocation(m_ShaderID, name);
		}

	}
}
