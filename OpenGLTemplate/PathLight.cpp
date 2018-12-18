#include "PathLight.h" 
#include "SpotLight.h"

PathLight::PathLight(glm::vec3 pos)
{
	position = pos;
	direction = glm::vec3(0,-1,0);
}
PathLight::~PathLight()
{
	Release();
}
void PathLight::Create(string filename) {
	m_texture.Load(filename); 
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	m_vbo.Create(); m_vbo.Bind();

	
	glm::vec3 A(-1 , 0, 1);
	glm::vec3 B(1, 0, 1);
	glm::vec3 C(1, 0, -1);
	glm::vec3 D(-1, 0, -1);
	glm::vec3 E(0, 2, 0);

	// Vertex positions
	glm::vec3 cubeVertixes[16] =
	{
		A,B,D,C,
		A,B,E,
		B,C,E,
		C,D,E,
		A,E,D,
	};

	// temp variables for points on texture mapping
	float third = 1.0f / 3.0f;
	float twothird = 2.0f / 3.0f;
	float half = 0.5f;
	float one = 1.0f;

	// Texture coordinates
	glm::vec2 cubeTexCoords[16] =
	{
		glm::vec2(0.0f,0.0f),
		glm::vec2(0.5f, 0.0f),
		glm::vec2(0.0f,0.5f),
		glm::vec2(0.5f,0.5f),

		glm::vec2(0.0f,0.5f),
		glm::vec2(0.5f,0.5f),
		glm::vec2(0.25f,1.0f),

		glm::vec2(0.0f,0.5f),
		glm::vec2(0.5f,0.5f),
		glm::vec2(0.25f,1.0f),

		glm::vec2(0.0f,0.5f),
		glm::vec2(0.5f,0.5f),
		glm::vec2(0.25f,1.0f),

		glm::vec2(0.0f,0.5f),
		glm::vec2(0.25f,1.0f),
		glm::vec2(0.5f,0.5f),
	};

	glm::vec3 cubeNormals[5] =
	{
		glm::vec3(glm::cross(C - B, D - B)),
		glm::vec3(glm::cross(A - B, E - B)),
		glm::vec3(glm::cross(E - B, C - B)),
		glm::vec3(glm::cross(D - C, E - C)),
		glm::vec3(glm::cross(E - A, D - A)),

	};
	// Put the vertex attributes in the VBO
	for (unsigned int i = 0; i < 16; i++) {
		m_vbo.AddData(&cubeVertixes[i], sizeof(glm::vec3));
		m_vbo.AddData(&cubeTexCoords[i], sizeof(glm::vec2));
		if (i < 4)
		{
			m_vbo.AddData(&cubeNormals[0], sizeof(glm::vec3));
		}
		else
		{
			m_vbo.AddData(&cubeNormals[i / 4], sizeof(glm::vec3));
		}
	}


	// Upload the VBO to the GPU
	m_vbo.UploadDataToGPU(GL_STATIC_DRAW);

	// Set the vertex attribute locations
	GLsizei istride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, istride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, istride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, istride, (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
}
void PathLight::Render() {
	glBindVertexArray(m_vao);
	m_texture.Bind();

	// Call glDrawArrays to render each side
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDrawArrays(GL_TRIANGLE_STRIP, 4, 3);

	glDrawArrays(GL_TRIANGLE_STRIP, 7, 3);

	glDrawArrays(GL_TRIANGLE_STRIP, 10, 3);

	glDrawArrays(GL_TRIANGLE_STRIP, 13, 3);


}
void PathLight::Release() {
	m_texture.Release();
	glDeleteVertexArrays(1, &m_vao);
	m_vbo.Release();
}

