/**
	A fractal graphics object - fern.

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

class Fern
{
private:
	std::vector<glm::vec3> vertexPositions;
	/**
	A primitive graphics object - sphere.
	Some code taken from lab4 example. The code was extracted into its own
	class and new functionality was added.

	It is possible to define whether the sphere needs to be full / half-sphere.
	It is possible to define the radiuss of the sphere object.

	@author Ian Martin, Modified by Jekabs Stikanss
	@version 1.0, 03/11/2014
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

public:
	Fern();
	~Fern();

	void Fern::create();
	void Fern::makeVBO();
	void Fern::draw();
};

