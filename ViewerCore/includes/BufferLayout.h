#pragma once
#include <vector>
#include <cstdint>
namespace TestEngine {
	enum class ShaderDataType
	{
		Float,
		Float2,
		Float3,
		Float4,
		Int,
		Int2,
		Int3,
		Int4,
		Mat4
	};
	struct BufferElement
	{
		ShaderDataType type;
		uint32_t component_type;
		size_t components_count;
		size_t size;
		size_t offset;
		BufferElement(const ShaderDataType type);
	};
	class BufferLayout
	{
	public:
		BufferLayout(std::initializer_list<BufferElement> elements);
		BufferLayout(const BufferLayout& otherBufferLayout);
		BufferLayout(BufferLayout&& otherBufferLayout) noexcept;
		BufferLayout& operator=(const BufferLayout& otherBufferLayout);
		BufferLayout& operator=(BufferLayout&& otherBufferLayout) noexcept;

		const std::vector<BufferElement>& get_elements() const noexcept { return m_elements; }
		size_t get_stride() const noexcept { return m_stride; }

	private:
		std::vector<BufferElement> m_elements;
		size_t m_stride = 0;
	};
}