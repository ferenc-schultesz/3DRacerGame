#include "PathNPC.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <fstream>

PathNPC::PathNPC(string path)
{
	m_vertexCount = 0;
	//w = 100.0f;  atm const public
	pathFile = path;
}

PathNPC::~PathNPC()
{}

// Perform Catmull Rom spline interpolation between four points, interpolating the space between p1 and p2
glm::vec3 PathNPC::Interpolate(glm::vec3 &p0, glm::vec3 &p1, glm::vec3 &p2, glm::vec3 &p3, float t)
{
	float t2 = t * t;
	float t3 = t2 * t;

	glm::vec3 a = p1;
	glm::vec3 b = 0.5f * (-p0 + p2);
	glm::vec3 c = 0.5f * (2.0f*p0 - 5.0f*p1 + 4.0f*p2 - p3);
	glm::vec3 d = 0.5f * (-p0 + 3.0f*p1 - 3.0f*p2 + p3);

	return a + b*t + c*t2 + d*t3;

}


void PathNPC::SetControlPoints()
{
	// Set control points (m_controlPoints) here, or load from disk
	std::ifstream inputFile(pathFile);
	
	vector<string> data;

	if (!inputFile)
	{
		cout << "There was an error opening the file.\n";
	}
	string line;
	while (inputFile >> line)
	{
		data.push_back(line);
	}

	for (int i = 0; i < data.size() -2; i+=3)
	{
		float x = std::stof(data[i]);
		float y = std::stof(data[i+1]);
		float z = std::stof(data[i+2]);
		m_controlPoints.push_back(glm::vec3(x, y, z));
	}
	


	// Optionally, set upvectors (m_controlUpVectors, one for each control point as well)


}


// Determine lengths along the control points, which is the set of control points forming the closed curve
void PathNPC::ComputeLengthsAlongControlPoints()
{
	int M = (int)m_controlPoints.size();

	float fAccumulatedLength = 0.0f;
	m_distances.push_back(fAccumulatedLength);
	for (int i = 1; i < M; i++) {
		fAccumulatedLength += glm::distance(m_controlPoints[i - 1], m_controlPoints[i]);
		m_distances.push_back(fAccumulatedLength);
	}

	// Get the distance from the last point to the first
	fAccumulatedLength += glm::distance(m_controlPoints[M - 1], m_controlPoints[0]);
	m_distances.push_back(fAccumulatedLength);
}


// Return the point (and upvector, if control upvectors provided) based on a distance d along the control polygon
bool PathNPC::Sample(float d, glm::vec3 &p, glm::vec3 &up)
{
	if (d < 0)
		return false;

	int M = (int)m_controlPoints.size();
	if (M == 0)
		return false;


	float fTotalLength = m_distances[m_distances.size() - 1];

	// The the current length along the control polygon; handle the case where we've looped around the track
	float fLength = d - (int)(d / fTotalLength) * fTotalLength;

	// Find the current segment
	int j = -1;
	for (int i = 0; i < (int)m_distances.size(); i++) {
		if (fLength >= m_distances[i] && fLength < m_distances[i + 1]) {
			j = i; // found it!
			break;
		}
	}

	if (j == -1)
		return false;

	// Interpolate on current segment -- get t
	float fSegmentLength = m_distances[j + 1] - m_distances[j];
	float t = (fLength - m_distances[j]) / fSegmentLength;

	// Get the indices of the four points along the control polygon for the current segment
	int iPrev = ((j - 1) + M) % M;
	int iCur = j;
	int iNext = (j + 1) % M;
	int iNextNext = (j + 2) % M;

	// Interpolate to get the point (and upvector)
	p = Interpolate(m_controlPoints[iPrev], m_controlPoints[iCur], m_controlPoints[iNext], m_controlPoints[iNextNext], t);
	if (m_controlUpVectors.size() == m_controlPoints.size())
		up = glm::normalize(Interpolate(m_controlUpVectors[iPrev], m_controlUpVectors[iCur], m_controlUpVectors[iNext], m_controlUpVectors[iNextNext], t));


	return true;
}



