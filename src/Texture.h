#pragma once

#include "Renderer.h"

class Texture
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;//跟踪纹理的实际文件路径
	unsigned char* m_LocalBuffer;//纹理的本地存储
	int m_Width, m_Heigh, m_BPP;//宽，高，纹理的每像素位
public:
	Texture(const std::string path);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeigh() const { return m_Heigh; }
};