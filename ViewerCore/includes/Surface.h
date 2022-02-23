#pragma once
#include <string>
#include <memory>
#include <functional>
#include "Event.h"
#include "Mesh.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include <glm/glm.hpp>
namespace TestEngine {
	struct SurfaceData
	{
		float old_epsilon = 0.006f;
		float new_epsilon = 0.006f;
		float old_const = 0.9f;
		float new_const = 0.9f;
		int old_grid_size = 60;
		int new_grid_size = 60;
		int old_section = 0;
		int new_section = 3;
		bool new_useWireFrames = false;
		bool old_useWireFrames = false;
		bool isChanged() const noexcept {
			return (abs(new_epsilon - old_epsilon) > 0.000001
				|| new_section != old_section
				|| old_grid_size != new_grid_size
				|| abs(new_const - old_const) > 0.000001)
				|| new_useWireFrames != old_useWireFrames;
		}
		void update() noexcept {
			old_epsilon = new_epsilon;
			old_grid_size = new_grid_size;
			old_const = new_const;
			old_section = new_section;
		}
	};
	class Surface
	{
	public:
		enum class Section{
			X_sec,
			Y_sec,
			Z_sec,
			Nothing
		};
		struct SurfaceSSBOData
		{
			alignas(4) int32_t type;
			alignas(4) uint32_t N;
			alignas(4) float constanta;
			alignas(4) float eps;
		};
		Surface(const int N, Section lambda);
		~Surface();
		Surface(const Surface& app) = delete;
		Surface(Surface&&) = delete;
		Surface& operator=(const Surface) = delete;
		Surface& operator=(Surface&&) = delete;

		void updateSurfaceVB(uint32_t N, const float epsilon, const float constant) noexcept;
		Mesh& getMesh() const noexcept;
		uint32_t count() const noexcept;
	private:
		const uint32_t m_max_N = 100;
		uint32_t m_N = 100;
		Section m_section = Section::Nothing;
		GLuint m_SSBO_init;
		GLuint m_SSBO_norm;
		GLuint m_SSBO;

		void updateSSBO();
		std::unique_ptr<Mesh> m_mesh;
		std::unique_ptr<VertexBuffer> m_VBO;
		std::unique_ptr<IndexBuffer> m_EBO;
		std::unique_ptr<VertexArray> m_VAO;
	};
}