// Sample a set of control points using an open Catmull-Rom spline, to produce a set of iNumSamples that are (roughly) equally spaced
void PathNPC::UniformlySampleControlPoints(int numSamples)
{
	glm::vec3 p, up;

	// Compute the lengths of each segment along the control polygon, and the total length
	ComputeLengthsAlongControlPoints();
	float fTotalLength = m_distances[m_distances.size() - 1];

	// The spacing will be based on the control polygon
	float fSpacing = fTotalLength / numSamples;

	// Call PointAt to sample the spline, to generate the points
	for (int i = 0; i < numSamples; i++) {
		Sample(i * fSpacing, p, up);
		m_centrelinePoints.push_back(p);
		if (m_controlUpVectors.size() > 0)
			m_centrelineUpVectors.push_back(up);

	}


	// Repeat once more for truly equidistant points
	m_controlPoints = m_centrelinePoints;
	m_controlUpVectors = m_centrelineUpVectors;
	m_centrelinePoints.clear();
	m_centrelineUpVectors.clear();
	m_distances.clear();
	ComputeLengthsAlongControlPoints();
	fTotalLength = m_distances[m_distances.size() - 1];
	fSpacing = fTotalLength / numSamples;
	for (int i = 0; i < numSamples; i++) {
		Sample(i * fSpacing, p, up);
		m_centrelinePoints.push_back(p);
		if (m_controlUpVectors.size() > 0)
			m_centrelineUpVectors.push_back(up);
	}


}

void PathNPC::Create(string filename)
{
	m_texture.Load(filename); m_texture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

	CreateCentreline();
	
}


void PathNPC::CreateCentreline()
{
	// Call Set Control Points
	SetControlPoints();
	// Call UniformlySampleControlPoints with the number of samples required
	UniformlySampleControlPoints(3000);
	// Create a VAO called m_vaoCentreline and a VBO to get the points onto the graphics card
	// Use VAO to store state associated with vertices
	glGenVertexArrays(1, &m_vaoCentreline);
	glBindVertexArray(m_vaoCentreline);
	// Create a VBO
	CVertexBufferObject vbo;
	vbo.Create();
	vbo.Bind();
	// Texture coordinate
	glm::vec2 texCoord(0.0f, 0.0f);
	// Normal
	glm::vec3 normal(0.0f, 1.0f, 0.0f);

	for (unsigned int i = 0; i < m_centrelinePoints.size(); i++) {

		glm::vec3 v = m_centrelinePoints[i];
		vbo.AddData(&v, sizeof(glm::vec3));
		vbo.AddData(&texCoord, sizeof(glm::vec2));
		vbo.AddData(&normal, sizeof(glm::vec3));
	}
	// Upload the VBO to the GPU
	vbo.UploadDataToGPU(GL_STATIC_DRAW);


	// Set the vertex attribute locations
	GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3)
		+ sizeof(glm::vec2)));

}

void PathNPC::RenderCentreline()
{
	// Bind the VAO m_vaoCentreline and render it
	glBindVertexArray(m_vaoCentreline);
	m_texture.Bind();
	glLineWidth(2.0f);
	glPointSize(5.0f);
	glDrawArrays(GL_POINTS, 0, m_centrelinePoints.size());
	glDrawArrays(GL_LINE_LOOP, 0, m_centrelinePoints.size());
}

