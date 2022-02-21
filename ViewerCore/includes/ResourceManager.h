#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include "UniformBuffer.h"
#include "VertexArray.h"
#include "ShaderProgram.h"
#include "Mesh.h"

namespace TestEngine {

	class ResourceManager
	{
	public: 
		static ResourceManager& Instance() {
			static ResourceManager manager;
			return manager;
		}
		enum class GLOBAL_UBO
		{
			GENERAL_MATRICES = 0,
			COMPUTE_INFO = 1
		};
		~ResourceManager() {};
		ResourceManager(const ResourceManager&) = delete;
		ResourceManager(ResourceManager&&) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;
		ResourceManager& operator=(ResourceManager&&) = delete;

		const UniformBuffer& getUBO(ResourceManager::GLOBAL_UBO UBOIndex) const;

		void addShaderProgram(const std::string& resourceName,
			const std::string& vertexShaderPath,
			const std::string& fragmentShaderPath);
		void addShaderProgram(const std::string& resourceName,
			const std::string& computeShaderPath);

		void addMesh(Mesh&&);
		std::vector<Mesh>& getMeshes();
		const ShaderProgram& getShaderProgram(const std::string& resourceName);
		
	private:
		ResourceManager();
		void init_values();

		std::unordered_map<std::string, ShaderProgram> m_mShaders;
		std::vector<Mesh> m_vMeshes;
		std::vector<UniformBuffer> m_vUBO;
	};
}
