#pragma once

#include "Camera.h"
#include "gamewindow.h"
#include "CatmullRom.h"
#include "Player.h"

// Constructor for camera -- initialise with some default values
CCamera::CCamera()
{
	// to test power ups at 0,0,0
	m_position = glm::vec3(0.0f, 60.0f, 100.0f);
	//m_position = glm::vec3(-331.8f, 299.1f, 1352.0f);
	m_view = glm::vec3(0.0f, 20.0f, 0.0f);
	m_upVector = glm::vec3(0.0f, 1.0f, 0.0f);

	m_speed = 0.1f;

	m_currentDistance = 0.0f;

	T = glm::vec3(0.0f, 0.0f, 0.0f);;
	N = glm::vec3(0.0f, 0.0f, 0.0f);;
	B = glm::vec3(0.0f, 0.0f, 0.0f);;
	p = glm::vec3(0.0f, 0.0f, 0.0f);;
	pNext = glm::vec3(0.0f, 0.0f, 0.0f);

	playerSidePos = 0.0f;

	m_pCatmullRom = NULL;

	// 1 - first person
	// 2 - third person
	// 3 - top view
	// 4 - free view
	cameraType = 2;
}
CCamera::~CCamera()
{}
 
// Set the camera at a specific position, looking at the view point, with a given up vector
void CCamera::Set(glm::vec3 &position, glm::vec3 &viewpoint, glm::vec3 &upVector)
{
	m_position = position;
	m_view = viewpoint;
	m_upVector = upVector;

}

// Respond to mouse movement
void CCamera::SetViewByMouse()
{  
	int middle_x = GameWindow::SCREEN_WIDTH >> 1;
	int middle_y = GameWindow::SCREEN_HEIGHT >> 1;

	float angle_y = 0.0f;
	float angle_z = 0.0f;
	static float rotation_x = 0.0f;

	POINT mouse;
	GetCursorPos(&mouse);

	if (mouse.x == middle_x && mouse.y == middle_y) {
		return;
	}

	SetCursorPos(middle_x, middle_y);

	angle_y = (float) (middle_x - mouse.x) / 1000.0f;
	angle_z = (float) (middle_y - mouse.y) / 1000.0f;

	rotation_x -= angle_z;

	float maxAngle = 1.56f; // Just a little bit below PI / 2

	if (rotation_x > maxAngle) {
		rotation_x = maxAngle;
	} else if (rotation_x < -maxAngle) {
		rotation_x = -maxAngle;
	} else {
		glm::vec3 cross = glm::cross(m_view - m_position, m_upVector);
		glm::vec3 axis = glm::normalize(cross);

		RotateViewPoint(angle_z, axis);
	}

	RotateViewPoint(angle_y, glm::vec3(0, 1, 0));
}

// Rotate the camera view point -- this effectively rotates the camera since it is looking at the view point
void CCamera::RotateViewPoint(float fAngle, glm::vec3 &vPoint)
{
	glm::vec3 vView = m_view - m_position;
	
	glm::mat4 R = glm::rotate(glm::mat4(1), fAngle * 180.0f / (float) M_PI, vPoint);
	glm::vec4 newView = R * glm::vec4(vView, 1);

	m_view = m_position + glm::vec3(newView);
}

// Strafe the camera (side to side motion)
void CCamera::Strafe(double direction)
{
	float speed = (float) (m_speed * direction);

	m_position.x = m_position.x + m_strafeVector.x * speed;
	m_position.z = m_position.z + m_strafeVector.z * speed;

	m_view.x = m_view.x + m_strafeVector.x * speed;
	m_view.z = m_view.z + m_strafeVector.z * speed;

}

// Advance the camera (forward / backward motion)
void CCamera::Advance(double direction)
{
	float speed = (float) (m_speed * direction);

	glm::vec3 view = glm::normalize(m_view - m_position);
	m_position = m_position + view * speed;
	m_view = m_view + view * speed;

}


