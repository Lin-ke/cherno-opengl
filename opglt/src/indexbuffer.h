#pragma once
#include "renderer.h"

class IndexBuffer {
private:
	uint32 m_Rendererid;
	uint32 m_Count;
public:
	IndexBuffer(const void* data, uint32 count);
	~IndexBuffer();
	void Bind() const;
	void Unbind() const ;
	inline uint32 GetCount() { return m_Count; }
};