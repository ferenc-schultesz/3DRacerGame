#pragma once

#include "Common.h"
#include "GameWindow.h"
#include "Timer.h"

// Classes used in game.  For a new class, declare it here and provide a pointer to an object of this class below.  Then, in Game.cpp, 
// include the header.  In the Game constructor, set the pointer to NULL and in Game::Initialise, create a new object.  Don't forget to 
// delete the object in the destructor.   
class CCamera;
class CSkybox;
class CShader;
class CShaderProgram;
class CPlane;
class CFreeTypeFont;
class CHighResolutionTimer;
class CSphere;
class COpenAssetImportMesh;
class CAudio;
class CCatmullRom;
class Player;
class CTime;
class CSpeedUp;
class ShipNPC;
class StaticMesh;
class PathLight;
class Timer;
class CHeightMapTerrain;

class Game {
private:
	// Three main methods used in the game.  Initialise runs once, while Update and Render run repeatedly in the game loop.
	void Initialise();
	void Update();
	void Render();

	// Pointers to game objects.  They will get allocated in Game::Initialise()
	CSkybox *m_pSkybox;
	CCamera *m_pCamera;
	vector <CShaderProgram *> *m_pShaderPrograms;
	vector <CTime*> m_pTimePowerUps;
	vector <CSpeedUp*> m_pSpeedPowerUps;
	vector <CSpeedUp*> m_pSpeedDownPowerUps;
	vector <PathLight*> m_pPathLights;
	//PathLight *m_pPathLight;

	CFreeTypeFont *m_pFtFont;
	Player *m_pPlayer;
	
	//COpenAssetImportMesh *m_pBarrel;
	//CSphere *m_pSphere;

	CHighResolutionTimer *m_pHighResolutionTimer;
	//CAudio *m_pAudio;
	CCatmullRom *m_pCatmullRom;

	CHeightMapTerrain *m_pHeightmapTerrain;
	CHeightMapTerrain *m_pHeightmapWater;

	// add npc ships
	ShipNPC *m_pShipFlyingCar1;
	ShipNPC *m_pShipFlyingCar;
	ShipNPC *m_pFighter1;
	ShipNPC *m_pX17Viper;
	ShipNPC *m_pFighter;
	ShipNPC *m_pFighter2;

	vector<ShipNPC*> ships;

	StaticMesh *m_pTower;
	StaticMesh *m_pScifiFloating;
	StaticMesh *m_pColony;
	StaticMesh *m_pCargo;
	StaticMesh *m_pGalacticCruiser;
	StaticMesh *m_pAncientCastle;
	StaticMesh *m_pScifiTropical;
	StaticMesh *m_pNaboo;

	vector<StaticMesh*> meshes;

	// Some other member variables
	double m_dt;
	float m_t;
	int m_framesPerSecond;
	bool m_appActive;
	float m_currentDistance;
	//float m_camreaSpeed;
	float poUpRotation;

	bool darkmode;
	Timer* m_pTimer;
	int t_Left;
	unsigned int gameState;

public:
	Game();
	~Game();
	static Game& GetInstance();
	LRESULT ProcessEvents(HWND window,UINT message, WPARAM w_param, LPARAM l_param);
	void SetHinstance(HINSTANCE hinstance);
	WPARAM Execute();
	int GetGameState();
	void SetGameState(unsigned int s);


private:
	static const int FPS = 60;
	void DisplayGUI();
	void GameLoop();
	GameWindow m_gameWindow;
	HINSTANCE m_hInstance;
	int m_frameCount;
	double m_elapsedTime;
	void writeLog(glm::vec3 & v);
	void checkColl();
};
