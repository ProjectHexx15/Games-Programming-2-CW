#include "Terrain.h"

Terrain::Terrain(int gridX, int gridZ, float size)
{
	this->gridX = gridX;
	this->gridZ = gridZ;
	this->SIZE = size;

	noise = PerlinNoise();
	noiseOffsetX = rand() % 10000;
	noiseOffsetZ = rand() % 10000;
}

Terrain::~Terrain()
{
}

void Terrain::GenerateTerrain()
{
	IndexedModel model;

	// reserve space for vertices, texcoords, and normals
	model.positions.reserve(VERTEX_COUNT * VERTEX_COUNT);
	model.texCoords.reserve(VERTEX_COUNT * VERTEX_COUNT);
	model.normals.resize(VERTEX_COUNT * VERTEX_COUNT, glm::vec3(0, 1, 0));

	// loop through each vertex in the grid
	for(int z = 0; z < VERTEX_COUNT; z++)
	{
		for(int x = 0; x < VERTEX_COUNT; x++)
		{
			// Calculate world coordinates for the vertex
			float worldX = (float)x / ((float)VERTEX_COUNT - 1) * SIZE;
			float worldZ = (float)z / ((float)VERTEX_COUNT - 1) * SIZE;

			// Use Perlin noise to calculate height based on world coordinates
			float height = noise.noise((worldX + noiseOffsetX) * 0.01f, (worldZ + noiseOffsetZ) * 0.01f) * 3.0f;

			// Store the vertex position and texture coordinate
			model.positions.push_back(glm::vec3(worldX, height, worldZ));

			// push uv tex coords, scaled down to try prevent stretching
			model.texCoords.push_back(glm::vec2(worldX / 4.0f, worldZ / 4.0f));
		}
	}
		// retrive height, for setting normals
		auto sampleHeight = [&](int x, int z)
		{
			x = glm::clamp(x, 0, VERTEX_COUNT - 2);
			z = glm::clamp(z, 0, VERTEX_COUNT - 2);
			return model.positions[z * VERTEX_COUNT + x].y;
		};

	for(int z = 0; z < VERTEX_COUNT - 1; z++)
	{
		for(int x = 0; x < VERTEX_COUNT - 1; x++)
		{
			// Calculate the indices for the two triangles that make up the current grid square
			int topLeft = (z * VERTEX_COUNT) + x;
			int topRight = topLeft + 1;
			int bottomLeft = ((z + 1) * VERTEX_COUNT) + x;
			int bottomRight = bottomLeft + 1;

			// First triangle (topLeft, bottomLeft, topRight)
			model.indices.push_back(topLeft);
			model.indices.push_back(bottomLeft);
			model.indices.push_back(topRight);

			// Second triangle (topRight, bottomLeft, bottomRight)
			model.indices.push_back(topRight);
			model.indices.push_back(bottomLeft);
			model.indices.push_back(bottomRight);
		}
	}

	for (int z = 0; z < VERTEX_COUNT; z++)
	{
		for(int x = 0; x < VERTEX_COUNT; x++)
		{
			float heightL = sampleHeight(x - 1, z);
			float heightR = sampleHeight(x + 1, z);
			float heightD = sampleHeight(x, z - 1);
			float heightU = sampleHeight(x, z + 1);

			glm::vec3 normal(heightL - heightR, 2.0f, heightD - heightU);
			normal = glm::normalize(normal);

			model.normals[z * VERTEX_COUNT + x] = normal;
		}
	}

	heightMap = model.positions;

	terrainMesh.initModel(model);

}

void Terrain::Draw()
{
	terrainMesh.draw();
}


// get world coord height
float Terrain::getHeight(float worldX, float worldZ)
{
	float terrainX = worldX - gridX;
	float terrainZ = worldZ - gridZ;

	float gridSquareSize = SIZE / (VERTEX_COUNT - 1);

	int x = (int)(terrainX / gridSquareSize);
	int z = (int)(terrainZ / gridSquareSize);

	x = glm::clamp(x, 0, VERTEX_COUNT - 1);
	z = glm::clamp(z, 0, VERTEX_COUNT - 1);

	return heightMap[z * VERTEX_COUNT + x].y;
}

glm::vec3 Terrain::getWorldPos() const
{
	return glm::vec3(gridX, 0, gridZ);
}




