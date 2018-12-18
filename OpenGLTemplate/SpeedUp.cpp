#include "SpeedUp.h" 

CSpeedUp::CSpeedUp(glm::vec3 pos)
{
	position = pos;
	alive = true;
}
CSpeedUp::~CSpeedUp()
{
	Release();
}
void CSpeedUp::Create(string filename) {
	m_texture.Load(filename);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	m_vbo.Create(); m_vbo.Bind();

	// Write the code to add interleaved vertex attributes to the VBO

	glm::vec3 P1 = glm::vec3(0, 1, 0);
	glm::vec3 P2 = glm::vec3(1, 0, 0);
	glm::vec3 P3 = glm::vec3(0, 0, 1);
	glm::vec3 P4 = glm::vec3(0, 0, 0);

	// Vertex positions
	glm::vec3 tetraVertixes[12] = { P3,P2,P1,
		P2,P4,P1,
		P3,P1,P4,
		P4,P2,P3 };



	// Texture coordinates
	glm::vec2 tetraTexCoords[12] =
	{
		
		//bottom
		glm::vec2(0.5f, 0.0f),
		glm::vec2(9.0f / 12.0f, 10.0f / 23.0f),
		glm::vec2(0.25f, 10.0f / 23.0f),

		//extra
		glm::vec2(0.75f, 10.0f / 23.0f),
		glm::vec2(0.5f , 20.0f / 23.0f),
		glm::vec2(0.25f , 10.0f / 23.0f),

		//smiley
		glm::vec2(0.25f , 10.0f / 23.0f),
		glm::vec2(0.5f , 0.0f),
		glm::vec2(0.0f, 0.0f),

		//time
		glm::vec2(1.0f , 0.0f),
		glm::vec2(0.75f, 10.0f / 23.0f),
		glm::vec2(0.5f , 0.0f),

	};

	// Cube (1 side) normal
	glm::vec3 tetraNormals[4] =
	{
		glm::vec3(glm::cross(P2 - P3,P1 - P3)),
		glm::vec3(0,0,-1.0f),
		glm::vec3(-1.0f,0,0),
		glm::vec3(0,-1.0f,0)


	};
	// Put the vertex attributes in the VBO
	for (unsigned int i = 0; i < 12; i++) {
		m_vbo.AddData(&tetraVertixes[i], sizeof(glm::vec3));
		m_vbo.AddData(&tetraTexCoords[i], sizeof(glm::vec2));
		m_vbo.AddData(&tetraNormals[i / 3], sizeof(glm::vec3));
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

	// Upload data to GPU m_vbo.UploadDataToGPU(GL_STATIC_DRAW); GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2); // Vertex positions glEnableVertexAttribArray(0); glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0); // Texture coordinates glEnableVertexAttribArray(1); glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3)); // Normal vectors glEnableVertexAttribArray(2); glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
}
void CSpeedUp::Render() {
	glBindVertexArray(m_vao);
	m_texture.Bind();

	// Call glDrawArrays to render each side
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);

	glDrawArrays(GL_TRIANGLE_STRIP, 3, 3);

	glDrawArrays(GL_TRIANGLE_STRIP, 6, 3);

	glDrawArrays(GL_TRIANGLE_STRIP, 9, 3);

}
void CSpeedUp::Release() {
	m_texture.Release();
	glDeleteVertexArrays(1, &m_vao);
	m_vbo.Release();
}

glm::vec3 CSpeedUp::GetPosition()
{
	return position;
}

bool CSpeedUp::GetAlive()
{
	return alive;
}

void CSpeedUp::Kill()
{
	alive = false;
}
