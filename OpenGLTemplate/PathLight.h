#pragma once

#include "Common.h" 
#include "Texture.h" 
#include "VertexBufferObject.h"

class SpotLight;
// Class for generating a unit cube 
class PathLight {

public:
	PathLight(glm::vec3 pos);
	~PathLight();
	void Create(string filename);
	void Render();
	void Release();
	glm::vec3 direction;
	glm::vec3 position;

private:
	GLuint m_vao;
	CVertexBufferObject m_vbo;
	CTexture m_texture;
};