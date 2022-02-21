#include "Mesh.h"

namespace TestEngine {
	Mesh::Mesh(const std::string& shader_name, VertexBuffer&& VBO, IndexBuffer&& EBO, VertexArray&& VAO, RenderType renderType)
		:m_shaderName(shader_name), m_VBO(std::move(VBO)), m_EBO(std::move(EBO)), m_VAO(std::move(VAO)), m_renderType(renderType)
	{
	}
	
	unsigned int Mesh::get_renderType() const
	{
		return m_renderType;
	}

	const std::string& Mesh::get_shaderName() const
	{
		return m_shaderName;
	}

	const VertexArray& Mesh::get_VAO() const
	{
		return m_VAO;
	}
	const IndexBuffer& Mesh::get_EBO() const
	{
		return m_EBO;
	}
}