#pragma once

#include "Player.h"
#include "OpenAssetImportMesh.h"
#include "gamewindow.h"
#include "Camera.h"
#include "SpotLight.h"

Player::Player()
{
	m_position = glm::vec3(0, 0, 0);
	m_strafeVector = glm::vec3(0, 0, 0);

	m_speed = 0.5f;
	m_currentDistance = 0.0f;
	m_sidePos = 0.0f;

	T = glm::vec3(0, 0, 0);
	N = glm::vec3(0, 0, 0);
	B = glm::vec3(0, 0, 0);

	p = glm::vec3(0, 0, 0);
	pNext = glm::vec3(0, 0, 0);

	m_pCatmullRom = NULL;

	m_pShipMesh = new COpenAssetImportMesh;
	m_pSpotLight = new SpotLight;
}

Player::~Player()
{
	delete m_pShipMesh;
	delete m_pSpotLight;
}

void Player::Create()
{
	m_pShipMesh->Load("resources\\models\\Starship\\starship.obj");

	// Create light
	m_pSpotLight->lightPosition = glm::vec4(m_position, 1);
	m_pSpotLight->La = glm::vec3(0.0f);
	m_pSpotLight->Ld = glm::vec3(1.0f);
	m_pSpotLight->Ls = glm::vec3(1.0f);
	m_pSpotLight->direction = glm::vec3(0,0,-1);
	m_pSpotLight->exponent = 20.0f;
	m_pSpotLight->cutoff = 30.0f;
	m_pSpotLight->Ma = glm::vec3(1.0f);
	m_pSpotLight->Md = glm::vec3(0.0f);
	m_pSpotLight->Ms = glm::vec3(0.0f);
	m_pSpotLight->matShininess = 15.0f;
}

void Player::Update(double dt, CCatmullRom * m_pCatmullRom, CCamera * m_pCamera)
{
	m_currentDistance += dt * m_pCamera->GetSpeed();

	// UpVector from catmullrom
	glm::vec3 upVector;
	// Calculate actual and next point
	m_pCatmullRom->Sample(m_currentDistance, p, upVector);
	m_pCatmullRom->Sample(m_currentDistance + 1.0f, pNext, upVector);

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
	m_pSpotLight->UpdateDirection(T + -0.5f * B);

	// update player's position
	m_position = p + (-20.0f * T) + (2.0f * B) + (m_sidePos * N);
	

	TranslateByKeyboard(dt, m_pCatmullRom->W);
	m_pCamera->SetPlayerSidePos(m_sidePos);
}


void Player::TranslateByKeyboard(double dt, float w)
{

	if (GetKeyState(VK_LEFT) & 0x80 || GetKeyState('A') & 0x80) {
		if (m_sidePos < -w / 2 + 2) {}
		else
		{
			m_sidePos += -m_speed;

		}
	}
	if (GetKeyState(VK_RIGHT) & 0x80 || GetKeyState('D') & 0x80) {
		if (m_sidePos > w / 2 - 2) {}
		else
		{
			m_sidePos += m_speed;
		}
	}
	
}

void Player::Release()
{
}

void Player::Strafe(double)
{
}

COpenAssetImportMesh* Player::GetMesh()
{
	return m_pShipMesh;
}

glm::vec3 Player::GetPosition()
{
	return m_position;
}

glm::mat4 Player::GetOrientation()
{
	return m_orientation;
}

SpotLight* Player::GetLight()
{
	return m_pSpotLight;
}

float Player::GetCurrentDist()
{
	return m_currentDistance;
}
