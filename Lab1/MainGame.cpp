#include "MainGame.h"
#include <SDL/SDL.h>
#include <direct.h>


MainGame::MainGame()
{
	_gameState = GameState::PLAY;
}

MainGame::~MainGame()
{
	delete terrain;
}

void MainGame::run()
{
	// Initialize the game, start main loop
	initSystems(); 
	gameLoop();
}


void MainGame::initSystems()
{
	_gameDisplay.initDisplay(); 

	// Initialize the light
	light.setPosition(200, 300, 200);
	light.setColor(0.55f, 0.65f, 1.0f);

	// Load the object models
	Tree.loadModel("..\\res\\Tree.obj");
	Flower.loadModel("..\\res\\Flower.obj");
	Rock.loadModel("..\\res\\Rock.obj");
	Grass.loadModel("..\\res\\Grass.obj");

	// Load model shader
	modelShader.init("..\\res\\model.vert", "..\\res\\model.frag");
	modelShader.setFloat("ambientStrength", 0.3f);
	modelShader.setFloat("specularStrength", 0.5f);
	modelShader.setFloat("shininess", 32.0f);      

	// Load terrain shader
	terrainShader.init("..\\res\\terrain.vert", "..\\res\\terrain.frag");

	terrainShader.Bind();

	terrainShader.setInt("terrainTexture", 0);
	terrainShader.setInt("flowerTerrainTexture", 1);
	terrainShader.setInt("pathTexture", 2);
	terrainShader.setInt("brickTexture", 3);
	terrainShader.setInt("blendMapTexture", 4);

	terrainShader.setFloat("ambientStrength", 0.6f);

	// Load model textures
	treeTexture.init("..\\res\\TreeTexture.jpg");
	flowerTexture.init("..\\res\\FlowerTexture.jpg");
	rockTexture.init("..\\res\\RockTexture.jpg");
	grassTexture.init("..\\res\\GrassTexture.jpg");

	// Load terrain Textures
	terrainTexture.init("..\\res\\TerrainTexture.jpg");
	flowerTerrainTexture.init("..\\res\\terrainFlowerTexture.jpg");
	pathTexture.init("..\\res\\TerrainPath.jpg");
	brickTexture.init("..\\res\\TerrainBrick.jpg");
	blendMapTexture.init("..\\res\\BlendMap.jpg");

	// Initialize the terrain
	terrain = new Terrain(0, 0, 400.0f);
	terrain->GenerateTerrain();

	terrainTransform.SetPos(glm::vec3(0, 0, 0));
	terrainTransform.SetRot(glm::vec3(0, 0, 0));
	terrainTransform.SetScale(glm::vec3(1, 1, 1));

	myCamera.initCamera(glm::vec3(200, 10, 200), 70.0f, (float)_gameDisplay.getWidth() / _gameDisplay.getHeight(), 0.01f, 1000.0f);

	// Hide and lock the mouse
	SDL_SetRelativeMouseMode(SDL_TRUE);

	// place objects randomly on the terrain
	randomisePositions(200, 200, 200, 200);
}

void MainGame::randomisePositions(int tCount, int fCount, int rCount, int gCount)
{
	// Clear existing positions
	treeTransforms.clear();
	flowerTransforms.clear();
	rockTransforms.clear();
	grassTransforms.clear();

	glm::vec3 terrainPos = terrain->getPos();
	float terrainSize = terrain->SIZE;

	// spwan objects within the terrain bounds
	auto spawnObject = [&](std::vector<Transform>& list)
	{
		Transform t;

		float x = terrainPos.x + (rand() % (int)terrainSize);
		float z = terrainPos.z + (rand() % (int)terrainSize);

		float y = terrain->getHeight(x, z);

		t.SetPos(glm::vec3(x, y, z));
		list.push_back(t);
	};

	// Loop for each object type
	for (int i = 0; i < tCount; i++)
	{
		spawnObject(treeTransforms);
	}

	for (int i = 0; i < fCount; i++)
	{
		spawnObject(flowerTransforms);
	}

	for (int i = 0; i < rCount; i++)
	{
		spawnObject(rockTransforms);
	}

	for (int i = 0; i < gCount; i++)
	{
		spawnObject(grassTransforms);
	}
}


