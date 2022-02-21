#include "GL/glew.h"
#include "UniformBuffer.h"
#include "LogManager.h"
namespace TestEngine {
	constexpr GLenum convert_usage_to_GLenum(const UniformBuffer::EUsage usage)
	{
		switch (usage)
		{
			case UniformBuffer::EUsage::Static: return GL_STATIC_DRAW;
			case UniformBuffer::EUsage::Dynamic: return GL_DYNAMIC_DRAW;
			case UniformBuffer::EUsage::Stream: return GL_STREAM_DRAW;
		}
		LogManager::Error() << "Unknown UniformBuffer usage\n" << LogManager::Show();
		return GL_STATIC_DRAW;
	}
	UniformBuffer::~UniformBuffer()
	{
		glDeleteBuffers(1, &m_id);
	}
	UniformBuffer::UniformBuffer(const void* data, BufferLayout buffer_layout, const BindingUBO& bindUBO, const UniformBuffer::EUsage usage)
		:m_buffer_layout(std::move(buffer_layout)),
		m_bindings(bindUBO)
	{
		glCreateBuffers(1, &m_id);
		glNamedBufferData(m_id, buffer_layout.get_stride(), data, convert_usage_to_GLenum(usage));
		glUniformBlockBinding(bindUBO.programID, bindUBO.uniformBlockIndex, bindUBO.bindingPoint);
		glBindBufferRange(GL_UNIFORM_BUFFER, bindUBO.bindingPoint,
			m_id, 0, buffer_layout.get_stride());
	}
	void UniformBuffer::updateData(const void* data) const
	{
		glNamedBufferSubData(m_id, 0, m_buffer_layout.get_stride(), data);
	}
	void UniformBuffer::updateSubData(const void* data, const unsigned int offset, const unsigned int size) const
	{
		glNamedBufferSubData(m_id, offset, size, data);
	}
	void UniformBuffer::updateElementData(const void* data, unsigned int elementID) const
	{
		auto& element = m_buffer_layout.get_elements()[elementID];
		glNamedBufferSubData(m_id, element.offset, element.size, data);
	}
	void UniformBuffer::bind() const
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_id);
	}
	/*static*/ void UniformBuffer::unBind()
	{
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
	UniformBuffer::UniformBuffer(UniformBuffer&& otherUniformBuffer) noexcept
		:m_buffer_layout(std::move(otherUniformBuffer.m_buffer_layout)),
		m_id(otherUniformBuffer.m_id)
	{
		otherUniformBuffer.m_id = 0;
	}
	UniformBuffer& UniformBuffer::operator=(UniformBuffer&& otherUniformBuffer) noexcept
	{
		m_buffer_layout = std::move(otherUniformBuffer.m_buffer_layout);
		m_id = otherUniformBuffer.m_id;
		otherUniformBuffer.m_id = 0;
		return *this;
	}
}