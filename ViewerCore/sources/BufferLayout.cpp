#include "GL/glew.h"
#include "BufferLayout.h"
#include "LogManager.h"
namespace TestEngine {
	constexpr unsigned int get_components_count_from_shader_data_type(const ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:
		case ShaderDataType::Int:
			return 1;
		case ShaderDataType::Float2:
		case ShaderDataType::Int2:
			return 2;
		case ShaderDataType::Float3:
		case ShaderDataType::Int3:
			return 3;
		case ShaderDataType::Float4:
		case ShaderDataType::Int4:
			return 4;
		case ShaderDataType::Mat4:
			return 16;
		}
		LogManager::Error() << "get_components_count_from_shader_data_type: unknown ShaderDataType!" << LogManager::Show();
		return 0;
	}
	constexpr unsigned int get_component_type_from_shader_data_type(const ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:
		case ShaderDataType::Float2:
		case ShaderDataType::Float3:
		case ShaderDataType::Float4:
		case ShaderDataType::Mat4:
			return GL_FLOAT;

		case ShaderDataType::Int:
		case ShaderDataType::Int2:
		case ShaderDataType::Int3:
		case ShaderDataType::Int4:
			return GL_INT;
		}
		LogManager::Error() << "get_component_type_from_shader_data_type: unknown ShaderDataType!" << LogManager::Show();
		return 0;
	}
	constexpr size_t get_type_size_from_shader_data_type(const ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:
		case ShaderDataType::Float2:
		case ShaderDataType::Float3:
		case ShaderDataType::Float4:
		case ShaderDataType::Mat4:
			return sizeof(GLfloat) * get_components_count_from_shader_data_type(type);
		case ShaderDataType::Int:
		case ShaderDataType::Int2:
		case ShaderDataType::Int3:
		case ShaderDataType::Int4:
			return sizeof(GLint) * get_components_count_from_shader_data_type(type);
		}
		LogManager::Error() << "get_type_size_from_shader_data_type: unknown ShaderDataType!" << LogManager::Show();
		return 0;
	}

	BufferElement::BufferElement(const ShaderDataType _type)
		:type(_type),
		component_type(get_component_type_from_shader_data_type(_type)),
		components_count(get_components_count_from_shader_data_type(_type)),
		size(get_type_size_from_shader_data_type(_type)),
		offset(0) {}
	BufferLayout::BufferLayout(std::initializer_list<BufferElement> elements)
		: m_elements(std::move(elements)),
		m_stride(0)
	{
		size_t offset = 0;
		for (auto& element : m_elements)
		{
			element.offset = offset;
			offset += element.size;
			m_stride += element.size;
		}
	}
	BufferLayout::BufferLayout(const BufferLayout& otherBufferLayout)
		:m_stride(otherBufferLayout.m_stride),
		m_elements(otherBufferLayout.m_elements)
	{
	}
	BufferLayout::BufferLayout(BufferLayout&& otherBufferLayout) noexcept
		:m_stride(otherBufferLayout.m_stride),
		m_elements(std::move(otherBufferLayout.m_elements))
	{
	}
	BufferLayout& BufferLayout::operator=(const BufferLayout& otherBufferLayout)
	{
		m_stride = otherBufferLayout.m_stride;
		m_elements = otherBufferLayout.m_elements;
		return *this;
	}
	BufferLayout& BufferLayout::operator=(BufferLayout&& otherBufferLayout) noexcept
	{
		m_stride = otherBufferLayout.m_stride;
		m_elements = std::move(otherBufferLayout.m_elements);
		return *this;
	}
}