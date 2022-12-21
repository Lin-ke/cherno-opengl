#include "indexbuffer.h"
void IndexBuffer::Bind() const{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Rendererid));
}
void IndexBuffer::Unbind() const{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
IndexBuffer::IndexBuffer(const void* data,uint32 count) 
: m_Count(count){
	ASSERT(sizeof(uint32) == sizeof(GLuint));
	GLCall(glGenBuffers(1, &m_Rendererid));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Rendererid)); // (GLsizei n, GLuint* buffers));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(uint32), data, GL_STATIC_DRAW));
}
IndexBuffer::~IndexBuffer() {
	GLCall(glDeleteBuffers(1,&m_Rendererid));
}