// Update the camera to respond to mouse motion for rotations and keyboard for translation
void CCamera::Update(double dt, CCatmullRom *m_pCatmullRom)
{
	// Calculate current distance, that is used to calculate next point
	m_currentDistance += dt * m_speed;
	// Upvector for p and pNext
	glm::vec3 upVector;
	// Calculate actual and next point
	m_pCatmullRom->Sample(m_currentDistance, p, upVector);
	m_pCatmullRom->Sample(m_currentDistance + 1.0f, pNext, upVector);
	// TNB frame
	T = glm::normalize(pNext - p);
	N = (glm::cross(T, upVector));
	B = (glm::cross(N, T));
	
	switch (cameraType)
	{
	// first person
	case 1:
		Set(p + -13.0f * T + 6.0f * B + N * playerSidePos, p + 200.0f * T + N * playerSidePos, B);
		break;
	//third person
	case 2:
		Set(p - 50.0f * T + 25.0f * B, p + 10.0f * T, B);
		break;
	// top view
	case 3:
		Set(p + 100.0f * B, p, T);
		break;
	// free view
	case 4:
		glm::vec3 vector = glm::cross(m_view - m_position, m_upVector);
		m_strafeVector = glm::normalize(vector);
		TranslateByKeyboard(dt);
		SetViewByMouse();
		break;
	}
}

// Update the camera to respond to key presses for translation
void CCamera::TranslateByKeyboard(double dt)
{
	if (GetKeyState(VK_UP) & 0x80 || GetKeyState('W') & 0x80) {
		Advance(10.0*dt);
	}

	if (GetKeyState(VK_DOWN) & 0x80 || GetKeyState('S') & 0x80) {
		Advance(-10.0*dt);
	}

	if (GetKeyState(VK_LEFT) & 0x80 || GetKeyState('A') & 0x80) {
		Strafe(-10.0*dt);
	}

	if (GetKeyState(VK_RIGHT) & 0x80 || GetKeyState('D') & 0x80) {
		Strafe(10.0*dt);
	}
}
// Return the camera position
glm::vec3 CCamera::GetPosition() const
{
	return m_position;
}

// Return the camera view point
glm::vec3 CCamera::GetView() const
{
	return m_view;
}

// Return the camera up vector
glm::vec3 CCamera::GetUpVector() const
{
	return m_upVector;
}

// Return the camera strafe vector
glm::vec3 CCamera::GetStrafeVector() const
{
	return m_strafeVector;
}

// Return the camera perspective projection matrix
glm::mat4* CCamera::GetPerspectiveProjectionMatrix()
{
	return &m_perspectiveProjectionMatrix;
}

// Return the camera orthographic projection matrix
glm::mat4* CCamera::GetOrthographicProjectionMatrix()
{
	return &m_orthographicProjectionMatrix;
}

// Set the camera perspective projection matrix to produce a view frustum with a specific field of view, aspect ratio, 
// and near / far clipping planes
void CCamera::SetPerspectiveProjectionMatrix(float fov, float aspectRatio, float nearClippingPlane, float farClippingPlane)
{
	m_perspectiveProjectionMatrix = glm::perspective(fov, aspectRatio, nearClippingPlane, farClippingPlane);
}

// The the camera orthographic projection matrix to match the width and height passed in
void CCamera::SetOrthographicProjectionMatrix(int width, int height)
{
	m_orthographicProjectionMatrix = glm::ortho(0.0f, float(width), 0.0f, float(height));
}

// Get the camera view matrix
glm::mat4 CCamera::GetViewMatrix()
{
	return glm::lookAt(m_position, m_view, m_upVector);
}

float CCamera::GetSpeed()
{
	return m_speed;
}

// The normal matrix is used to transform normals to eye coordinates -- part of lighting calculations
glm::mat3 CCamera::ComputeNormalMatrix(const glm::mat4 &modelViewMatrix)
{
	return glm::transpose(glm::inverse(glm::mat3(modelViewMatrix)));
}

void CCamera::SetCameraType(int cType)
{
	switch (cType)
	{
	case 1:
		cameraType = 1;
		break;
	case 2:
		cameraType = 2;
		break;
	case 3:
		cameraType = 3;
		break;
	case 4:
		cameraType = 4;
		Set(p, p + 10.0f * T, glm::vec3(0, 1, 0));
		break;
	}

}

int CCamera::GetCameraType()
{
	return cameraType;
}

void CCamera::SetPlayerSidePos(float sp)
{
	playerSidePos = sp;
}

void CCamera::IncreaseSpeed(float s)
{
	m_speed += s;
}
