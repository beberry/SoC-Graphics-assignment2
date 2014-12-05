/**
	Class which generates a 2D texture iusing Perlin noise, to
	simulate clouds.

	@author Jekabs Stikans (Some parts have been taken from the example code by Ian Martin)
	@version 1.0, 03/12/2014
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

/* Some STL includes. */
#include <vector> 

#pragma once
class CloudTexture
{
private:
	GLuint width;
	GLuint height;
	GLuint texID;

	GLubyte *noisValues;

public:
	CloudTexture();
	~CloudTexture();

	void CloudTexture::create(GLuint width, GLuint height);
	void CloudTexture::generateNoise(GLfloat frequency, GLfloat scale, GLuint octaves);
	GLuint CloudTexture::getTexID();
};