//
//void PathNPC::CreateOffsetCurves()
//{
//	// Compute the offset curves, one left, and one right.  Store the points in m_leftOffsetPoints and m_rightOffsetPoints respectively
//
//	for (unsigned int i = 0; i < m_centrelinePoints.size(); ++i)
//	{
//		glm::vec3 p = m_centrelinePoints[i];
//		glm::vec3 pNext;
//		if (i == m_centrelinePoints.size() - 1)
//		{
//			pNext = m_centrelinePoints[0];
//		}
//		else
//		{
//			pNext = m_centrelinePoints[i + 1];
//		}
//
//		//TNB frame
//		glm::vec3 T = glm::normalize(pNext - p);
//		glm::vec3 N(glm::cross(T, m_controlUpVectors[i]));
//		glm::vec3 B(glm::cross(N, T));
//
//		glm::vec3 l = p - (W / 2) * N;
//		glm::vec3 r = p + (W / 2) * N;
//
//
//		m_leftOffsetPoints.push_back(l);
//		m_rightOffsetPoints.push_back(r);
//
//	}
//
//	// Generate two VAOs called m_vaoLeftOffsetCurve and m_vaoRightOffsetCurve, each with a VBO, and get the offset curve points on the graphics card
//	// Note it is possible to only use one VAO / VBO with all the points instead.
//	// Left offset points
//	glGenVertexArrays(1, &m_vaoLeftOffsetCurve);
//	glBindVertexArray(m_vaoLeftOffsetCurve);
//	// Create a VBO
//	CVertexBufferObject vbo;
//	vbo.Create();
//	vbo.Bind();
//	// Texture coordinate
//	glm::vec2 texCoord(0.0f, 0.0f);
//	// Normal
//	glm::vec3 normal(0.0f, 1.0f, 0.0f);
//
//	for (unsigned int i = 0; i < m_leftOffsetPoints.size(); i++) {
//
//		glm::vec3 v = m_leftOffsetPoints[i];
//		vbo.AddData(&v, sizeof(glm::vec3));
//		vbo.AddData(&texCoord, sizeof(glm::vec2));
//		vbo.AddData(&normal, sizeof(glm::vec3));
//	}
//	// Upload the VBO to the GPU
//	vbo.UploadDataToGPU(GL_STATIC_DRAW);
//	// Set the vertex attribute locations
//	GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
//	// Vertex positions
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
//	// Texture coordinates
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
//	// Normal vectors
//	glEnableVertexAttribArray(2);
//	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3)
//		+ sizeof(glm::vec2)));
//
//	// Right offset points
//	glGenVertexArrays(1, &m_vaoRightOffsetCurve);
//	glBindVertexArray(m_vaoRightOffsetCurve);
//	// Create a VBO
//	//CVertexBufferObject vbo;
//	vbo.Create();
//	vbo.Bind();
//	// Texture coordinate
//	texCoord = glm::vec2(0.0f, 0.0f);
//	// Normal
//	normal = glm::vec3(0.0f, 1.0f, 0.0f);
//
//	for (unsigned int i = 0; i < m_rightOffsetPoints.size(); i++) {
//
//		glm::vec3 v = m_rightOffsetPoints[i];
//		vbo.AddData(&v, sizeof(glm::vec3));
//		vbo.AddData(&texCoord, sizeof(glm::vec2));
//		vbo.AddData(&normal, sizeof(glm::vec3));
//	}
//	// Upload the VBO to the GPU
//	vbo.UploadDataToGPU(GL_STATIC_DRAW);
//	// Set the vertex attribute locations
//	stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
//	// Vertex positions
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
//	// Texture coordinates
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
//	// Normal vectors
//	glEnableVertexAttribArray(2);
//	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3)
//		+ sizeof(glm::vec2)));
//
//
//}
//
//
//void PathNPC::CreateTrack()
//{
//
//	// Generate a VAO called m_vaoTrack and a VBO to get the offset curve points and indices on the graphics card
//	glGenVertexArrays(1, &m_vaoTrack);
//	glBindVertexArray(m_vaoTrack);
//	// Create a VBO
//	CVertexBufferObject vbo;
//	vbo.Create();
//	vbo.Bind();
//	// Texture coordinate
//	glm::vec2 texCoord(0.0f, 0.0f);
//	// Normal
//	glm::vec3 normal(0.0f, 1.0f, 0.0f);
//
//	std::vector<glm::vec2> pathTexCoords;
//
//	for (int i = 0; i < m_leftOffsetPoints.size() * 2 - 2; ++i)
//	{
//		pathTexCoords.push_back(glm::vec2(0.0f, 0.0f));
//		pathTexCoords.push_back(glm::vec2(1.0f, 0.0f));
//		pathTexCoords.push_back(glm::vec2(1.0f / 23.0f, 0.0f));
//		pathTexCoords.push_back(glm::vec2(1.0f, 1.0f / 23.0f));
//	}
//
//
//	for (unsigned int i = 0; i < m_leftOffsetPoints.size(); i++) {
//
//		glm::vec3 vl = m_leftOffsetPoints[i];
//		glm::vec2 tl = pathTexCoords[i];
//
//		vbo.AddData(&vl, sizeof(glm::vec3));
//		vbo.AddData(&tl, sizeof(glm::vec2));
//		vbo.AddData(&normal, sizeof(glm::vec3));
//		++m_vertexCount;
//
//		glm::vec3 vr = m_rightOffsetPoints[i];
//		glm::vec2 tr = pathTexCoords[i + 1];
//		vbo.AddData(&vr, sizeof(glm::vec3));
//		vbo.AddData(&tr, sizeof(glm::vec2));
//		vbo.AddData(&normal, sizeof(glm::vec3));
//		++m_vertexCount;
//
//	}
//	// Add extra point repeating the first two point one from l one from r
//	glm::vec3 vl = m_leftOffsetPoints[0];
//	vbo.AddData(&vl, sizeof(glm::vec3));
//	vbo.AddData(&texCoord, sizeof(glm::vec2));
//	vbo.AddData(&normal, sizeof(glm::vec3));
//	++m_vertexCount;
//
//	glm::vec3 vr = m_rightOffsetPoints[0];
//	vbo.AddData(&vr, sizeof(glm::vec3));
//	vbo.AddData(&texCoord, sizeof(glm::vec2));
//	vbo.AddData(&normal, sizeof(glm::vec3));
//	++m_vertexCount;
//
//	// Upload the VBO to the GPU
//	vbo.UploadDataToGPU(GL_STATIC_DRAW);
//	// Set the vertex attribute locations
//	GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
//	// Vertex positions
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
//	// Texture coordinates
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
//	// Normal vectors
//	glEnableVertexAttribArray(2);
//	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3)
//		+ sizeof(glm::vec2)));
//}




