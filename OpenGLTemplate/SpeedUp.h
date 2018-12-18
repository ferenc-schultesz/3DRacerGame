#pragma once

#include "Common.h" 
#include "Texture.h" 
#include "VertexBufferObject.h"

// Class for generating a unit cube 
class CSpeedUp {
public:
	CSpeedUp(glm::vec3 pos);
	~CSpeedUp();
	void Create(string filename);
	void Render();
	void Release();
	glm::vec3 GetPosition();
	bool GetAlive();
	void Kill();
private:
	GLuint m_vao;
	CVertexBufferObject m_vbo;
	CTexture m_texture;
	glm::vec3 position;
	bool alive;
};