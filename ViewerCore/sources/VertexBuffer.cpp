#include "VertexBuffer.h"
#include "LogManager.h"
#include "GL/glew.h"
namespace TestEngine {
	constexpr GLenum convert_usage_to_GLenum(const VertexBuffer::EUsage usage)
	{
		switch (usage)
		{
		case VertexBuffer::EUsage::Static: return GL_STATIC_DRAW;
		case VertexBuffer::EUsage::Dynamic: return GL_DYNAMIC_DRAW;
		case VertexBuffer::EUsage::Stream: return GL_STREAM_DRAW;
		}
		LogManager::Error() << "Unknown VertexBuffer usage\n" << LogManager::Show();
		return GL_STATIC_DRAW;
	}
	VertexBuffer::VertexBuffer(const void* data, const size_t size, BufferLayout buffer_layout, const EUsage usage)
		:m_buffer_layout(std::move(buffer_layout))
	{
		glCreateBuffers(1, &m_id);
		glNamedBufferData(m_id, size, data, convert_usage_to_GLenum(usage));
	}

	void VertexBuffer::updateData(const void* data, const size_t size, const EUsage usage)
	{
		glNamedBufferSubData(m_id, 0, size, data);
	}
	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_id);
	}

	VertexBuffer::VertexBuffer(VertexBuffer&& otherVertexBuffer) noexcept
		:m_buffer_layout(std::move(otherVertexBuffer.m_buffer_layout)),
		m_id(otherVertexBuffer.m_id)
	{
		otherVertexBuffer.m_id = 0;
	}

	VertexBuffer& VertexBuffer::operator=(VertexBuffer&& otherVertexBuffer) noexcept
	{
		m_buffer_layout = std::move(otherVertexBuffer.m_buffer_layout);
		m_id = otherVertexBuffer.m_id;
		otherVertexBuffer.m_id = 0;
		return *this;
	}

	void VertexBuffer::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
	}

	void VertexBuffer::unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

}