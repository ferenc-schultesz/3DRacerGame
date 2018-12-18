#include "ShipNPC.h"
#include "SpotLight.h"
ShipNPC::ShipNPC(string mesh, string pathFile, float s, float sp)
{
	m_position = glm::vec3(0, 0, 0);
	m_strafeVector = glm::vec3(0, 0, 0);

	m_scale = s;

	m_speed = sp;
	m_currentDistance = 0.0f;
	m_sidePos = 0.0f;

	T = glm::vec3(0, 0, 0);
	N = glm::vec3(0, 0, 0);
	B = glm::vec3(0, 0, 0);

	p = glm::vec3(0, 0, 0);
	pNext = glm::vec3(0, 0, 0);

	
	meshFile = mesh;

	m_pPath = new PathNPC(pathFile);
	m_pPath->CreateCentreline();

	m_pShipMesh = new COpenAssetImportMesh;
	m_pShipMesh->Load(meshFile);

	m_pSpotLight = new SpotLight;
}

ShipNPC::~ShipNPC()
{
	delete m_pShipMesh;
	delete m_pPath;
	delete m_pSpotLight;
}

void ShipNPC::Create()
{
	// Create light
	m_pSpotLight->lightPosition = glm::vec4(m_position, 1);
	m_pSpotLight->La = glm::vec3(0.0f);
	m_pSpotLight->Ld = glm::vec3(1.0f);
	m_pSpotLight->Ls = glm::vec3(1.0f);
	m_pSpotLight->direction = glm::vec3(0, 0, -1);
	m_pSpotLight->exponent = 20.0f;
	m_pSpotLight->cutoff = 30.0f;
	m_pSpotLight->Ma = glm::vec3(1.0f);
	m_pSpotLight->Md = glm::vec3(0.0f);
	m_pSpotLight->Ms = glm::vec3(0.0f);
	m_pSpotLight->matShininess = 15.0f;

}

void ShipNPC::Update(double dt)
{
	m_currentDistance += dt * m_speed;

	// UpVector from path
	glm::vec3 upVector(glm::vec3(0,1,0));
	// Calculate actual and next point
	m_pPath->Sample(m_currentDistance, p);
	m_pPath->Sample(m_currentDistance + 1.0f, pNext);

	// TNB frame
	T = glm::normalize(pNext - p);
	N = (glm::cross(T, upVector));
	B = (glm::cross(N, T));

	// Update ship orientation
	m_orientation = glm::mat4(glm::mat3(T, B, N));

	// Update spotlight
	//updates light's position
	m_pSpotLight->UpdatePos(m_position + 15.0f * B);
	// focus light on path
	m_pSpotLight->UpdateDirection(T );

	// use ship's pos instead of camera
	m_position = p + (-20.0f * T) + (2.0f * B) + (m_sidePos * N);

	
}

void ShipNPC::Release()
{
}

COpenAssetImportMesh* ShipNPC::GetMesh()
{
	return m_pShipMesh;
}

glm::vec3 ShipNPC::GetPosition()
{
	return m_position;
}

glm::mat4 ShipNPC::GetOrientation()
{
	return m_orientation;
}

float ShipNPC::GetScale()
{
	return m_scale;
}

SpotLight * ShipNPC::GetLight()
{
	return m_pSpotLight;
}
