#pragma once

#include <iostream>

class VertexBufferLayout;

class VertexArray
{
private:
	unsigned int m_RendererID, m_VBO, m_IBO;
	int m_ElementsCount, m_IndexCount;

public:
	VertexArray();
	~VertexArray();

	void AddIndexBuffer(const unsigned int* data, unsigned int size, unsigned int type);
	void AddBuffer(const void* data, unsigned int size, const VertexBufferLayout& layout);

	inline int GetElementsCount() const { return m_ElementsCount; }
	inline int GetIndexCount() const { return m_IndexCount; }

	void Bind() const;
	void Unbind() const;
};

