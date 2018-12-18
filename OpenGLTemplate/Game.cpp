/* 
OpenGL Template for INM376 / IN3005
City University London, School of Mathematics, Computer Science and Engineering
Source code drawn from a number of sources and examples, including contributions from
 - Ben Humphrey (gametutorials.com), Michal Bubner (mbsoftworks.sk), Christophe Riccio (glm.g-truc.net)
 - Christy Quinn, Sam Kellett and others

 For educational use by Department of Computer Science, City University London UK.

 This template contains a skybox, simple terrain, camera, lighting, shaders, texturing

 Potential ways to modify the code:  Add new geometry types, shaders, change the terrain, load new meshes, change the lighting, 
 different camera controls, different shaders, etc.
 
 Template version 5.0a 29/01/2017
 Dr Greg Slabaugh (gregory.slabaugh.1@city.ac.uk) 
*/


#include "game.h"


// Setup includes
#include "HighResolutionTimer.h"
#include "GameWindow.h"
#include "IOStream.hpp"

// Game includes
#include "Camera.h"
#include "Skybox.h"
#include "Plane.h"
#include "Shaders.h"
#include "FreeTypeFont.h"
#include "Sphere.h"
#include "MatrixStack.h"
#include "OpenAssetImportMesh.h"
#include "Audio.h"
#include "Player.h"
#include "Time.h"
#include "SpeedUp.h"
#include "PathLight.h"
#include "ShipNPC.h"
#include "StaticMesh.h"
#include "SpotLight.h"
#include "HeightMapTerrain.h"

#include <iostream>
#include <fstream>

// Constructor
Game::Game()
{
	m_pSkybox = NULL;
	m_pCamera = NULL;
	m_pShaderPrograms = NULL;
	m_pFtFont = NULL;
	m_pPlayer = NULL;

	m_pHighResolutionTimer = NULL;
	//m_pAudio = NULL;
	m_pCatmullRom = NULL;
	m_pHeightmapTerrain = NULL;
	m_pHeightmapTerrain = NULL;

	// NPC ships
	m_pShipFlyingCar1 = NULL;
	m_pShipFlyingCar = NULL;
	m_pFighter1 = NULL;
	m_pX17Viper = NULL;
	m_pFighter = NULL;
	m_pFighter2 = NULL;

	// Static meshes
	m_pTower = NULL;
	m_pScifiFloating = NULL;
	m_pColony = NULL;
	m_pCargo = NULL;
	m_pGalacticCruiser = NULL;
	m_pAncientCastle = NULL;
	m_pScifiTropical = NULL;
	m_pNaboo = NULL;

	m_pTimer = NULL;

	m_dt = 0.0;
	m_framesPerSecond = 0;
	m_frameCount = 0;
	m_elapsedTime = 0.0f;
	m_currentDistance = 0.0f;

	t_Left = 60;

	// Controls the power up rotation speed
	poUpRotation = 0.0f;
	
	darkmode = false;

	m_t = 0.0f;
	/*temp = 0;
	temp2 = 0;*/

	gameState = 1;
}

// Destructor
Game::~Game() 
{ 
	//game objects
	delete m_pCamera;
	delete m_pSkybox;
	delete m_pFtFont;
	delete m_pPlayer;
	delete m_pHeightmapTerrain;
	delete m_pHeightmapWater;
	delete m_pCatmullRom;
	delete m_pX17Viper;

	// NPC ships
	delete m_pShipFlyingCar1;
	delete m_pShipFlyingCar;
	delete m_pFighter1;
	delete m_pFighter;
	delete m_pFighter2;

	// Static meshes
	delete m_pTower;
	delete m_pScifiFloating;
	delete m_pColony;
	delete m_pCargo;
	delete m_pGalacticCruiser;
	delete m_pAncientCastle;
	delete m_pScifiTropical;
	delete m_pNaboo;

	delete m_pTimer;

	if (m_pShaderPrograms != NULL) {
		for (unsigned int i = 0; i < m_pShaderPrograms->size(); i++)
			delete (*m_pShaderPrograms)[i];
	}
	delete m_pShaderPrograms;

	//setup objects
	delete m_pHighResolutionTimer;
}

