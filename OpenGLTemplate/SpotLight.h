#pragma once
#include "Common.h"

class SpotLight
{
public:
	SpotLight();
	~SpotLight();

	glm::vec4 lightPosition;
	glm::vec3 La;		
	glm::vec3 Ld;
	glm::vec3 Ls;
	glm::vec3 direction;
	float exponent;
	float cutoff;
	glm::vec3 Ma;
	glm::vec3 Md;
	glm::vec3 Ms;
	float matShininess;

	void UpdatePos(glm::vec3 pos);
	void UpdateDirection(glm::vec3 dir);
};
