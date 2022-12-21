#include "vertexbuffer.h"
void VertexBuffer::Bind() {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Rendererid));
}
void VertexBuffer::Unbind() {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
VertexBuffer::VertexBuffer(const void* data,uint32 size) {
	GLCall(glGenBuffers(1, &m_Rendererid));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Rendererid)); // (GLsizei n, GLuint* buffers));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}
VertexBuffer::~VertexBuffer() {
	GLCall(glDeleteBuffers(1,&m_Rendererid));
}