#pragma once
#include<string>
namespace TestEngine
{
	class ShaderProgram
	{
	public:
		ShaderProgram(const std::string& vertex_shader_path, const std::string& fragment_shader_path);
		ShaderProgram(const std::string& compute_shader_path);
		ShaderProgram(ShaderProgram&&) noexcept;
		ShaderProgram& operator=(ShaderProgram&&) noexcept;
		~ShaderProgram();

		ShaderProgram() = delete;
		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;

		void bind() const;
		static void unbind();
		bool isCompiled() const;
		unsigned int getProgramID() const;
		unsigned int getUniformBlockIndex(const char* name) const;
	private:
		bool m_isCompiled = false;
		unsigned int m_id = 0;
	};
}

