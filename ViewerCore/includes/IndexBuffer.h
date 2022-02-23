#pragma once
namespace TestEngine {
	class IndexBuffer
	{
	public:
		enum class EUsage { Static, Dynamic, Stream };
		~IndexBuffer();
		IndexBuffer(const void* data, const size_t count, const EUsage usage = EUsage::Static);
		IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer& operator=(const IndexBuffer&) = delete;
		IndexBuffer(IndexBuffer&& otherIndexBuffer) noexcept;
		IndexBuffer& operator=(IndexBuffer&& otherIndexBuffer) noexcept;

		void bind() const;
		static void unbind();
		size_t get_count() const noexcept { return m_count; }
		unsigned int get_buffer() const noexcept { return m_id; }
	private:
		unsigned int m_id = 0;
		size_t m_count = 0;
	};
}
