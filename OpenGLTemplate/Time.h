#pragma once

#include "Common.h" 
#include "Texture.h" 
#include "VertexBufferObject.h"

// Class for generating a unit cube 
class CTime {

public:
	CTime(glm::vec3 pos);
	~CTime();
	void Create(string filename);
	void Render();
	void Release();

	glm::vec3 position;

	bool GetAlive();
	void Kill();

private:
	GLuint m_vao;
	CVertexBufferObject m_vbo;
	CTexture m_texture;
	bool alive;
};