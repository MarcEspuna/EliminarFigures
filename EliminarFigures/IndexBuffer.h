#pragma once
class IndexBuffer
{
public:
	IndexBuffer(const void* data, size_t size);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	inline size_t GetCount() const { return m_Count; }

private:
	unsigned int m_RendererID;
	size_t m_Count;
	

};

