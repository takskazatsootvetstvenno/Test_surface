#include <GL/glew.h>
#include "ShaderProgram.h"
#include <fstream>
#include "LogManager.h"
namespace TestEngine {
	bool create_shader(const std::string& shader_path, const GLenum shader_type, GLuint& shader_id)
	{
		shader_id = glCreateShader(shader_type);
		std::ifstream inFile(shader_path);
		std::string source((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
		if (source.empty())
			LogManager::Error() << "Can't load shader! Path: " << shader_path << "\n" << LogManager::Show();
		const char* src = source.c_str();
		glShaderSource(shader_id, 1, &src, nullptr);
		glCompileShader(shader_id);

		inFile.close();

		GLint success;
		glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE) {
			char info_log[1024];
			glGetShaderInfoLog(shader_id, 1024, nullptr, info_log);

			LogManager::Error() << "Shader compilation error: " << info_log << "\n" << LogManager::Show();
			return false;
		}
		return true;
	}
	ShaderProgram::ShaderProgram(const std::string& vertex_shader_path, const std::string& fragment_shader_path)
	{
		GLuint vertex_shader_id = 0;
		if (!create_shader(vertex_shader_path, GL_VERTEX_SHADER, vertex_shader_id)) {
			LogManager::Error() << "VERTEX SHADER: compile-time error!\n" << LogManager::Show();;
			glDeleteShader(vertex_shader_id);
			return;
		}

		GLuint fragment_shader_id = 0;
		if (!create_shader(fragment_shader_path, GL_FRAGMENT_SHADER, fragment_shader_id)) {
			LogManager::Error() << "FRAGMENT SHADER: compile-time error!\n" << LogManager::Show();;
			glDeleteShader(vertex_shader_id);
			glDeleteShader(fragment_shader_id);
			return;
		}

		m_id = glCreateProgram();
		glAttachShader(m_id, vertex_shader_id);
		glAttachShader(m_id, fragment_shader_id);
		glLinkProgram(m_id);

		GLint success;
		glGetProgramiv(m_id, GL_LINK_STATUS, &success);
		if (success == GL_FALSE) {
			GLchar info_log[1024];
			glGetProgramInfoLog(m_id, 1024, nullptr, info_log);
			LogManager::Error() << "SHADER PROGRAM: Link-time error: " << info_log << "\n" << LogManager::Show();
			glDeleteProgram(m_id);
			m_id = 0;
			glDeleteShader(vertex_shader_id);
			glDeleteShader(fragment_shader_id);
			return;
		}

		m_isCompiled = true;
		glDetachShader(m_id, vertex_shader_id);
		glDetachShader(m_id, fragment_shader_id);
		glDeleteShader(vertex_shader_id);
		glDeleteShader(fragment_shader_id);
	}

	ShaderProgram::ShaderProgram(const std::string& compute_shader_path)
	{
		GLuint compute_shader_id = 0;
		if (!create_shader(compute_shader_path, GL_COMPUTE_SHADER, compute_shader_id)) {
			LogManager::Error() << "COMPUTE SHADER: compile-time error!\n" << LogManager::Show();
			glDeleteShader(compute_shader_id);
			return;
		}

		m_id = glCreateProgram();
		glAttachShader(m_id, compute_shader_id);
		glLinkProgram(m_id);

		GLint success;
		glGetProgramiv(m_id, GL_LINK_STATUS, &success);
		if (success == GL_FALSE) {
			GLchar info_log[1024];
			glGetProgramInfoLog(m_id, 1024, nullptr, info_log);
			LogManager::Error() << "SHADER PROGRAM: Link-time error: " << info_log << "\n" << LogManager::Show();
			glDeleteProgram(m_id);
			glDeleteShader(compute_shader_id);
			m_id = 0;
			return;
		}
		m_isCompiled = true;
		glDetachShader(m_id, compute_shader_id);
		glDeleteShader(compute_shader_id);
	}

	ShaderProgram::ShaderProgram(ShaderProgram&& OtherProgram) noexcept
		:m_id(OtherProgram.m_id),
		m_isCompiled(OtherProgram.m_isCompiled)
	{
		OtherProgram.m_id = 0;
		OtherProgram.m_isCompiled = false;
	}
	ShaderProgram& ShaderProgram::operator=(ShaderProgram&& otherProgram) noexcept
	{
		glDeleteProgram(m_id);
		m_id = otherProgram.m_id;
		m_isCompiled = otherProgram.m_isCompiled;
		otherProgram.m_id = 0;
		otherProgram.m_isCompiled = false;

		return *this;
	}
	ShaderProgram::~ShaderProgram()
	{
		glDeleteProgram(m_id);
	}
	void ShaderProgram::bind() const noexcept
	{
		glUseProgram(m_id);
	}
	void ShaderProgram::unbind()
	{
		glUseProgram(0);
	}
	bool ShaderProgram::isCompiled() const
	{
		return m_isCompiled;
	}
	unsigned int ShaderProgram::getProgramID() const noexcept
	{
		return m_id;
	}
	unsigned int ShaderProgram::getUniformBlockIndex(const char* name) const noexcept
	{
		return glGetUniformBlockIndex(m_id, name);
	}
}
