#include "Cube.h" 

CCube::CCube() 
{

}
CCube::~CCube() 
{
	Release();
}
void CCube::Create(string filename) {
	m_texture.Load(filename); m_texture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT); 
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT); 
	glGenVertexArrays(1, &m_vao); 
	glBindVertexArray(m_vao); 
	m_vbo.Create(); m_vbo.Bind(); 
	
	// Write the code to add interleaved vertex attributes to the VBO

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



	// Texture coordinates
	glm::vec2 cubeTexCoords[24] =
	{
		glm::vec2(0.0f, 0.0f),
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),

		glm::vec2(0.0f, 0.0f),
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),

		glm::vec2(0.0f, 0.0f),
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),

		glm::vec2(0.0f, 0.0f),
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),

		glm::vec2(0.0f, 0.0f),
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),

		glm::vec2(0.0f, 0.0f),
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),

	};


	// Cube (1 side) normal
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
		m_vbo.AddData(&cubeNormals[i/4], sizeof(glm::vec3));
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
void CCube::Render() {
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
void CCube::Release() {
	m_texture.Release(); 
	glDeleteVertexArrays(1, &m_vao); 
	m_vbo.Release();
}