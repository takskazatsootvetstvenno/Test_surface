#pragma once

#include "Surface.h"
#include "BufferLayout.h"
#include <vector>
#include <memory>
#include "ResourceManager.h"
/*#include <thread>
#include <chrono>*/
#include <cmath>
namespace TestEngine {
	Surface::Surface(const int N, Section sec)
		:m_N(N), m_section(sec)
	{
		glCreateBuffers(1, &m_SSBO_init);
		glCreateBuffers(1, &m_SSBO);
		m_InitSSBO_data.resize(m_max_N * m_max_N * m_max_N);
		m_SSBO_data.resize(m_max_N * m_max_N * m_max_N);

		ResourceManager::Instance().getShaderProgram("Compute_init").bind();
		glNamedBufferData(m_SSBO_init,
			m_InitSSBO_data.size() * sizeof(float),
			m_InitSSBO_data.data(),
			GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_SSBO_init);

		ResourceManager::Instance().getShaderProgram("Compute").bind();

		glNamedBufferData(m_SSBO,
			m_SSBO_data.size() * sizeof(glm::vec4),
			m_SSBO_data.data(),
			GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_SSBO);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		int32_t type = 0;	//TO DO, make section on compute shader
		auto& UBO = ResourceManager::Instance().getUBO(ResourceManager::GLOBAL_UBO::COMPUTE_INFO);
		SurfaceSSBOData new_data{ type, m_max_N, 0.f, 0.f };
		UBO.updateData(&new_data);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_UNIFORM_BARRIER_BIT);
		updateSSBO();
	}

	Surface::~Surface()
	{
		glDeleteBuffers(1, &m_SSBO_init);
		glDeleteBuffers(1, &m_SSBO);
	}

	void Surface::updateSurfaceVB(uint32_t N, const float epsilon, const float constant) noexcept
	{
		int32_t type = 0;	//TO DO, make section on compute shader
		auto& UBO = ResourceManager::Instance().getUBO(ResourceManager::GLOBAL_UBO::COMPUTE_INFO);
		SurfaceSSBOData new_data{ type, N, constant, epsilon };
		UBO.updateData(&new_data);
		if (N != m_N)
		{
			m_N = N;
			updateSSBO();
		}
		m_N = N;		
		glMemoryBarrier(GL_ALL_BARRIER_BITS);
		ResourceManager::Instance().getShaderProgram("Compute").bind();
		glDispatchCompute(
			(GLuint)std::ceil(m_N),
			(GLuint)std::ceil(m_N),
			(GLuint)std::ceil(m_N));
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	}

	Mesh& Surface::getMesh() const noexcept
	{
		return *m_mesh;
	}

	uint32_t Surface::count() const noexcept
	{
		return m_N * m_N * m_N;
	}

	void Surface::updateSSBO()
	{
		ResourceManager::Instance().getShaderProgram("Compute_init").bind();
		glDispatchCompute(
			(GLuint)std::ceil(m_N),
			(GLuint)std::ceil(m_N),
			(GLuint)std::ceil(m_N));
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	}
}
