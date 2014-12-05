/**
	A primitive graphics object - cube.
	Some code taken from lab4 example. The code was extracted into its own
	class and new functionality was added.

	@author Ian Martin, Modified by Jekabs Stikanss
	@version 2.0, 02/12/2014
*/

#pragma once

/* OpenGL libs */
#include <glload/gl_4_0.h>
#include <glload/gl_load.h>

/* GLM core */
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

/* Some STL includes. */
#include <vector>

class Cube
{
private:
	/* Common for graphics objects. */
	std::vector<GLfloat> vertexPositions;
	std::vector<GLfloat> vertexNormals;

	GLuint bufferObject, normalsBufferObject, elementBuffer, textureBuffer, texID;

	int drawmode;
	GLuint vertexCount;

	/* Only for this object. */
	GLfloat numlats;
	GLfloat numlongs;

	GLfloat height;
	GLfloat maxTopRadius;
	GLfloat radiussCoeff; // used to multiply the bottom radius with this coeff, to get the top radius.
	std::vector<GLfloat> texCoords;


public:
	// Common for graphics objects.
	Cube();
	~Cube();

	std::vector<GLfloat>* Cube::getVertexPositions();

	void Cube::makeVBO();
	void Cube::makeUnitObject(GLuint numlats, GLuint numlongs);
	void Cube::draw();
	void Cube::setDrawmode(int drawmode);


	GLfloat angle_x, angle_y, angle_z, angle_inc_x, angle_inc_y, angle_inc_z;

	// Only for this object.
	void setVertexPositions(std::vector<GLfloat> vertexPositions);
	void setTexID(GLuint texID);
};

