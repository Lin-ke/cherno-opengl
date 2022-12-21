#pragma once
#include "renderer.h"

class VertexBuffer {
private:
	uint32 m_Rendererid;
public:
	VertexBuffer(const void* data, uint32 size);
	~VertexBuffer();
	void Bind();
	void Unbind();
};