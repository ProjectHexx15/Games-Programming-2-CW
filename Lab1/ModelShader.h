#pragma once
#include "Shader.h"

// Inherits from shader, so that the shaders can have different uniforms
class ModelShader : public Shader
{
public:
	ModelShader() = default;

protected:
	void bindAttributes() override
	{
		glBindAttribLocation(shaderID, 0, "VertexPosition");
		glBindAttribLocation(shaderID, 1, "VertexNormal");
		glBindAttribLocation(shaderID, 2, "VertexTexCoord");
	}

	void getAllUniformLocations() override
	{
		_model = getUniform("model");
		_view = getUniform("view");
		_projection = getUniform("projection");

		_lightPos = getUniform("lightPos");
		_lightColor = getUniform("lightColor");

		_modelTexture = getUniform("modelTexture");
		_cameraPos = getUniform("cameraPos");
		_fogColor = getUniform("fogColor");

		_ambientStrength = getUniform("ambientStrength");
		_specularStrength = getUniform("specularStrength");
		_shininess = getUniform("shininess");

		_edgeFogStrength = getUniform("edgeFogStrength");

	}

public:
	GLint _model;
	GLint _view;
	GLint _projection;

	GLint _lightPos;
	GLint _lightColor;
	GLint _modelTexture;
	GLint _cameraPos;
	GLint _fogColor;

	GLint _ambientStrength;
	GLint _specularStrength;
	GLint _shininess;

	GLint _edgeFogStrength;

};