#pragma once
#include <string>
#include <GL/glew.h>
#include "IndexBuffer.h"
#include "LogManager.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
namespace TestEngine {
	class Mesh
	{
	public:
		enum RenderType
		{
			TRIANGLE = GL_TRIANGLES,
			POINTS = GL_POINTS,
			LINES = GL_LINES
		};
		Mesh(const std::string& shader_name, VertexBuffer&& VBO, IndexBuffer&& EBO, VertexArray&& VAO, RenderType RT = RenderType::TRIANGLE);
		Mesh(Mesh&&) = default;
		~Mesh() {};
		unsigned int get_renderType() const noexcept;
		const std::string& get_shaderName() const noexcept;
		const VertexArray& get_VAO() const noexcept;
		const IndexBuffer& get_EBO() const noexcept;
	private:
		std::string m_shaderName;
		VertexBuffer m_VBO;
		VertexArray m_VAO;
		IndexBuffer m_EBO;
		RenderType m_renderType;
	};
}
