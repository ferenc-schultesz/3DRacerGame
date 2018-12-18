#include "SpotLight.h"

SpotLight::SpotLight()
{
}

SpotLight::~SpotLight()
{
}

void SpotLight::UpdatePos(glm::vec3 pos)
{
	lightPosition = glm::vec4(pos, 1);
}

void SpotLight::UpdateDirection(glm::vec3 dir)
{
	direction = dir;
}
