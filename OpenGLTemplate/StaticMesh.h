#pragma once
#include "Common.h"
#include "OpenAssetImportMesh.h"
#include "gamewindow.h"

class COpenAssetImportMesh;

class StaticMesh
{
private:
	COpenAssetImportMesh* mesh;
	glm::vec3 position;
	glm::vec3 rotVector;
	float rotation;
	float scale;
	

public:
	StaticMesh();
	StaticMesh(string f, glm::vec3 pos, glm::vec3 rotV, float rot, float sc);
	~StaticMesh();


	COpenAssetImportMesh* GetMesh();
	glm::vec3 GetPosition();
	glm::vec3 GetrotVector();
	float GetRotation();
	float GetScale();
	

};