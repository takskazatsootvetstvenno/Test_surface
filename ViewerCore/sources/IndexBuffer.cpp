#include <GL/glew.h>
#include "LogManager.h"
#include "IndexBuffer.h"
namespace TestEngine {
    constexpr GLenum convert_usage_to_GLenum(const IndexBuffer::EUsage usage)
    {
        switch (usage)
        {
        case IndexBuffer::EUsage::Static:  return GL_STATIC_DRAW;
        case IndexBuffer::EUsage::Dynamic: return GL_DYNAMIC_DRAW;
        case IndexBuffer::EUsage::Stream:  return GL_STREAM_DRAW;
        }

        LogManager::Error() << "Unknown VertexBuffer usage\n" << LogManager::Show();
        return GL_STREAM_DRAW;
    }
    IndexBuffer::~IndexBuffer()
    {
        glDeleteBuffers(1, &m_id);
    }
    IndexBuffer::IndexBuffer(const void* data, const size_t count, const IndexBuffer::EUsage usage)
        :m_count(count)
	{
		glCreateBuffers(1, &m_id);
		glNamedBufferData(m_id, count * sizeof(GLuint), data, convert_usage_to_GLenum(usage));
	}
    IndexBuffer::IndexBuffer(IndexBuffer&& otherIndexBuffer) noexcept
        :m_count(otherIndexBuffer.m_count),
        m_id(otherIndexBuffer.m_id)
    {
        otherIndexBuffer.m_count = 0;
        otherIndexBuffer.m_id = 0;
    }
    IndexBuffer& IndexBuffer::operator=(IndexBuffer&& otherIndexBuffer) noexcept
    {
        glDeleteBuffers(1, &m_id);
        m_id = otherIndexBuffer.m_id;
        m_count = otherIndexBuffer.m_count;
        otherIndexBuffer.m_id = 0;
        otherIndexBuffer.m_count = 0;
        return *this;
    }
    void IndexBuffer::bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    }
    void IndexBuffer::unbind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}
