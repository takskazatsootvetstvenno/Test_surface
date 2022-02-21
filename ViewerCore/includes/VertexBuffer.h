#pragma once
#include "BufferLayout.h"
namespace TestEngine {
	class VertexBuffer {
	public:
		enum class EUsage {Static, Dynamic, Stream};

		VertexBuffer(const void* data, const size_t size, BufferLayout buffer_layout,
			const EUsage usage = VertexBuffer::EUsage::Static);
		~VertexBuffer();

		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer& operator=(const VertexBuffer&) = delete;
		VertexBuffer(VertexBuffer&& otherVertexBuffer) noexcept;
		VertexBuffer& operator=(VertexBuffer&& otherVertexBuffer) noexcept;

		unsigned int get_buffer() const { return m_id; }
		
		static void unbind();
		void bind() const;
		void updateData(const void* data, const size_t size,
			const EUsage usage = VertexBuffer::EUsage::Static);
		const BufferLayout& get_layout() const { return m_buffer_layout; }
	private:
		unsigned int m_id = 0;
		BufferLayout m_buffer_layout;
	};
}