// Initialisation:  This method only runs once at startup
void Game::Initialise() 
{
	
	// Set the clear colour and depth
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClearDepth(1.0f);

	
	/// Create objects
	m_pCamera = new CCamera;
	m_pSkybox = new CSkybox;
	m_pShaderPrograms = new vector <CShaderProgram *>;
	m_pFtFont = new CFreeTypeFont;
	
	m_pHeightmapTerrain = new CHeightMapTerrain;
	m_pHeightmapWater = new CHeightMapTerrain;
	m_pPlayer = new Player;
	

	m_pShipFlyingCar1 = new ShipNPC("resources\\models\\FlyingCar\\flying_car.obj", "resources\\paths\\interceptor.txt", 0.7f, 0.2f);
	m_pShipFlyingCar = new ShipNPC("resources\\models\\FlyingCar\\flying_car.obj", "resources\\paths\\flying_car.txt", 0.2f, 0.7f);
	m_pFighter1 = new ShipNPC("resources\\models\\Fighter\\fighter1.obj", "resources\\paths\\cruiser.txt", 1.5f, 0.4f);
	m_pX17Viper = new ShipNPC("resources\\models\\X-17 Viper\\viper.obj", "resources\\paths\\x17viper.txt", 0.4f, 0.4f);
	m_pFighter = new ShipNPC("resources\\models\\Fighter\\fighter1.obj", "resources\\paths\\fighter.txt", 0.8f, 0.4f);
	m_pFighter2 = new ShipNPC("resources\\models\\Fighter2\\fighter1.obj", "resources\\paths\\fighter2.txt", 0.8f, 0.4f);

	ships.push_back(m_pShipFlyingCar1);
	ships.push_back(m_pShipFlyingCar);
	ships.push_back(m_pFighter1);
	ships.push_back(m_pX17Viper);
	ships.push_back(m_pFighter);
	ships.push_back(m_pFighter2);
	
	m_pTower = new StaticMesh("resources\\models\\tower\\Tower.obj", glm::vec3(-920.0f, 0.0f, 700.0f), glm::vec3(0,1,0), 0.0f, 40.0f);
	m_pScifiFloating = new StaticMesh("resources\\models\\Scifi Floating City\\Scifi Floating City.obj", glm::vec3(-800.0f, -50.0f, 560.0f), glm::vec3(0.0f, 1.0f, 0.0f), 255.0f, 6.4f);
	m_pColony = new StaticMesh("resources\\models\\colony\\alterian colony station.obj", glm::vec3(1000.0f, -070.0f, -2100.0f), glm::vec3(0.0f, 1.0f, 0.0f), 275.0f, 4.4f);
	m_pCargo = new StaticMesh("resources\\models\\cargo platform\\cargo platform.obj", glm::vec3(1800.0f, -70.0f, -200.0f), glm::vec3(0.0f, 1.0f, 0.0f), 255.0f, 4.4f);
	m_pGalacticCruiser = new StaticMesh("resources\\models\\Class II Gallactic Cruiser\\Class II Gallactic Cruiser.obj", glm::vec3(740, -450.0f, -680.0f), glm::vec3(0.0f, 1.0f, 0.0f), 65.0f, 4.4f);
	m_pAncientCastle = new StaticMesh("resources\\models\\eastern ancient casttle\\eastern ancient casttle.obj", glm::vec3(-1200, -50.0f, -20.0f), glm::vec3(0.0f, 1.0f, 0.0f), 65.0f, 1.0f);
	m_pScifiTropical = new StaticMesh("resources\\models\\scifi tropical city\\Sci-fi Tropical city.obj", glm::vec3(-1350, -150.0f, -1550.0f), glm::vec3(0.0f, 1.0f, 0.0f), 45.0f, 5.0f);
	m_pNaboo = new StaticMesh("resources\\models\\naboo complex\\naboo complex.obj", glm::vec3(380.0f, -50.0f, 1870.0f), glm::vec3(0.0f, 1.0f, 0.0f), 145.0f, 5.0f);

	meshes.push_back(m_pTower);
	meshes.push_back(m_pScifiFloating);
	meshes.push_back(m_pColony);
	meshes.push_back(m_pCargo);
	meshes.push_back(m_pGalacticCruiser);
	meshes.push_back(m_pAncientCastle);
	meshes.push_back(m_pScifiTropical);
	meshes.push_back(m_pNaboo);
	
	//m_pAudio = new CAudio;

	RECT dimensions = m_gameWindow.GetDimensions();

	int width = dimensions.right - dimensions.left;
	int height = dimensions.bottom - dimensions.top;

	// Set the orthographic and perspective projection matrices based on the image size
	m_pCamera->SetOrthographicProjectionMatrix(width, height); 
	m_pCamera->SetPerspectiveProjectionMatrix(90.0f, (float) width / (float) height, 1.0f, 5000.0f);

	// Load shaders
	vector<CShader> shShaders;
	vector<string> sShaderFileNames;
	sShaderFileNames.push_back("mainShader.vert");
	sShaderFileNames.push_back("mainShader.frag");
	sShaderFileNames.push_back("textShader.vert");
	sShaderFileNames.push_back("textShader.frag");
	sShaderFileNames.push_back("waterShader.vert");
	sShaderFileNames.push_back("waterShader.frag");
	sShaderFileNames.push_back("terrianShader.vert");
	sShaderFileNames.push_back("terrianShader.frag");
	sShaderFileNames.push_back("pathLightShader.vert");
	sShaderFileNames.push_back("pathLightShader.frag");

	for (int i = 0; i < (int) sShaderFileNames.size(); i++) {
		string sExt = sShaderFileNames[i].substr((int) sShaderFileNames[i].size()-4, 4);
		int iShaderType;
		if (sExt == "vert") iShaderType = GL_VERTEX_SHADER;
		else if (sExt == "frag") iShaderType = GL_FRAGMENT_SHADER;
		else if (sExt == "geom") iShaderType = GL_GEOMETRY_SHADER;
		else if (sExt == "tcnl") iShaderType = GL_TESS_CONTROL_SHADER;
		else iShaderType = GL_TESS_EVALUATION_SHADER;
		CShader shader;
		shader.LoadShader("resources\\shaders\\"+sShaderFileNames[i], iShaderType);
		shShaders.push_back(shader);
	}

	// Create the main shader program
	CShaderProgram *pMainProgram = new CShaderProgram;
	pMainProgram->CreateProgram();
	pMainProgram->AddShaderToProgram(&shShaders[0]);
	pMainProgram->AddShaderToProgram(&shShaders[1]);
	pMainProgram->LinkProgram();
	m_pShaderPrograms->push_back(pMainProgram);

	// Create a shader program for fonts
	CShaderProgram *pFontProgram = new CShaderProgram;
	pFontProgram->CreateProgram();
	pFontProgram->AddShaderToProgram(&shShaders[2]);
	pFontProgram->AddShaderToProgram(&shShaders[3]);
	pFontProgram->LinkProgram();
	m_pShaderPrograms->push_back(pFontProgram);
	
	// Create a shader program for water
	CShaderProgram *pWaterProgram = new CShaderProgram;
	pWaterProgram->CreateProgram();
	pWaterProgram->AddShaderToProgram(&shShaders[4]);
	pWaterProgram->AddShaderToProgram(&shShaders[5]);
	pWaterProgram->LinkProgram();
	m_pShaderPrograms->push_back(pWaterProgram);

	//// Create a shader program for terrian
	CShaderProgram *pTerrianProgram = new CShaderProgram;
	pTerrianProgram->CreateProgram();
	pTerrianProgram->AddShaderToProgram(&shShaders[6]);
	pTerrianProgram->AddShaderToProgram(&shShaders[7]);
	pTerrianProgram->LinkProgram();
	m_pShaderPrograms->push_back(pTerrianProgram);

	//// Create a shader program for terrian
	CShaderProgram *pPathLightProgram = new CShaderProgram;
	pPathLightProgram->CreateProgram();
	pPathLightProgram->AddShaderToProgram(&shShaders[8]);
	pPathLightProgram->AddShaderToProgram(&shShaders[9]);
	pPathLightProgram->LinkProgram();
	m_pShaderPrograms->push_back(pPathLightProgram);

	// You can follow this pattern to load additional shaders

	// Create the skybox
	// Skybox downloaded from http://www.akimbo.in/forum/viewtopic.php?f=10&t=9
	m_pSkybox->Create(1500.0f);

	m_pFtFont->LoadSystemFont("resources\\fonts\\Transformers Movie.ttf", 55);
	m_pFtFont->SetShaderProgram(pFontProgram);

	glEnable(GL_CULL_FACE);
	// Initialise audio and play background music
	//m_pAudio->Initialise();
	//m_pAudio->LoadEventSound("Resources\\Audio\\Boing.wav");					// Royalty free sound from freesound.org
	//m_pAudio->LoadMusicStream("Resources\\Audio\\DST-Garote.mp3");	// Royalty free music from http://www.nosoapradio.us/
	//m_pAudio->PlayMusicStream();

	m_pCatmullRom = new CCatmullRom();
	m_pCatmullRom->Create("resources\\textures\\metal_floor.png");
	
	m_pTimePowerUps.push_back(new CTime(glm::vec3(617.098694f, 29.0f, -1346.243042f)));
	m_pTimePowerUps.push_back(new CTime(glm::vec3(1318.526367f, 211.889145f, -225.748550f)));
	m_pTimePowerUps.push_back(new CTime(glm::vec3(-199.430939f, 55.614742f, 444.593933f)));

	m_pTimePowerUps.push_back(new CTime(glm::vec3(367.545685f, 81.775352f, -542.452881f)));
	m_pTimePowerUps.push_back(new CTime(glm::vec3(1324.618774f, 30.568798f, -816.086487f)));
	m_pTimePowerUps.push_back(new CTime(glm::vec3(1259.323730f, 79.747040f, 334.208160f)));
	m_pTimePowerUps.push_back(new CTime(glm::vec3(83.966202f, 59.454563f, 1621.918213f)));
	m_pTimePowerUps.push_back(new CTime(glm::vec3(-262.011078f, 108.178490f, 538.475586f)));
	m_pTimePowerUps.push_back(new CTime(glm::vec3(-687.137878f, 260.976196f, 353.281647f)));
	m_pTimePowerUps.push_back(new CTime(glm::vec3(-1009.467896f, 281.584656f, 32.136337f)));
	m_pTimePowerUps.push_back(new CTime(glm::vec3(-1818.126587f, 114.358147f, 158.569031f)));

	m_pSpeedDownPowerUps.push_back(new CSpeedUp(glm::vec3(-115.0f, 77.0f, -946.0f)));
	m_pSpeedDownPowerUps.push_back(new CSpeedUp(glm::vec3(-559.735596f, 84.0f, 712.939453f)));
	m_pSpeedDownPowerUps.push_back(new CSpeedUp(glm::vec3(430.008453f, 119.734566f, 1344.288086f)));
	m_pSpeedDownPowerUps.push_back(new CSpeedUp(glm::vec3(-1966.294678f, 100.037224f, -33.072556f)));
	m_pSpeedDownPowerUps.push_back(new CSpeedUp(glm::vec3(803.824463f, 140.191727f, 298.585205f)));
	m_pSpeedDownPowerUps.push_back(new CSpeedUp(glm::vec3(772.947388f, 175.939590f, -62.620174f)));
	m_pSpeedDownPowerUps.push_back(new CSpeedUp(glm::vec3(305.204895f, 67.551086f, -456.052124f)));
	m_pSpeedDownPowerUps.push_back(new CSpeedUp(glm::vec3(104.204109f, 37.055817f, -1311.876465f)));
	m_pSpeedDownPowerUps.push_back(new CSpeedUp(glm::vec3(650.419861f, 22.735188f, -1346.436768f)));
	m_pSpeedDownPowerUps.push_back(new CSpeedUp(glm::vec3(1313.296265f, 163.557877f, -371.796722f)));
	m_pSpeedDownPowerUps.push_back(new CSpeedUp(glm::vec3(1220.544922f, 82.915024f, 610.113525f)));
	m_pSpeedDownPowerUps.push_back(new CSpeedUp(glm::vec3(-108.620750f, 34.220795f, 1630.623657f)));
	m_pSpeedDownPowerUps.push_back(new CSpeedUp(glm::vec3(272.228729f, 123.503105f, 675.159546f)));
	m_pSpeedDownPowerUps.push_back(new CSpeedUp(glm::vec3(-995.305237f, 104.511559f, 1054.446777f)));
	m_pSpeedDownPowerUps.push_back(new CSpeedUp(glm::vec3(-503.425537f, 271.750671f, 592.944214f)));
	m_pSpeedDownPowerUps.push_back(new CSpeedUp(glm::vec3(-889.513062f, 332.768188f, 1156.092407f)));
	m_pSpeedDownPowerUps.push_back(new CSpeedUp(glm::vec3(-982.558472f, 270.826996f, 31.229530f)));
	m_pSpeedDownPowerUps.push_back(new CSpeedUp(glm::vec3(-1657.770142f, 177.147552f, -509.294739f)));
	m_pSpeedDownPowerUps.push_back(new CSpeedUp(glm::vec3(-1417.951172f, 97.388077f, 261.689758f)));


	m_pSpeedPowerUps.push_back(new CSpeedUp(glm::vec3(221.811005f, 164.989563f, 1096.464111f)));
	m_pSpeedPowerUps.push_back(new CSpeedUp(glm::vec3(-1245.002930f, 342.456726f, 704.326477f)));
	m_pSpeedPowerUps.push_back(new CSpeedUp(glm::vec3(-621.005249f, 98.648468f, 645.772034f)));
	m_pSpeedPowerUps.push_back(new CSpeedUp(glm::vec3(1018.948669f, 28.0f, -1266.431030f)));
	m_pSpeedPowerUps.push_back(new CSpeedUp(glm::vec3(1213.851196f, 95.870773f, 752.061157f)));
	m_pSpeedPowerUps.push_back(new CSpeedUp(glm::vec3(838.602417f, 187.261063f, -58.227177f)));
	m_pSpeedPowerUps.push_back(new CSpeedUp(glm::vec3(374.160706f, 83.347099f, -469.215363f)));
	m_pSpeedPowerUps.push_back(new CSpeedUp(glm::vec3(240.232620f, 44.064156f, -1348.266846f)));
	m_pSpeedPowerUps.push_back(new CSpeedUp(glm::vec3(1516.797974f, 21.527023f, -915.018127f)));
	m_pSpeedPowerUps.push_back(new CSpeedUp(glm::vec3(1187.669067f, 80.717537f, 335.402069f)));
	m_pSpeedPowerUps.push_back(new CSpeedUp(glm::vec3(686.230042f, 68.243969f, 895.458801f)));
	m_pSpeedPowerUps.push_back(new CSpeedUp(glm::vec3(-207.789856f, 60.520367f, 1496.035767f)));
	m_pSpeedPowerUps.push_back(new CSpeedUp(glm::vec3(333.279999f, 126.0f, 561.689758f)));
	m_pSpeedPowerUps.push_back(new CSpeedUp(glm::vec3(-1043.214233f, 116.212860f, 1115.171875f)));
	m_pSpeedPowerUps.push_back(new CSpeedUp(glm::vec3(-1120.448486f, 204.117477f, 519.432007f)));
	m_pSpeedPowerUps.push_back(new CSpeedUp(glm::vec3(-801.700989f, 345.697235f, 1103.918457f)));
	m_pSpeedPowerUps.push_back(new CSpeedUp(glm::vec3(-1178.933838f, 230.607208f, -263.558685f)));
	m_pSpeedPowerUps.push_back(new CSpeedUp(glm::vec3(-1498.998413f, 111.273865f, 248.268417f)));
	m_pSpeedPowerUps.push_back(new CSpeedUp(glm::vec3(49.132965f, 37.560455f, 657.433716f)));

	//Create  power ups
	// Time
	for (unsigned int i = 0; i < m_pTimePowerUps.size(); ++i)
	{
		m_pTimePowerUps[i]->Create("resources\\textures\\powerup\\timeup.png");
	}
	//Speed up
	for (unsigned int i = 0; i < m_pSpeedPowerUps.size(); ++i)
	{
		m_pSpeedPowerUps[i]->Create("resources\\textures\\powerup\\speedup.png");
	}
	//Speed down
	for (unsigned int i = 0; i < m_pSpeedDownPowerUps.size(); ++i)
	{
		m_pSpeedDownPowerUps[i]->Create("resources\\textures\\powerup\\speeddown.png");
	}

	//Create Player
	m_pPlayer->Create();

	// Create NPC ships
	for (unsigned int i = 0; i < ships.size(); ++i)
	{
		ships[i]->Create();
	}

	// Create Pathlights
	m_pPathLights.push_back(new PathLight(glm::vec3(617.098694f, 58.0f, -1346.243042f)));
	m_pPathLights.push_back(new PathLight(glm::vec3(-199.430939f, 83.6f, 444.593933f)));
	m_pPathLights.push_back(new PathLight(glm::vec3(367.545685f, 110.8f, -542.452881f)));
	m_pPathLights.push_back(new PathLight(glm::vec3(1324.618774f, 70.6f, -816.086487f)));
	m_pPathLights.push_back(new PathLight(glm::vec3(83.966202f, 81.5f, 1621.918213f)));
	m_pPathLights.push_back(new PathLight(glm::vec3(-687.137878f, 290.0f, 353.281647f)));
	m_pPathLights.push_back(new PathLight(glm::vec3(-115.0f, 107.0f, -946.0f)));
	m_pPathLights.push_back(new PathLight(glm::vec3(-559.735596f, 125.0f, 712.939453f)));
	m_pPathLights.push_back(new PathLight(glm::vec3(430.008453f, 153.7f, 1344.288086f)));
	m_pPathLights.push_back(new PathLight(glm::vec3(-1966.294678f, 131.0f, -33.072556f)));
	m_pPathLights.push_back(new PathLight(glm::vec3(772.947388f, 208.9f, -62.620174f)));
	m_pPathLights.push_back(new PathLight(glm::vec3(650.419861f, 51.7f, -1346.436768f)));
	m_pPathLights.push_back(new PathLight(glm::vec3(272.22872f, 167.5f, 675.159546f)));
	m_pPathLights.push_back(new PathLight(glm::vec3(-503.425537f, 300.8f, 592.944214f)));
	m_pPathLights.push_back(new PathLight(glm::vec3(-889.513062f, 372.8f, 1156.092407f)));
	m_pPathLights.push_back(new PathLight(glm::vec3(-1417.951172f, 126.4f, 261.689758f)));
	m_pPathLights.push_back(new PathLight(glm::vec3(221.811005f, 197.0f, 1096.464111f)));
	m_pPathLights.push_back(new PathLight(glm::vec3(-1245.002930f, 372.5f, 704.326477f)));
	m_pPathLights.push_back(new PathLight(glm::vec3(-621.005249f, 142.6f, 645.772034f)));
	m_pPathLights.push_back(new PathLight(glm::vec3(1018.948669f, 61.0f, -1266.431030f)));
	m_pPathLights.push_back(new PathLight(glm::vec3(838.602417f, 218.3f, -58.227177f)));
	m_pPathLights.push_back(new PathLight(glm::vec3(1516.797974f, 53.5f, -915.018127f)));
	m_pPathLights.push_back(new PathLight(glm::vec3(1187.669067f, 100.7f, 335.402069f)));
	m_pPathLights.push_back(new PathLight(glm::vec3(686.230042f, 93.2f, 895.458801f)));
	m_pPathLights.push_back(new PathLight(glm::vec3(333.279999f, 146.0f, 561.689758f)));
	m_pPathLights.push_back(new PathLight(glm::vec3(-1043.214233f, 137.2f, 1115.171875f)));
	m_pPathLights.push_back(new PathLight(glm::vec3(-801.700989f, 368.7f, 1103.918457f)));
	m_pPathLights.push_back(new PathLight(glm::vec3(-1178.933838f, 251.6f, -263.558685f)));
	m_pPathLights.push_back(new PathLight(glm::vec3(49.132965f, 57.6f, 657.433716f)));

	for (unsigned int i = 0; i < m_pPathLights.size(); ++i)
	{
		m_pPathLights[i]->Create("resources\\textures\\pathlight.png");
	}
	// Create the heightmap terrain
	m_pHeightmapTerrain->Create("resources\\textures\\terrainHeightMap200.bmp", "resources\\textures\\GrassBright.bmp", glm::vec3(0, 0, 0), 500.0f, 500.0f, 100.5f);
	m_pHeightmapWater->Create("resources\\textures\\water_hm.jpg", "resources\\textures\\water_hm.jpg", glm::vec3(0, 0, 0), 1000.0f, 1200.0f, 8.0f);

	m_pTimer = new Timer;
	m_pTimer->start();
}

