#pragma once
#include "Common.h"

class COpenAssetImportMesh;
class SpotLight;

class Player {
private:
	glm::vec3 m_position;			// The position of the camera's centre of projection
	glm::vec3 m_strafeVector;		// The camera's strafe vector
	glm::mat4 m_orientation;

	float m_speed;					// How fast the ship moves
	float m_currentDistance;
	float m_sidePos;

	glm::vec3 T;
	glm::vec3 N;
	glm::vec3 B;

	glm::vec3 p;
	glm::vec3 pNext;

	CCatmullRom *m_pCatmullRom;
	COpenAssetImportMesh *m_pShipMesh;
	SpotLight *m_pSpotLight;

public:
	Player();
	~Player();
	void Create();
	void Update(double dt, CCatmullRom *m_pCatmullRom, CCamera *m_pCamera);
	void TranslateByKeyboard(double dt, float w);
	void Release();
	void Strafe(double);
	COpenAssetImportMesh* GetMesh();
	glm::vec3 GetPosition();
	glm::mat4 GetOrientation();
	SpotLight* GetLight();
	float GetCurrentDist();

};