#include "Mesh.h"
#include <vector>
#include <iostream>

void Mesh::init(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices)
{
	IndexedModel model;

	// loop through each vertex and push model, normal and texcoord positions
	for (unsigned int i = 0; i < numVertices; i++)
	{
		model.positions.push_back(*vertices[i].GetPos());
		model.normals.push_back(*vertices[i].GetNormal());
		model.texCoords.push_back(*vertices[i].GetTexCoord());
	}

	// loop through and store each indice
	for (unsigned int i = 0; i < numIndices; i++)
	{
		model.indices.push_back(indices[i]);
	}
		
	initModel(model);
}

void Mesh::initModel(const IndexedModel& model)
{
	drawCount = model.indices.size();

	// creates and bind VAO
	glGenVertexArrays(1, &vertexArrayObject); 
	glBindVertexArray(vertexArrayObject); 

	// generates buffers (pos, texcoord, normal, index)
	glGenBuffers(NUM_BUFFERS, vertexArrayBuffers); 
	
	// uploads all vertex positions to the gpu
	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[0]); 
	glBufferData(GL_ARRAY_BUFFER, model.positions.size() * sizeof(model.positions[0]), &model.positions[0], GL_STATIC_DRAW); 
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// uploads normals
	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.normals[0]) * model.normals.size(), &model.normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// uploads texture coordinates
	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[TEXCOORD_VB]); 
	glBufferData(GL_ARRAY_BUFFER, model.texCoords.size() * sizeof(model.texCoords[0]), &model.texCoords[0], GL_STATIC_DRAW); 
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// uploads the index list
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexArrayBuffers[INDEX_VB]); 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.indices.size() * sizeof(model.indices[0]), &model.indices[0], GL_STATIC_DRAW); 

	// stop editing the VAO
	glBindVertexArray(0); 
}

Mesh::Mesh()
{
	drawCount = 0;
}

void Mesh::loadModel(const std::string& filename)
{
	// loads an object file, converst to an indexed model
	IndexedModel model = OBJModel(filename).ToIndexedModel();
	initModel(model);
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &vertexArrayObject); 
}

void Mesh::draw()
{
	// binds th VAO
	glBindVertexArray(vertexArrayObject);
	// draw the indexed triangles
	glDrawElements(GL_TRIANGLES, drawCount, GL_UNSIGNED_INT, 0);
	// unbinds the VAO
	glBindVertexArray(0);
}