void MainGame::gameLoop()
{
	long previousTime = _gameDisplay.getCurrentTime();

	// Run the game until the player quits, presses escape key
	while (_gameState != GameState::EXIT)
	{
		// calculate delta time to use in movement speed
		long currentTime = _gameDisplay.getCurrentTime();
		float deltaTime = (currentTime - previousTime) / 1000.0f;

		if (deltaTime < 0.0f) deltaTime = 0.0f;
		previousTime = currentTime;

		processInput(deltaTime);
		drawGame();
	}
}

void MainGame::processInput(float deltaTime)
{
	SDL_Event evnt;

	while(SDL_PollEvent(&evnt))
	{
		switch (evnt.type)
		{
		case SDL_QUIT:
		{
			_gameState = GameState::EXIT;
			break;
		}

		case SDL_MOUSEMOTION:
		{

			// mouse movement
			const float sensitivity = 0.1f; 

			float dx = evnt.motion.xrel * sensitivity;
			float dy = evnt.motion.yrel * sensitivity;

			myCamera.RotateY(-dx);

			myCamera.Pitch(-dy);

			break;

		}

		case SDL_KEYDOWN:
		{
			if (!evnt.key.repeat)
			{
				if (evnt.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
				{
					_gameState = GameState::EXIT;
				}

				if (evnt.key.keysym.scancode == SDL_SCANCODE_R)
				{
					// regenerate the terrain and objects
					delete terrain;
					terrain = new Terrain(0, 0, 400.0f);
					terrain->GenerateTerrain();
					randomisePositions(200, 200, 200, 200);
				}
			}

			break;
		}

		}

	}

	// retrieves current state of every key on keyboard
	const Uint8* keyState = SDL_GetKeyboardState(NULL);
	const float speed = 2500.0f;

	// checks if key is being held - move camera if so in respective direction, check collision with objects
	if (keyState[SDL_SCANCODE_W])
	{
		glm::vec3 oldPos = myCamera.getPos();
		myCamera.MoveForward(speed * deltaTime);

		if (isColliding(myCamera.getPos()))
		{
			myCamera.setPos(oldPos); 
		}
	}

	if (keyState[SDL_SCANCODE_S])
	{
		glm::vec3 oldPos = myCamera.getPos();
		myCamera.MoveBackward(speed * deltaTime);

		if (isColliding(myCamera.getPos()))
		{
			myCamera.setPos(oldPos);
		}
	}

	if (keyState[SDL_SCANCODE_A])
	{
		glm::vec3 oldPos = myCamera.getPos();
		myCamera.MoveLeft(speed * deltaTime);

		if (isColliding(myCamera.getPos()))
		{
			myCamera.setPos(oldPos);
		}
	}

	if (keyState[SDL_SCANCODE_D])
	{
		glm::vec3 oldPos = myCamera.getPos();
		myCamera.MoveRight(speed * deltaTime);

		if (isColliding(myCamera.getPos()))
		{
			myCamera.setPos(oldPos);
		}
	}

	// Clamp camera to terrain bounds
	glm::vec3 camPos = myCamera.getPos();

	float minX = terrain->getPos().x;
	float maxX = terrain->getPos().x + terrain->SIZE;

	float minZ = terrain->getPos().z;
	float maxZ = terrain->getPos().z + terrain->SIZE;

	camPos.x = glm::clamp(camPos.x, minX, maxX);
	camPos.z = glm::clamp(camPos.z, minZ, maxZ);

	myCamera.setPos(camPos);

	float terrainY = terrain->getHeight(camPos.x, camPos.z);
	camPos.y = terrainY + 2.0f;

	myCamera.setPos(camPos);

}


void MainGame::drawGame()
{
	// set clear color to a dark blue
	_gameDisplay.clearDisplay(0.0157f, 0.1020f, 0.2510f, 1.0f);

	glm::mat4 view = myCamera.getView();
	glm::mat4 projection = myCamera.getProjection();

	// Bind and send values to terrain shader
	terrainShader.Bind();
	terrainShader.setMat4("model", terrainTransform.GetModel());
	terrainShader.setMat4("view", view);
	terrainShader.setMat4("projection", projection);
	terrainShader.setVec3("cameraPos", myCamera.getPos());
	terrainShader.setVec3("fogColor", glm::vec3(0.0157f, 0.1020f, 0.2510f));  // dark bluey fog

	terrainShader.setFloat("uvGrass", 100.0f);
	terrainShader.setFloat("uvFlower", 100.0f);
	terrainShader.setFloat("uvPath", 100.0f);
	terrainShader.setFloat("uvBrick", 100.0f);
	terrainShader.setFloat("ambientStrength", 0.08f);

	// Bind each terrain texture
	glActiveTexture(GL_TEXTURE0);
	terrainTexture.Bind(0);

	glActiveTexture(GL_TEXTURE1);
	flowerTerrainTexture.Bind(1);

	glActiveTexture(GL_TEXTURE2);
	pathTexture.Bind(2);

	glActiveTexture(GL_TEXTURE3);
	brickTexture.Bind(3);

	glActiveTexture(GL_TEXTURE4);
	blendMapTexture.Bind(4);

	terrain->Draw();

	// increase fog when reaching an edge
	glm::vec3 cam = myCamera.getPos();

	float minX = terrain->getPos().x;
	float maxX = terrain->getPos().x + terrain->SIZE;
	float minZ = terrain->getPos().z;
	float maxZ = terrain->getPos().z + terrain->SIZE;

	float distToEdge = std::min({ cam.x - minX, maxX - cam.x, cam.z - minZ, maxZ - cam.z });

	float fadeStart = 120.0f;
	edgeFog = 1.0f - glm::clamp(distToEdge / fadeStart, 0.0f, 1.0f);

	terrainShader.setFloat("edgeFogStrength", edgeFog);
	modelShader.setFloat("edgeFogStrength", edgeFog);

	modelShader.Bind();
	modelShader.setMat4("view", view);
	modelShader.setMat4("projection", projection);
	modelShader.setVec3("cameraPos", myCamera.getPos());
	modelShader.setVec3("lightPos", light.getPosition());
	modelShader.setVec3("lightColor", light.getColor());
	modelShader.setVec3("fogColor", glm::vec3(0.0157f, 0.1020f, 0.2510f)); // dark bluey fog

	drawModels();

	_gameDisplay.swapBuffer();
} 

void MainGame::drawModels()
{
	// Draw Trees
	glActiveTexture(GL_TEXTURE0);
	treeTexture.Bind(0);
	modelShader.setInt("modelTexture", 0);
	modelShader.setFloat("ambientStrength", 0.08f);

	for (auto& t : treeTransforms)
	{
		modelShader.setMat4("model", t.GetModel());
		modelShader.setFloat("edgeFogStrength", edgeFog);
		Tree.draw();
	}

	// Draw Flowers
	glActiveTexture(GL_TEXTURE0);
	flowerTexture.Bind(0);
	modelShader.setInt("modelTexture", 0);

	for (auto& t : flowerTransforms)
	{
		modelShader.setMat4("model", t.GetModel());
		modelShader.setFloat("edgeFogStrength", edgeFog);
		Flower.draw();
	}

	// Draw Rocks
	glActiveTexture(GL_TEXTURE0);
	rockTexture.Bind(0);
	modelShader.setInt("modelTexture", 0);

	for (auto& t : rockTransforms)
	{
		modelShader.setMat4("model", t.GetModel());
		modelShader.setFloat("edgeFogStrength", edgeFog);
		Rock.draw();
	}

	// Draw Grass
	glActiveTexture(GL_TEXTURE0);
	grassTexture.Bind(0);
	modelShader.setInt("modelTexture", 0);

	for (auto& t : grassTransforms)
	{
		modelShader.setMat4("model", t.GetModel());
		modelShader.setFloat("edgeFogStrength", edgeFog);
		Grass.draw();
	}

}

bool MainGame::isColliding(const glm::vec3& camPos)
{
	// Check collisions for each object, camera moves back a space if they collide
	for (auto& t : treeTransforms)
	{
		if (glm::distance(camPos, t.GetPos()) < treeRadius)
		{
			return true;
		}
	}

	for (auto& t : rockTransforms)
	{
		if (glm::distance(camPos, t.GetPos()) < rockRadius)
		{
			return true;
		}
			
	}

	for (auto& t : flowerTransforms)
	{
		if (glm::distance(camPos, t.GetPos()) < flowerRadius)
		{
			return true;
		}

	}

	for (auto& t : grassTransforms)
	{
		if (glm::distance(camPos, t.GetPos()) < grassRadius)
		{
			return true;
		}
	}

	return false;

}