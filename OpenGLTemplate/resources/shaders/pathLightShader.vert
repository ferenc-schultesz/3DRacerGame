#version 400 core

// Structure for matrices
uniform struct Matrices
{
	mat4 projMatrix;
	mat4 modelViewMatrix; 
	mat3 normalMatrix;
} matrices;

uniform float dt;
// Layout of vertex attributes in VBO
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inCoord;
layout (location = 2) in vec3 inNormal;

//out vec3 vColour;	// Colour computed using reflectance model
out vec2 vTexCoord;	// Texture coordinate
out float t;

// This is the entry point into the vertex shader
void main()
{	
	vec3 p = inPosition;
	// Transform the vertex spatial position using 
	gl_Position = matrices.projMatrix * matrices.modelViewMatrix * vec4(p, 1.0);
	// Pass through the texture coordinate
	vTexCoord = inCoord;
	t = dt;
} 