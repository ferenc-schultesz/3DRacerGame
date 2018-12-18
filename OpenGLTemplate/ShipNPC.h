#pragma once
#include "Common.h"
#include "OpenAssetImportMesh.h"
#include "gamewindow.h"
#include "PathNPC.h"


class COpenAssetImportMesh;
class SpotLight;

class ShipNPC {
private:
	glm::vec3 m_position;			// The position of the camera's centre of projection
	glm::vec3 m_strafeVector;		// The camera's strafe vector
	glm::mat4 m_orientation;

	float m_scale;

	float m_speed;					// How fast the ship moves
	float m_currentDistance;
	float m_sidePos;

	glm::vec3 T;
	glm::vec3 N;
	glm::vec3 B;

	glm::vec3 p;
	glm::vec3 pNext;


	PathNPC *m_pPath;
	COpenAssetImportMesh *m_pShipMesh;
	SpotLight *m_pSpotLight;

	string meshFile;
	string pathFile;
	

public:
	ShipNPC(string mesh, string path, float scale, float speed);
	~ShipNPC();
	void Create();
	void Update(double dt);
	void Release();
	
	COpenAssetImportMesh* GetMesh();
	glm::vec3 GetPosition();
	glm::mat4 GetOrientation();
	float GetScale();
	SpotLight* GetLight();
};