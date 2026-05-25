#pragma once
#include "Shader.h"

// Inherits from shader, allows the terrain to use multitexturing without messing up uniform stuff
class TerrainShader : public Shader
{
public:
	TerrainShader() = default;


protected:
	void bindAttributes() override
	{
		glBindAttribLocation(shaderID, 0, "VertexPosition");
		glBindAttribLocation(shaderID, 1, "VertexTexCoord");
	}

	void getAllUniformLocations() override
	{
		_model = getUniform("model");
		_view = getUniform("view");
		_cameraPos = getUniform("cameraPos");

		_fogColor = getUniform("fogColor");
		_terrainTexture = getUniform("terrainTexture");
		_flowerTerrainTexture = getUniform("flowerTerrainTexture");
		_pathTexture = getUniform("pathTexture");
		_brickTexture = getUniform("blendMapTexture");

		_edgeFogStrength = getUniform("edgeFogStrength");

	}

public:
	GLint _model;
	GLint _view;
	GLint _cameraPos;
	GLint _fogColor;
	GLint _terrainTexture;
	GLint _flowerTerrainTexture;
	GLint _pathTexture;
	GLint _brickTexture;
	GLint _blendMapTexture;
	GLint _edgeFogStrength;


};