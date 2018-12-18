#version 400 core
// Structure for matrices
uniform struct Matrices
{
	mat4 projMatrix;
	mat4 modelViewMatrix; 
	mat3 normalMatrix;
	mat4 invViewMatrix;
} matrices;

out vec3 n;
out vec4 p, vWorldPosition;

// Layout of vertex attributes in VBO
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inCoord;
layout (location = 2) in vec3 inNormal;
//out vec3 vColour;	// Colour computed using reflectance model
out vec2 vTexCoord;	// Texture coordinate
// This is the entry point into the vertex shader
void main()
{
	// Save the world position for rendering the skybox
	//vWorldPosition = inPosition;
	vWorldPosition = matrices.invViewMatrix * matrices.modelViewMatrix * vec4(inPosition,1.0f);
	// Transform the vertex spatial position using 
	gl_Position = matrices.projMatrix * matrices.modelViewMatrix * vec4(inPosition, 1.0f);
	// Get the vertex normal and vertex position in eye coordinates
	n = normalize(matrices.normalMatrix * inNormal);
	p = matrices.modelViewMatrix * vec4(inPosition, 1.0f);
	// Pass through the texture coordinate
	vTexCoord = inCoord;
} 
	