#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

namespace TestEngine {
	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		VertexArray(const VertexArray&) = delete;
		VertexArray& operator=(const VertexArray&) = delete;
		VertexArray(VertexArray&& otherVertexArray) noexcept;
		VertexArray& operator=(VertexArray&& otherVertexArray) noexcept;

		void add_vertex_buffer(const VertexBuffer& vertex_buffer);
		void set_index_buffer(const IndexBuffer& index_buffer);
		void bind() const;
		void unbind() const;
		size_t get_indices_count() const {return m_indices_count;}
	private:
		unsigned int m_id = 0;
		unsigned int m_elements_count = 0;
		size_t m_indices_count = 0;
	};
}
