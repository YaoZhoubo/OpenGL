#pragma once

#include "GL/glew.h"
#include "Shader.h"
#include "IndexBuffer.h"
#include "VertexArray.h"


#define ASSERT(x) if (!(x)) __debugbreak();
#define  GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall())

void GLClearError();
bool GLLogCall();

class Renderer
{

public:
	void Clear() const;
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};