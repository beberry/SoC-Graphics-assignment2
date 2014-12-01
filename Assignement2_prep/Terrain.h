/**
	A primitive graphics object - terrain..

	@author Jekabs Stikanss
	@version 1.0, 30/11/2014
*/

#pragma once

/* OpenGL libs */
#include <glload/gl_4_0.h>
#include <glload/gl_load.h>

/* GLM core */
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/noise.hpp>

#include <vector>
#include <iostream>

class Terrain
{
private:
	/* Common for graphics objects. */
	std::vector<glm::vec3> vertexPositions;
	std::vector<GLuint> indices;
	std::vector<glm::vec3> vertexNormals;

	GLfloat* noiseValues;

	/* Should rename this one */
	GLfloat height_scale;
	GLfloat width;
	GLfloat height;
	//

	int drawmode;
	GLuint vertexCountX;
	GLuint vertexCountZ;
	GLuint perlin_octaves;

	GLuint bufferObject, normalsBufferObject, indiceBufferObject;

public:
	Terrain();
	~Terrain();

	void Terrain::draw();
	void Terrain::create();
	void Terrain::createVBO();
	void Terrain::calcHeight();
	void Terrain::calcNormals();
	void Terrain::calculateNoise(GLfloat freq, GLfloat scale);
	void Terrain::setDrawmode(int drawMode);
};

