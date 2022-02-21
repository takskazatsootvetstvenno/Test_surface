#include "ResourceManager.h"
#include "glm/glm.hpp"
#include <memory>

namespace TestEngine {
	const UniformBuffer& ResourceManager::getUBO(ResourceManager::GLOBAL_UBO UBOIndex) const
	{
		return m_vUBO[static_cast<unsigned int>(UBOIndex)];
	}
	void ResourceManager::addShaderProgram(const std::string& resourceName, const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
	{
		m_mShaders.try_emplace(resourceName, vertexShaderPath, fragmentShaderPath);
	}

	void ResourceManager::addShaderProgram(const std::string& resourceName, const std::string& computeShaderPath)
	{
		m_mShaders.try_emplace(resourceName, computeShaderPath);
	}
	
	void ResourceManager::addMesh(Mesh&& new_mesh)
	{
		m_vMeshes.push_back(std::move(new_mesh));
	}
	const ShaderProgram& ResourceManager::getShaderProgram(const std::string& resourceName)
	{
		return m_mShaders.find(resourceName)->second;
	}
	std::vector<Mesh>& ResourceManager::getMeshes()
	{
		return m_vMeshes;
	}
	ResourceManager::ResourceManager()
	{
		init_values();
	}
	void ResourceManager::init_values()
	{
		addShaderProgram("Red", "Shaders/Red.vert", "Shaders/Red.frag");
		addShaderProgram("Axes", "Shaders/Axes.vert", "Shaders/Axes.frag");
		addShaderProgram("Compute_init", "Shaders/Init.comp");
		addShaderProgram("Compute", "Shaders/Compute.comp");
		std::vector<glm::vec3> buf(12);
		BufferLayout axes_BL
		{
			ShaderDataType::Float3,
			ShaderDataType::Float3
		};

		buf[0] = { 0.f, 0.f, 0.f }; //0
		buf[1] = { 1.f, 0.f, 0.f }; //red
		buf[2] = { 1.f, 0.f, 0.f }; //x1
		buf[3] = { 1.f, 0.f, 0.f }; //red

		buf[4] = { 0.f, 0.f, 0.f }; //0
		buf[5] = { 0.f, 1.f, 0.f }; //green
		buf[6] = { 0.f, 1.f, 0.f }; //y1
		buf[7] = { 0.f, 1.f, 0.f }; //green

		buf[8] = { 0.f, 0.f, 0.f }; //0
		buf[9] = { 0.f, 0.f, 1.f }; //blue
		buf[10] = { 0.f, 0.f, 1.f };//z1
		buf[11] = { 0.f, 0.f, 1.f };//blue
		std::vector<GLuint> axes_ind_buf{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
		
		VertexBuffer axes_VBO(buf.data(), buf.size() * sizeof(glm::vec3), axes_BL);
		IndexBuffer axes_EBO(axes_ind_buf.data(), axes_ind_buf.size());
		VertexArray axes_VAO;
		axes_VAO.add_vertex_buffer(axes_VBO);
		axes_VAO.set_index_buffer(axes_EBO);
		m_vMeshes.emplace_back(
			std::string("Axes"),
			std::move(axes_VBO),
			std::move(axes_EBO),
			std::move(axes_VAO),
			Mesh::LINES
		);

		BufferLayout buffer_layout_1mat4 //UBO
		{
			ShaderDataType::Mat4,
			ShaderDataType::Mat4,
			ShaderDataType::Mat4
		};
		BufferLayout buffer_layout_1int_2float //UBO
		{
			ShaderDataType::Int,
			ShaderDataType::Int,
			ShaderDataType::Float,
			ShaderDataType::Float
		};

		BufferLayout BL
		{
			ShaderDataType::Float3
		};

		auto m_VBO = VertexBuffer(nullptr, 0 * sizeof(glm::vec3), BL);
		auto m_EBO = IndexBuffer(nullptr, 0);
		auto m_VAO = VertexArray();

		m_vMeshes.emplace_back(
			std::string("Red"),
			std::move(m_VBO),
			std::move(m_EBO),
			std::move(m_VAO),
			Mesh::POINTS
			);

		BindingUBO uboBinding;
		uboBinding.programID = getShaderProgram("Red").getProgramID();
		uboBinding.uniformBlockIndex = getShaderProgram("Red").getUniformBlockIndex("GlobalMatrices");
		uboBinding.bindingPoint = 0;
		BindingUBO uboBindingCompute;
		uboBindingCompute.programID = getShaderProgram("Compute").getProgramID();
		uboBindingCompute.uniformBlockIndex = getShaderProgram("Compute").getUniformBlockIndex("SurfaceInfo");
		uboBindingCompute.bindingPoint = 1;
		m_vUBO.emplace_back(nullptr, buffer_layout_1mat4, uboBinding);
		m_vUBO.emplace_back(nullptr, buffer_layout_1int_2float, uboBindingCompute);
	}
}