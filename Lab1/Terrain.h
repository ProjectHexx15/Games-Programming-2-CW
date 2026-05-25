#pragma once
#include "glm/glm.hpp"
#include "PerlinNoise.h"
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "obj_loader.h"

#include <vector>

class Terrain
{

public:
	Terrain(int gridX, int gridZ, float size);
	~Terrain();

	void GenerateTerrain();
	void Draw();

	// Getters
	float getHeight(float worldX, float worldZ);
	glm::vec3 getWorldPos() const;
	glm::vec3 getPos() { return glm::vec3(gridX, 0, gridZ); }

	float SIZE = 400.0f;
	int gridX, gridZ;
	std::vector<glm::vec3> heightMap;

private:
	int VERTEX_COUNT = 128;
	float noiseOffsetX;
	float noiseOffsetZ;

	Mesh terrainMesh;

	PerlinNoise noise;


};