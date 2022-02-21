#include <GL/glew.h>
#include "VertexArray.h"
namespace TestEngine {
	VertexArray::VertexArray()
	{
		glCreateVertexArrays(1, &m_id);
	}
	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_id);
	}
	VertexArray::VertexArray(VertexArray&& otherVertexArray) noexcept
		:m_id(otherVertexArray.m_id),
		m_elements_count(otherVertexArray.m_elements_count),
		m_indices_count(otherVertexArray.m_indices_count)
	{
		otherVertexArray.m_id = 0;
		otherVertexArray.m_elements_count = 0;
		otherVertexArray.m_indices_count = 0;
	}
	VertexArray& VertexArray::operator=(VertexArray&& otherVertexArray) noexcept
	{
		glDeleteVertexArrays(1, &m_id);
		m_id = otherVertexArray.m_id;
		m_elements_count = otherVertexArray.m_elements_count;
		m_indices_count = otherVertexArray.m_indices_count;
		otherVertexArray.m_id = 0;
		otherVertexArray.m_elements_count = 0;
		otherVertexArray.m_indices_count = 0;
		return *this;
	}
	void VertexArray::bind() const {
		glBindVertexArray(m_id);
	}
	/* static */ void VertexArray::unbind() const {
		glBindVertexArray(0);
	}
	void VertexArray::add_vertex_buffer(const VertexBuffer& vertex_buffer)
	{
		glVertexArrayVertexBuffer(m_id, 0, vertex_buffer.get_buffer(), 0, static_cast<GLsizei>(vertex_buffer.get_layout().get_stride()));

		for (const BufferElement& current_element : vertex_buffer.get_layout().get_elements())
		{
			glEnableVertexArrayAttrib(m_id, m_elements_count);
			glVertexArrayAttribFormat(
				m_id,
				m_elements_count,
				static_cast<GLint>(current_element.components_count),
				current_element.component_type,
				GL_FALSE,
				static_cast<GLuint>(current_element.offset)
				);
			glVertexArrayAttribBinding(m_id, m_elements_count, 0);

			++m_elements_count;
		}
	}
	void VertexArray::set_index_buffer(const IndexBuffer& index_buffer)
	{
		glVertexArrayElementBuffer(m_id, index_buffer.get_buffer());
		m_indices_count = index_buffer.get_count();
	}
}
