#include "VertexArray.h"

#include "VertexBufferLayout.h"
#include "Renderer.h"

VertexArray::VertexArray()
	: m_VBO(0), m_IBO(0), m_ElementsCount(0), m_IndexCount(0)
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
	GLCall(glBindVertexArray(m_RendererID));
}

VertexArray::~VertexArray()
{
	if (m_VBO != 0)
		GLCall(glDeleteBuffers(1, &m_VBO));
	if (m_IBO != 0)
		GLCall(glDeleteBuffers(1, &m_IBO));

	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const void* data, unsigned int size, const VertexBufferLayout& layout)
{
	Bind();

	m_ElementsCount = size;

	GLCall(glGenBuffers(1, &m_VBO));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));

	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));

		if (element.type == GL_UNSIGNED_INT)
		{
			GLCall(glVertexAttribIPointer(i, element.count, element.type,
				layout.GetStride(), (const void*)offset));
		}
		else
		{
			GLCall(glVertexAttribPointer(i, element.count, element.type,
				element.normalized, layout.GetStride(), (const void*)offset));
		}

		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::AddIndexBuffer(const unsigned int* data, unsigned int size, unsigned int type)
{
	Bind();
	m_IndexCount = size / VertexBufferElement::GetSizeOfType(type);

	glGenBuffers(1, &m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO));
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}
