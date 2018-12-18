#include "Time.h" 

CTime::CTime(glm::vec3 pos)
{
	position = pos;
}
CTime::~CTime()
{
	Release();
}
void CTime::Create(string filename) {
	m_texture.Load(filename); m_texture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	m_vbo.Create(); m_vbo.Bind();
	alive = true;

	// Vertex positions
	glm::vec3 cubeVertixes[24] =
	{
		glm::vec3(-1, -1, 1),
		glm::vec3(1, -1, 1),
		glm::vec3(-1, 1, 1),
		glm::vec3(1, 1, 1),

		glm::vec3(1, -1, 1),
		glm::vec3(1, -1, -1),
		glm::vec3(1, 1, 1),
		glm::vec3(1, 1, -1),

		glm::vec3(1, -1, -1),
		glm::vec3(-1, -1, -1),
		glm::vec3(1, 1, -1),
		glm::vec3(-1, 1, -1),

		glm::vec3(-1, -1, -1),
		glm::vec3(-1, -1, 1),
		glm::vec3(-1, 1, -1),
		glm::vec3(-1, 1, 1),

		glm::vec3(1, 1, -1),
		glm::vec3(-1, 1, -1),
		glm::vec3(1, 1, 1),
		glm::vec3(-1, 1, 1),

		glm::vec3(-1, -1, 1),
		glm::vec3(-1, -1, -1),
		glm::vec3(1, -1, 1),
		glm::vec3(1, -1, -1),
	};

	// temp variables for points on texture mapping
	float third = 1.0f / 3.0f;
	float twothird = 2.0f / 3.0f;
	float half = 0.5f;
	float one = 1.0f;

	// Texture coordinates
	glm::vec2 cubeTexCoords[24] =
	{
	
		glm::vec2(0.0f, 0.0f),
		glm::vec2(third, 0.0f),
		glm::vec2(0.0f, third+0.17f),
		glm::vec2(third, third+0.17f),

		
		glm::vec2(third, 0.0f),
		glm::vec2(twothird, 0.0f),
		glm::vec2(third, half),
		glm::vec2(twothird, half),

		glm::vec2(twothird, 0.0f),
		glm::vec2(one, 0.0f),
		glm::vec2(twothird, half),
		glm::vec2(one, half),

		glm::vec2(0.0f, half),
		glm::vec2(third, half),
		glm::vec2(0.0f, one),
		glm::vec2(third, one),

		glm::vec2(third, half),
		glm::vec2(twothird, half),
		glm::vec2(third, one),
		glm::vec2(twothird, one),

		glm::vec2(twothird, half),
		glm::vec2(one, half),
		glm::vec2(twothird, one),
		glm::vec2(one, one),

	};

	glm::vec3 cubeNormals[6] =
	{
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f)

	};
	// Put the vertex attributes in the VBO
	for (unsigned int i = 0; i < 24; i++) {
		m_vbo.AddData(&cubeVertixes[i], sizeof(glm::vec3));
		m_vbo.AddData(&cubeTexCoords[i], sizeof(glm::vec2));
		m_vbo.AddData(&cubeNormals[i / 4], sizeof(glm::vec3));
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
void CTime::Render() {
	glBindVertexArray(m_vao);
	m_texture.Bind();

	// Call glDrawArrays to render each side
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);

	glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);

	glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);

	glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);

	glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);
}
void CTime::Release() {
	m_texture.Release();
	glDeleteVertexArrays(1, &m_vao);
	m_vbo.Release();
}

bool CTime::GetAlive()
{
	return alive;
}

void CTime::Kill()
{
	alive = false;
}
