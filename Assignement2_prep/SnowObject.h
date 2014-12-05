/**
	Snow particle animation class.

	@author Jekabs Stikans  (Some parts have been taken from the example code by Ian Martin)
	@version 1.0, 1/12/2014
*/

#pragma once

/* OpenGL libs */
#include <glload/gl_4_0.h>
#include <glload/gl_load.h>
#include <GLFW/glfw3.h>

/* GLM core */
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/gtc/random.hpp>

#include <vector>
#include <iostream>
#include <algorithm>

#pragma once

struct SnowFlake
{
	glm::vec3 pos, speed;
	unsigned char r, g, b, a; // Color
	float size, angle, weight;
	float life; // Remaining life of the particle. if < 0 : dead and unused.

	float cameradistance; // *Squared* distance to the camera. if dead : -1.0f

	// Whether the particle has landed on the terrain. Implemented by Jekabs
	bool landed = false;


	bool operator<(const SnowFlake& that) const {
		// Sort in reverse order : far particles drawn first.
		return this->cameradistance > that.cameradistance;
	}
};

class SnowObject
{
	public:
	SnowObject();
	~SnowObject();

	GLfloat* noiseValues;
	GLuint terrainVertexCountX;
	GLuint terrainVertexCountZ;
	GLfloat terrainWidth;
	GLfloat terrainHeight;

	void SnowObject::create(GLuint program, GLfloat* noiseValues, GLuint terrainVertexCountX, GLuint terrainVertexCountZ, GLfloat terrainWidth, GLfloat terrainHeight);
	int FindUnusedParticle();
	void SortParticles();
	void drawParticles(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix);
	void defineUniforms();
	int SnowObject::findNoiseLocation(GLfloat x, GLfloat z);

	GLuint billboard_vertex_buffer;
	GLuint particles_position_buffer;
	GLuint particles_color_buffer;
	const int MaxParticles = 5500;
	SnowFlake ParticlesContainer[5500];

	std::vector<SnowFlake> ParticlesContainer2;

	int LastUsedParticle;
	GLfloat* g_particule_position_size_data;
	GLubyte* g_particule_color_data;
	int ParticlesCount;
	double lastTime;

	GLuint VertexArrayID;
	GLuint programID;
	GLuint Texture;
	GLuint TextureID;
	GLuint CameraRight_worldspace_ID;
	GLuint CameraUp_worldspace_ID;
	GLuint ViewProjMatrixID;
};