// Render method runs repeatedly in a loop
void Game::Render() 
{
	// Clear the buffers and enable depth testing (z-buffering)
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// Set up a matrix stack
	glutil::MatrixStack modelViewMatrixStack;
	modelViewMatrixStack.SetIdentity();

	// Use the main shader program 
	CShaderProgram *pMainProgram = (*m_pShaderPrograms)[0];
	pMainProgram->UseProgram();
	
	pMainProgram->SetUniform("bUseTexture", true);
	pMainProgram->SetUniform("sampler0", 0);
	// Note: cubemap and non-cubemap textures should not be mixed in the same texture unit.  Setting unit 10 to be a cubemap texture.
	int cubeMapTextureUnit = 10; 
	pMainProgram->SetUniform("CubeMapTex", cubeMapTextureUnit);
	
	// Set the projection matrix
	pMainProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());

	// Call LookAt to create the view matrix and put this on the modelViewMatrix stack. 
	// Store the view matrix and the normal matrix associated with the view matrix for later (they're useful for lighting -- since lighting is done in eye coordinates)
	modelViewMatrixStack.LookAt(m_pCamera->GetPosition(), m_pCamera->GetView(), m_pCamera->GetUpVector());
	glm::mat4 viewMatrix = modelViewMatrixStack.Top();
	glm::mat3 viewNormalMatrix = m_pCamera->ComputeNormalMatrix(viewMatrix);
	
	glm::vec4 lightPosition1;
	// LIGHTINGS
	{
		// Set lighting for dark or normal mode
		if (!darkmode)
		{
			pMainProgram->SetUniform("renderSpotLights", false);
			// Set light and materials in main shader program
			lightPosition1 = glm::vec4(-2000, 100, 1000, 1); // Position of light source *in world coordinates*
			pMainProgram->SetUniform("light1.position", viewMatrix*lightPosition1); // Position of light source *in eye coordinates*
			pMainProgram->SetUniform("light1.La", glm::vec3(0.7f));		// Ambient colour of light
			pMainProgram->SetUniform("light1.Ld", glm::vec3(0.5f));		// Diffuse colour of light
			pMainProgram->SetUniform("light1.Ls", glm::vec3(0.5f));		// Specular colour of light
			pMainProgram->SetUniform("material1.Ma", glm::vec3(1.0f));	// Ambient material reflectance
			pMainProgram->SetUniform("material1.Md", glm::vec3(0.0f));	// Diffuse material reflectance
			pMainProgram->SetUniform("material1.Ms", glm::vec3(0.0f));	// Specular material reflectance
			pMainProgram->SetUniform("material1.shininess", 15.0f);		// Shininess material property
		}
		else
		{
			pMainProgram->SetUniform("renderSpotLights", true);
			// Set light and materials in main shader program
			lightPosition1 = glm::vec4(-2000, 100, -2000, 1); // Position of light source *in world coordinates*
			pMainProgram->SetUniform("light1.position", viewMatrix*lightPosition1); // Position of light source *in eye coordinates*
			pMainProgram->SetUniform("light1.La", glm::vec3(0.1f));		// Ambient colour of light
			pMainProgram->SetUniform("light1.Ld", glm::vec3(0.2f));		// Diffuse colour of light
			pMainProgram->SetUniform("light1.Ls", glm::vec3(0.2f));		// Specular colour of light
			pMainProgram->SetUniform("material1.Ma", glm::vec3(1.0f));	// Ambient material reflectance
			pMainProgram->SetUniform("material1.Md", glm::vec3(0.0f));	// Diffuse material reflectance
			pMainProgram->SetUniform("material1.Ms", glm::vec3(0.0f));	// Specular material reflectance
			pMainProgram->SetUniform("material1.shininess", 15.0f);		// Shininess material property

		}

		// set NPC ships spotlights 6! ships
		for (unsigned int i = 0; i < ships.size(); ++i)
		{
			lightPosition1 = ships[i]->GetLight()->lightPosition;
			pMainProgram->SetUniform("spotlights[" + std::to_string(i) + "].position", viewMatrix*lightPosition1); // Position of light source *in eye coordinates*
			pMainProgram->SetUniform("spotlights[" + std::to_string(i) + "].La", ships[i]->GetLight()->La);		// Ambient colour of light
			pMainProgram->SetUniform("spotlights[" + std::to_string(i) + "].Ld", ships[i]->GetLight()->Ld);		// Diffuse colour of light
			pMainProgram->SetUniform("spotlights[" + std::to_string(i) + "].Ls", ships[i]->GetLight()->Ls);		// Specular colour of light
			pMainProgram->SetUniform("spotlights[" + std::to_string(i) + "].direction", glm::normalize(viewNormalMatrix*ships[i]->GetLight()->direction));
			pMainProgram->SetUniform("spotlights[" + std::to_string(i) + "].exponent", ships[i]->GetLight()->exponent);
			pMainProgram->SetUniform("spotlights[" + std::to_string(i) + "].cutoff", ships[i]->GetLight()->cutoff);
		}

		// set Patlight spot lights 31! 
		for (unsigned int i = 0; i < m_pPathLights.size(); ++i)
		{
			lightPosition1 = glm::vec4(m_pPathLights[i]->position,1.0f);
			pMainProgram->SetUniform("spotlights[" + std::to_string(i+6) + "].position", viewMatrix*lightPosition1); // Position of light source *in eye coordinates*
			pMainProgram->SetUniform("spotlights[" + std::to_string(i+6) + "].La", glm::vec3(0.0f));		// Ambient colour of light
			pMainProgram->SetUniform("spotlights[" + std::to_string(i+6) + "].Ld", glm::vec3(1.0f));		// Diffuse colour of light
			pMainProgram->SetUniform("spotlights[" + std::to_string(i+6) + "].Ls", glm::vec3(1.0f));		// Specular colour of light
			pMainProgram->SetUniform("spotlights[" + std::to_string(i+6) + "].direction", glm::vec3(0, -1, 0));
			pMainProgram->SetUniform("spotlights[" + std::to_string(i+6) + "].exponent", 30.0f);
			pMainProgram->SetUniform("spotlights[" + std::to_string(i+6) + "].cutoff", 60.0f);
		}


		//set player's spotlight
		lightPosition1 = m_pPlayer->GetLight()->lightPosition;
		pMainProgram->SetUniform("spotlights[38].position", viewMatrix*lightPosition1); // Position of light source *in eye coordinates*
		pMainProgram->SetUniform("spotlights[38].La", m_pPlayer->GetLight()->La);		// Ambient colour of light
		pMainProgram->SetUniform("spotlights[38].Ld", m_pPlayer->GetLight()->Ld);		// Diffuse colour of light
		pMainProgram->SetUniform("spotlights[38].Ls", m_pPlayer->GetLight()->Ls);		// Specular colour of light
		pMainProgram->SetUniform("spotlights[38].direction", glm::normalize(viewNormalMatrix*m_pPlayer->GetLight()->direction));
		pMainProgram->SetUniform("spotlights[38].exponent", m_pPlayer->GetLight()->exponent);
		pMainProgram->SetUniform("spotlights[38].cutoff", m_pPlayer->GetLight()->cutoff);
	}
		
	// Render the skybox and terrain with full ambient reflectance 
	{
		modelViewMatrixStack.Push();
			pMainProgram->SetUniform("renderSkybox", true);
			// Translate the modelview matrix to the camera eye point so skybox stays centred around camera
			glm::vec3 vEye = m_pCamera->GetPosition();
			modelViewMatrixStack.Translate(vEye);
			pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
			pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
			m_pSkybox->Render(cubeMapTextureUnit);
			pMainProgram->SetUniform("renderSkybox", false);
		modelViewMatrixStack.Pop();
	}

	// Turn on diffuse + specular materials
	pMainProgram->SetUniform("material1.Ma", glm::vec3(0.5f));	// Ambient material reflectance
	pMainProgram->SetUniform("material1.Md", glm::vec3(0.5f));	// Diffuse material reflectance
	pMainProgram->SetUniform("material1.Ms", glm::vec3(1.0f));	// Specular material reflectance	

	// Rendering static meshes
	for (unsigned int i = 0; i < meshes.size(); ++i)
		{
			modelViewMatrixStack.Push();
				modelViewMatrixStack.Translate(meshes[i]->GetPosition());
				modelViewMatrixStack.Rotate(meshes[i]->GetrotVector(), meshes[i]->GetRotation());
				modelViewMatrixStack.Scale(meshes[i]->GetScale());
				pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
				pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
				meshes[i]->GetMesh()->Render();
			modelViewMatrixStack.Pop();
		}

	// Render the speed power ups
	for (unsigned int i = 0; i < m_pSpeedPowerUps.size(); ++i)
	{
		if (m_pSpeedPowerUps[i]->GetAlive())
		{
			modelViewMatrixStack.Push();
				modelViewMatrixStack.Translate(m_pSpeedPowerUps[i]->GetPosition());
				modelViewMatrixStack.Rotate(glm::vec3(1.0f, 0.0f, -1.0f), 125);
				modelViewMatrixStack.Rotate(glm::vec3(-1.0f, -1.0f, -1.0f), -poUpRotation);
				modelViewMatrixStack.Scale(20.0f);
				pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
				pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
				m_pSpeedPowerUps[i]->Render();
			modelViewMatrixStack.Pop();
		}
	}

	// Render the speed power downs
	for (unsigned int i = 0; i < m_pSpeedDownPowerUps.size(); ++i)
	{
		if (m_pSpeedDownPowerUps[i]->GetAlive())
		{
			modelViewMatrixStack.Push();
				modelViewMatrixStack.Translate(m_pSpeedDownPowerUps[i]->GetPosition());
				modelViewMatrixStack.Rotate(glm::vec3(1.0f, 0.0f, -1.0f), 305);
				modelViewMatrixStack.Rotate(glm::vec3(-1.0f, -1.0f, -1.0f), -poUpRotation);
				modelViewMatrixStack.Scale(20.0f);
				pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
				pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
				m_pSpeedDownPowerUps[i]->Render();
			modelViewMatrixStack.Pop();
		}
	}

	for (unsigned int i = 0; i < m_pTimePowerUps.size(); ++i)
	{
		if (m_pTimePowerUps[i]->GetAlive())
		{
			// Render the time power up
			modelViewMatrixStack.Push();
				//modelViewMatrixStack.Translate(glm::vec3(-20.0f, 60.0f, 10.0f));
				modelViewMatrixStack.Translate(m_pTimePowerUps[i]->position);
				modelViewMatrixStack.Rotate(glm::vec3(1.0f, 0.0f, 1.0f), 45);
				modelViewMatrixStack.Rotate(glm::vec3(1.0f, 1.0f, -1.0f), poUpRotation);
				modelViewMatrixStack.Scale(7.0f);
				pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
				pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
				m_pTimePowerUps[i]->Render();
			modelViewMatrixStack.Pop();
		}
	}
	
	// Render path
	modelViewMatrixStack.Push();
		pMainProgram->SetUniform("bUseTexture", true); // turn off texturing
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pCatmullRom->RenderTrack();
	modelViewMatrixStack.Pop();

	// Render player
	modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(m_pPlayer->GetPosition());
		modelViewMatrixStack *= m_pPlayer->GetOrientation();
		modelViewMatrixStack.Scale(0.05f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pPlayer->GetMesh()->Render();
	modelViewMatrixStack.Pop();

	// Render NPC ships
	for (unsigned int i = 0; i < ships.size(); ++i)
	{
		modelViewMatrixStack.Push();
			modelViewMatrixStack.Translate(ships[i]->GetPosition());
			modelViewMatrixStack *= ships[i]->GetOrientation();
			modelViewMatrixStack.Scale(ships[i]->GetScale());
			pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
			pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
			ships[i]->GetMesh()->Render();
		modelViewMatrixStack.Pop();
	}

	// Render path lights with their own shader
	CShaderProgram *pPathLightProgram = (*m_pShaderPrograms)[4];
	pPathLightProgram->UseProgram();
	pPathLightProgram->SetUniform("dt", m_t);
	pPathLightProgram->SetUniform("sampler0", 0);
	pPathLightProgram->SetUniform("darkmode", darkmode);
	for (unsigned int i = 0; i < m_pPathLights.size(); ++i)
	{
		modelViewMatrixStack.Push();
			modelViewMatrixStack.Translate(m_pPathLights[i]->position);
			modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), poUpRotation);
			modelViewMatrixStack.Scale(7.0f);
			pPathLightProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
			pPathLightProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
			pPathLightProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
			m_pPathLights[i]->Render();
		modelViewMatrixStack.Pop();
	}

	CShaderProgram *pWaterProgram = (*m_pShaderPrograms)[2];
	pWaterProgram->UseProgram();
	pWaterProgram->SetUniform("dt", m_t);
	pWaterProgram->SetUniform("sampler0", 0);
	pWaterProgram->SetUniform("darkmode", darkmode);
	modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(glm::vec3(0.0f, 50.0f, 0.0f));
		modelViewMatrixStack.Scale(20.0f);
		pWaterProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
		pWaterProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pWaterProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pHeightmapWater->Render();
	modelViewMatrixStack.Pop();

	// Render terrain around the lake
	CShaderProgram *pTerrianProgram = (*m_pShaderPrograms)[3];
	pTerrianProgram->UseProgram();
	lightPosition1 = glm::vec4(-2000, 100, 1000, 1); // Position of light source *in world coordinates*
	if (darkmode)
	{
		pTerrianProgram->SetUniform("light1.position", viewMatrix*lightPosition1); // Position of light source *in eye coordinates*
		pTerrianProgram->SetUniform("light1.La", glm::vec3(0.1f));		// Ambient colour of light
		pTerrianProgram->SetUniform("light1.Ld", glm::vec3(0.2f));		// Diffuse colour of light
		pTerrianProgram->SetUniform("light1.Ls", glm::vec3(0.2f));		// Specular colour of light
		pTerrianProgram->SetUniform("material1.Ma", glm::vec3(1.0f));	// Ambient material reflectance
		pTerrianProgram->SetUniform("material1.Md", glm::vec3(0.0f));	// Diffuse material reflectance
		pTerrianProgram->SetUniform("material1.Ms", glm::vec3(0.0f));	// Specular material reflectance
		pTerrianProgram->SetUniform("material1.shininess", 15.0f);		// Shininess material property
	}
	else
	{
		pTerrianProgram->SetUniform("light1.position", viewMatrix*lightPosition1); // Position of light source *in eye coordinates*
		pTerrianProgram->SetUniform("light1.La", glm::vec3(0.7f));		// Ambient colour of light
		pTerrianProgram->SetUniform("light1.Ld", glm::vec3(0.5f));		// Diffuse colour of light
		pTerrianProgram->SetUniform("light1.Ls", glm::vec3(0.5f));		// Specular colour of light
		pTerrianProgram->SetUniform("material1.Ma", glm::vec3(1.0f));	// Ambient material reflectance
		pTerrianProgram->SetUniform("material1.Md", glm::vec3(0.0f));	// Diffuse material reflectance
		pTerrianProgram->SetUniform("material1.Ms", glm::vec3(0.0f));	// Specular material reflectance
		pTerrianProgram->SetUniform("material1.shininess", 15.0f);		// Shininess material property
	}
	pTerrianProgram->SetUniform("fMaxHeight", 30.0f);
	pTerrianProgram->SetUniform("fMinHeight", 80.0f);
	pTerrianProgram->SetUniform("sampler0", 0);
	pTerrianProgram->SetUniform("sampler1", 1);
	modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(glm::vec3(70.0f, -450.0f, 0.0f));
		modelViewMatrixStack.Scale(20.0f);
		pTerrianProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
		pTerrianProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pTerrianProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		pTerrianProgram->SetUniform("matrices.invViewMatrix", glm::inverse(viewMatrix));
		m_pHeightmapTerrain->Render();
	modelViewMatrixStack.Pop();

	// Draw the 2D graphics after the 3D graphics
	DisplayGUI();

	// Swap buffers to show the rendered image
	SwapBuffers(m_gameWindow.Hdc());		
}

