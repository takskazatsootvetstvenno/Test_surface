#pragma once

#include "Surface.h"
#include "BufferLayout.h"
#include "ResourceManager.h"
#include <vector>
#include <memory>
#include <cmath>

namespace TestEngine {
	Surface::Surface(const int N)
		:m_N(N)
	{
		glCreateBuffers(1, &m_SSBO_init);
		glCreateBuffers(1, &m_SSBO_norm);
		glCreateBuffers(1, &m_SSBO);

		glNamedBufferData(m_SSBO_init,
			(m_max_N * m_max_N * m_max_N) * sizeof(float),
			nullptr,
			GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_SSBO_init);

		glNamedBufferData(m_SSBO,
			(12 * 3 * m_max_N * m_max_N * m_max_N) * sizeof(glm::uvec4),
			nullptr,
			GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_SSBO);

		glNamedBufferData(m_SSBO_norm,
			(12 * 3 * m_max_N * m_max_N * m_max_N) * sizeof(glm::uvec4),
			nullptr,
			GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_SSBO_norm);

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
		glDeleteBuffers(1, &m_SSBO_norm);
		glDeleteBuffers(1, &m_SSBO);
	}

	void Surface::updateSurfaceVB(const uint32_t N, const float constant) noexcept
	{
		int32_t type = 0;	//TO DO, make section on compute shader
		auto& UBO = ResourceManager::Instance().getUBO(ResourceManager::GLOBAL_UBO::COMPUTE_INFO);
		SurfaceSSBOData new_data{ type, N, constant, 0.f };
		UBO.updateData(&new_data);
		if (N != m_N)
		{
			m_N = N;
			updateSSBO();
		}
		m_N = N;		
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
		return 12 * 3 * m_N * m_N * m_N;
	}

	void Surface::updateSSBO() const noexcept
	{
		ResourceManager::Instance().getShaderProgram("Compute_init").bind();
		glDispatchCompute(
			(GLuint)std::ceil(m_N),
			(GLuint)std::ceil(m_N),
			(GLuint)std::ceil(m_N));
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	}
}
