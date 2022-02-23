#pragma once
#include "BufferLayout.h"
namespace TestEngine {

	struct BindingUBO
	{
		unsigned int programID = 0;
		unsigned int uniformBlockIndex = 0;
		unsigned int bindingPoint = 0;
	};
	class UniformBuffer
	{
	public:
		enum class EUsage { Static, Dynamic, Stream };
		~UniformBuffer();
		UniformBuffer(const void* data, BufferLayout buffer_layout, const BindingUBO& bindUBO, const UniformBuffer::EUsage usage = UniformBuffer::EUsage::Static);
		UniformBuffer(const UniformBuffer&) = delete;
		UniformBuffer& operator=(const UniformBuffer&) = delete;
		UniformBuffer(UniformBuffer&& otherUniformBuffer) noexcept;
		UniformBuffer& operator=(UniformBuffer&& otherUniformBuffer) noexcept;

		void updateData(const void* data) const noexcept;
		void updateSubData(const void* data, const unsigned int offset, const unsigned int size) const noexcept;
		void updateElementData(const void* data, unsigned int elementID) const noexcept;
		void bind() const;
		static void unBind();
		unsigned int get_buffer() const { return m_id; }
	private:
		unsigned int m_id = 0;
		BufferLayout m_buffer_layout;
		BindingUBO m_bindings;
	};
}