// Update method runs repeatedly with the Render method
void Game::Update() 
{
	poUpRotation += 0.2f * m_dt;
	if (gameState == 1)
	{
		m_pCamera->Update(m_dt, m_pCatmullRom);
		m_pPlayer->Update(m_dt, m_pCatmullRom, m_pCamera);
		checkColl();
	}
	for (unsigned int i = 0; i < ships.size(); ++i)
	{
		ships[i]->Update(m_dt);
	}
	m_t += (float)(0.01f * m_dt);
}

void Game::DisplayGUI()
{
	CShaderProgram *fontProgram = (*m_pShaderPrograms)[1];

	RECT dimensions = m_gameWindow.GetDimensions();
	int height = dimensions.bottom - dimensions.top;
	int width = dimensions.right - dimensions.left  ;

	// Increase the elapsed time and frame counter
	m_elapsedTime += m_dt;
	m_frameCount++;

	// Now we want to subtract the current time by the last time that was stored
	// to see if the time elapsed has been over a second, which means we found our FPS.
	if (m_elapsedTime > 1000)
    {
		m_elapsedTime = 0;
		m_framesPerSecond = m_frameCount;

		// Reset the frames per second
		m_frameCount = 0;
    }

	if (m_framesPerSecond > 0) {
		// Use the font shader program and render the text
		fontProgram->UseProgram();
		glDisable(GL_DEPTH_TEST);
		fontProgram->SetUniform("matrices.modelViewMatrix", glm::mat4(1));
		fontProgram->SetUniform("matrices.projMatrix", m_pCamera->GetOrthographicProjectionMatrix());
		switch (gameState) {
		case 0:
			fontProgram->SetUniform("vColour", glm::vec4(0.5f, 0.0f, 0.0f, 1.0f));
			m_pFtFont->Render(200, height / 2, 40, "Time's up! You lost!");
			break;
		case 1:
			fontProgram->SetUniform("vColour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
			m_pFtFont->Render(20, 20, 30, "FPS: %d", m_framesPerSecond);
			m_pFtFont->Render(20, height - 30, 30, "Speed: %i", (int) (m_pCamera->GetSpeed() * 100));
			m_pFtFont->Render(width - 200, height - 20, 20, "Distance left: %i", 41000 - int(m_pPlayer->GetCurrentDist()));
			m_pFtFont->Render(width - 110, height - 40, 20, "Lap: %i", int(m_pPlayer->GetCurrentDist()) / 20500 + 1);
			m_pFtFont->Render(width - 60, height - 40, 20, "/2");
			if (t_Left - m_pTimer->getTime() / 1000 < 0.5)
			{
				gameState = 0;
			}
			if (int(m_pPlayer->GetCurrentDist() > 41000))
			{
				gameState = 2;
			}

			if (t_Left - m_pTimer->getTime() / 1000 < 20)
			{
				fontProgram->SetUniform("vColour", glm::vec4(0.5f, 0.0f, 0.0f, 1.0f));
				m_pFtFont->Render(width / 2 - 20, height - 60, 80, "%d", t_Left - (m_pTimer->getTime() / 1000));
			}
			else
			{
				fontProgram->SetUniform("vColour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
				m_pFtFont->Render(width / 2 - 20, height - 60, 80, "%d", t_Left - (m_pTimer->getTime() / 1000));
			}
			break;
		case 2:
			fontProgram->SetUniform("vColour", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
			m_pFtFont->Render(280, height/2, 50, "Well done!");
			break;
		}
	}
}

// The game loop runs repeatedly until game over
void Game::GameLoop()
{
	/*
	// Fixed timer
	dDt = pHighResolutionTimer->Elapsed();
	if (dDt > 1000.0 / (double) Game::FPS) {
		pHighResolutionTimer->Start();
		Update();
		Render();
	}
	*/	
	// Variable timer
	m_pHighResolutionTimer->Start();
	Update();
	Render();
	m_dt = m_pHighResolutionTimer->Elapsed();
}

void Game::writeLog(glm::vec3 & v)
{
	float x = v.x;
	float y = v.y;
	float z = v.z;
	ofstream myfile;
	myfile.open("temp.txt", ios::app);
	// for vec3 style output
	//myfile << "glm::vec3(" << std::to_string(x) << "f, " << std::to_string(y) << "f, " << std::to_string(z) << "f);\n";
	// for floats only style output 
	myfile << "(" << std::to_string(x) << "f, " << std::to_string(y) << "f, " << std::to_string(z) << "f)\n";

	myfile.close();
}

void Game::checkColl()
{
	// Check for collision for Time power ups
	for (unsigned int i = 0; i < m_pTimePowerUps.size(); ++i)
	{
		if ((glm::distance((m_pTimePowerUps[i]->position - glm::vec3(0,5,0)), m_pPlayer->GetPosition()) < 12.0f) && m_pTimePowerUps[i]->GetAlive())
		{
			t_Left += 20;
			m_pTimePowerUps[i]->Kill();
		}
	}
	// Check for collision for Speed power ups
	for (unsigned int i = 0; i < m_pSpeedPowerUps.size(); ++i)
	{
		if ((glm::distance(m_pSpeedPowerUps[i]->GetPosition() - glm::vec3(0, 5, 0), m_pPlayer->GetPosition()) < 12.0f) && m_pSpeedPowerUps[i]->GetAlive())
		{
			m_pCamera->IncreaseSpeed(0.02f);
			m_pSpeedPowerUps[i]->Kill();
		}
	}
	// Check for collision for Speed down
	for (unsigned int i = 0; i < m_pSpeedDownPowerUps.size(); ++i)
	{
		if ((glm::distance(m_pSpeedDownPowerUps[i]->GetPosition(), m_pPlayer->GetPosition()) < 12.0f) && m_pSpeedDownPowerUps[i]->GetAlive())
		{
			m_pCamera->IncreaseSpeed(-0.02f);
			m_pSpeedDownPowerUps[i]->Kill();
		}
	}
}

WPARAM Game::Execute() 
{
	m_pHighResolutionTimer = new CHighResolutionTimer;
	m_gameWindow.Init(m_hInstance);

	if(!m_gameWindow.Hdc()) {
		return 1;
	}

	Initialise();
	m_pHighResolutionTimer->Start();
	MSG msg;
	while(1) {													
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { 
			if(msg.message == WM_QUIT) {
				break;
			}

			TranslateMessage(&msg);	
			DispatchMessage(&msg);
		} else if (m_appActive) {
			GameLoop();
		} 
		else Sleep(200); // Do not consume processor power if application isn't active
	}

	m_gameWindow.Deinit();

	return(msg.wParam);
}

int Game::GetGameState()
{
	return gameState;
}

void Game::SetGameState(unsigned int s)
{
	gameState = s;
}

LRESULT Game::ProcessEvents(HWND window,UINT message, WPARAM w_param, LPARAM l_param) 
{
	LRESULT result = 0;

	switch (message) {


	case WM_ACTIVATE:
	{
		switch(LOWORD(w_param))
		{
			case WA_ACTIVE:
			case WA_CLICKACTIVE:
				m_appActive = true;
				m_pHighResolutionTimer->Start();
				break;
			case WA_INACTIVE:
				m_appActive = false;
				break;
		}
		break;
		}

	case WM_SIZE:
			RECT dimensions;
			GetClientRect(window, &dimensions);
			m_gameWindow.SetDimensions(dimensions);
		break;

	case WM_PAINT:
		PAINTSTRUCT ps;
		BeginPaint(window, &ps);
		EndPaint(window, &ps);
		break;

	case WM_KEYDOWN:
		switch(w_param) {
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		case VK_SPACE:
			writeLog(m_pCamera->GetPosition());
			break;
		case '1':
			m_pCamera->SetCameraType(1);
			break;
		case '2':
			m_pCamera->SetCameraType(2);
			break;
		case '3':
			m_pCamera->SetCameraType(3);
			break;
		case '4':
			m_pCamera->SetCameraType(4);
			break;
		case '5':
			t_Left += 20;
			break;
		case '6':
			m_pCamera->IncreaseSpeed(-0.02f);
			break;
		case '7':
			m_pCamera->IncreaseSpeed(0.02f);
			break;
		case '0':
			darkmode = !darkmode;
			m_pSkybox->swapTexture();
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		result = DefWindowProc(window, message, w_param, l_param);
		break;
	}

	return result;
}

Game& Game::GetInstance() 
{
	static Game instance;

	return instance;
}

void Game::SetHinstance(HINSTANCE hinstance) 
{
	m_hInstance = hinstance;
}

LRESULT CALLBACK WinProc(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
	return Game::GetInstance().ProcessEvents(window, message, w_param, l_param);
}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE, PSTR, int) 
{
	Game &game = Game::GetInstance();
	game.SetHinstance(hinstance);

	return game.Execute();
}


