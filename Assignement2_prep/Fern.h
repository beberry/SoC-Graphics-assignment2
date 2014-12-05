/**
	A fractal graphics object - fern.
	Using parts of the example code by Ian Martin.

	@author Jekabs Stikanss
	@version 1.0, 04/12/2014
*/
#pragma once

/* OpenGL libs */
#include <glload/gl_4_0.h>
#include <glload/gl_load.h>

/* GLM core */
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <stack>

class Fern
{
private:
	std::vector<glm::vec3> vertexPositions;
	std::string lSystem;
	std::stack<glm::mat4> transformations;
	GLuint modelID, bufferObject, normalsBufferObject, elementBuffer, textureBuffer, texID;
	glm::mat4 model;
	int drawmode;
	int level;

	GLfloat xAngle, yAngle;

public:
	Fern(GLuint modelID, int level);
	~Fern();

	void Fern::create();
	void Fern::makeVBO();
	void Fern::make(int level, int leaveCount);
	void Fern::draw();
	void Fern::drawBranch(int leaf);
	void Fern::setDrawmode(int drawmode);
};

