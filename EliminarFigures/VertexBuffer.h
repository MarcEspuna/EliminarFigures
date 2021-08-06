#pragma once
class VertexBuffer
{
public:
	VertexBuffer();
	VertexBuffer(const void* data, size_t size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
	void LoadData(const void* data, size_t size);


private:
	unsigned int m_RendererID;
	const void* data;

};

