#include "StaticMesh.h"

StaticMesh::StaticMesh()
{
}

StaticMesh::StaticMesh(string f, glm::vec3 pos, glm::vec3 rotV, float rot, float sc)
{
	mesh = new COpenAssetImportMesh();
	mesh->Load(f);
	position = pos;
	rotVector = rotV;
	rotation = rot;
	scale = sc;
}

StaticMesh::~StaticMesh()
{
	delete mesh;
}

COpenAssetImportMesh* StaticMesh::GetMesh()
{
	return mesh;
}

glm::vec3 StaticMesh::GetPosition()
{
	return position;
}

glm::vec3 StaticMesh::GetrotVector()
{
	return rotVector;
}

float StaticMesh::GetRotation()
{
	return rotation;
}

float StaticMesh::GetScale()
{
	return scale;
}