//void PathNPC::RenderOffsetCurves()
//{
//	// Bind the VAO m_vaoLeftOffsetCurve and render it
//	glBindVertexArray(m_vaoLeftOffsetCurve);
//	m_texture.Bind();
//	glDrawArrays(GL_POINTS, 0, m_leftOffsetPoints.size());
//	glDrawArrays(GL_LINE_STRIP, 0, m_leftOffsetPoints.size());
//
//	// Bind the VAO m_vaoRightOffsetCurve and render it
//	glBindVertexArray(m_vaoRightOffsetCurve);
//	m_texture.Bind();
//	glDrawArrays(GL_POINTS, 0, m_rightOffsetPoints.size());
//	glDrawArrays(GL_LINE_STRIP, 0, m_rightOffsetPoints.size());
//}
//
//
//void PathNPC::RenderTrack()
//{
//	// Bind the VAO m_vaoTrack and render it
//	glBindVertexArray(m_vaoTrack);
//	m_texture.Bind();
//
//	// Shows edged of triangle but not filling
//	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//
//	//glDisable(GL_CULL_FACE);
//
//	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_vertexCount);
//
//	// Fill triangles for the rest such as skybox
//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//}

int PathNPC::CurrentLap(float d)
{

	return (int)(d / m_distances.back());

}

float PathNPC::m_getLength(glm::vec3 &v)
{
	float length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	return length;
}