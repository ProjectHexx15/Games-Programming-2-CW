#pragma once

#include "Display.h"
#include "Camera.h"
#include "Light.h"
#include "Transform.h"
#include "Mesh.h"
#include "Texture.h"
#include "Terrain.h"

#include "ModelShader.h"
#include "TerrainShader.h"

#include <vector>

enum class GameState{PLAY, EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:

	void initSystems();
	void processInput(float deltaTime);
	void gameLoop();

	void drawModels();
	void drawGame();

	void randomisePositions(int tCount,  int fCount, int rCount, int gCount);

	bool isColliding(const glm::vec3& camPos);

	float treeRadius = 3.0f;
	float rockRadius = 3.0f;
	float flowerRadius = 3.0f;
	float grassRadius = 3.5f;

	float edgeFog;

	GameState _gameState;

	Display _gameDisplay;
	Camera myCamera;
	Light light;

	ModelShader modelShader;
	TerrainShader terrainShader;

	Mesh Tree;
	Mesh Flower;
	Mesh Rock;
	Mesh Grass;

	Texture treeTexture;
	Texture grassTexture;
	Texture flowerTexture;
	Texture rockTexture;

	Texture terrainTexture;
	Texture flowerTerrainTexture;
	Texture pathTexture;
	Texture brickTexture;
	Texture blendMapTexture;

	Terrain* terrain = nullptr;
	Transform terrainTransform;

	std::vector<Transform> treeTransforms;
	std::vector<Transform> flowerTransforms;
	std::vector<Transform> rockTransforms;
	std::vector<Transform> grassTransforms;


};

