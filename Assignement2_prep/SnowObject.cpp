#include "SnowObject.h"
#include "soil.h"


SnowObject::SnowObject()
{
	LastUsedParticle = 0;

	srand(time(0));

}


SnowObject::~SnowObject()
{
}

void SnowObject::SortParticles(){
	std::sort(&ParticlesContainer[0], &ParticlesContainer[MaxParticles]);
}


void SnowObject::create(GLuint program, GLfloat* noiseValues, GLuint terrainVertexCountX, GLuint terrainVertexCountZ, GLfloat terrainWidth, GLfloat terrainHeight)
{
	this->terrainVertexCountX = terrainVertexCountX;
	this->terrainVertexCountZ = terrainVertexCountZ;
	this->terrainWidth  = terrainWidth;
	this->terrainHeight = terrainHeight;
	this->noiseValues = noiseValues;

	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	programID = program;
	glUseProgram(programID);

	// The VBO containing the 4 vertices of the particles.
	// Thanks to instancing, they will be shared by all particles.
	static const GLfloat g_vertex_buffer_data[] = {
		-0.5f, -0.5f, 0.0f,
	};

	g_particule_position_size_data = new GLfloat[MaxParticles * 4];
	g_particule_color_data = new GLubyte[MaxParticles * 4];

	for (int i = 0; i<MaxParticles; i++){
		ParticlesContainer[i].life = -1.0f;
		ParticlesContainer[i].cameradistance = -1.0f;
	}

	glGenBuffers(1, &billboard_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	// The VBO containing the positions and sizes of the particles
	glGenBuffers(1, &particles_position_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	// The VBO containing the colors of the particles

	glGenBuffers(1, &particles_color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);

	// Vertex shader
	GLuint CameraRight_worldspace_ID = glGetUniformLocation(programID, "CameraRight_worldspace");
	GLuint CameraUp_worldspace_ID = glGetUniformLocation(programID, "CameraUp_worldspace");
	GLuint ViewProjMatrixID = glGetUniformLocation(programID, "VP");

	/* load an image file directly as a new OpenGL texture */
	Texture = SOIL_load_OGL_texture("circle.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);

	/* check for an error during the load process */
	if (Texture == 0)
	{
		printf("TexID SOIL loading error: '%s'\n", SOIL_last_result());
	}

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	/* Define the uniform variables */
	defineUniforms();

	lastTime = 0;
}


/* Update the particle animation and draw the particles */
void SnowObject::drawParticles(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix)
{

	double currentTime = glfwGetTime();
	if (lastTime == 0)
	{
		lastTime = currentTime;
	}

	double delta = currentTime - lastTime;
	lastTime = currentTime;

	glBindVertexArray(VertexArrayID);

	// We will need the camera's position in order to sort the particles
	// w.r.t the camera's distance.
	glm::vec3 CameraPosition(glm::inverse(ViewMatrix)[3]);
	glm::mat4 ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;

	// Generate 10 new particule each millisecond,
	// but limit this to 16 ms (60 fps), or if you have 1 long frame (1sec),
	// newparticles will be huge and the next frame even longer.
	int newparticles = (int)(delta*1000.0);
	if (newparticles > (int)(0.003f*10000.0))
		newparticles = (int)(0.003f*10000.0);

	for (int i = 0; i<newparticles; i++){
		int particleIndex = FindUnusedParticle();

		if (particleIndex < 0)
		{
			break;
		}
		
		// written by me.
		if (ParticlesContainer[particleIndex].landed == true)
		{
			// Beffore incrementing the terrain height check whether this particle has landed already..


			int noiseIndex = this->findNoiseLocation(ParticlesContainer[particleIndex].pos.x, ParticlesContainer[particleIndex].pos.z);

			if (noiseIndex >= 0)
			{
				this->noiseValues[noiseIndex] -= ParticlesContainer[particleIndex].size;
			}
		}

		ParticlesContainer[particleIndex].speed = glm::vec3(0, 0, 0);
		ParticlesContainer[particleIndex].landed = false;
		ParticlesContainer[particleIndex].life = glm::linearRand(20.0f, 240.0f); // This particle will live 5 seconds.
		ParticlesContainer[particleIndex].pos = glm::vec3(glm::linearRand(-1 * this->terrainWidth / 2, this->terrainWidth / 2), glm::linearRand(2.6f, 4.4f), glm::linearRand(-1 * this->terrainHeight / 2, this->terrainHeight / 2));


		float spread = 6.5f;
		glm::vec3 maindir = glm::vec3(0.0f, 100.0f, 0.0f);
		// Very bad way to generate a random direction; 
		// See for instance http://stackoverflow.com/questions/5408276/python-uniform-spherical-distribution instead,
		// combined with some user-controlled parameters (main direction, spread, etc)
		glm::vec3 randomdir = glm::vec3(
			(rand() % (int)this->terrainWidth - this->terrainWidth / 2) / (this->terrainWidth / 2),
			0.3f,
			(rand() % (int)this->terrainHeight - this->terrainHeight / 2) / (this->terrainHeight / 2)
			);

		ParticlesContainer[particleIndex].speed = maindir + randomdir*spread;

		if (ParticlesContainer[particleIndex].speed.y <= 0)
		{
			int kkkz = 0;
		}


		// Very bad way to generate a random color
		ParticlesContainer[particleIndex].r = rand() % 256;
		ParticlesContainer[particleIndex].g = rand() % 256;
		ParticlesContainer[particleIndex].b = rand() % 256;
		ParticlesContainer[particleIndex].a = (rand() % 256) / 3;

		ParticlesContainer[particleIndex].size = glm::linearRand(0.003f, 0.008f);
	}

	// Simulate all particles
	int ParticlesCount = 0;
	for (int i = 0; i<MaxParticles; i++){

		SnowFlake& p = ParticlesContainer[i]; // shortcut

		if (p.life > 0.0f){

			// Decrease life
			p.life -= delta;
			
			GLfloat terrainHeight = -100; // Not the best thing to do.
			
			int noiseIndex = this->findNoiseLocation(p.pos.x, p.pos.z);

			if (noiseIndex >= 0)
			{
				terrainHeight = this->noiseValues[noiseIndex];
			}
				
			if (terrainHeight-0.988 < p.pos.y && p.landed == false)
			{

				if (p.life > 0.0f){

					// Simulate simple physics : gravity only, no collisions
					p.speed = glm::vec3(0.0f, -9.81f, 0.0f) * (float)delta * 0.5f;
					p.pos += p.speed * (float)delta;

					if (p.pos.y <= 0)
					{
						int kkkz = 0;

					}
					p.cameradistance = glm::length(p.pos - CameraPosition);

					if (p.cameradistance < 1.0)
					{
						p.cameradistance = -1.0f;
					}

					//ParticlesContainer[i].pos += glm::vec3(0.0f,10.0f, 0.0f) * (float)delta;

					// Fill the GPU buffer
					g_particule_position_size_data[4 * ParticlesCount + 0] = p.pos.x;
					g_particule_position_size_data[4 * ParticlesCount + 1] = p.pos.y;
					g_particule_position_size_data[4 * ParticlesCount + 2] = p.pos.z;

					g_particule_position_size_data[4 * ParticlesCount + 3] = p.size;

					g_particule_color_data[4 * ParticlesCount + 0] = p.r;
					g_particule_color_data[4 * ParticlesCount + 1] = p.g;
					g_particule_color_data[4 * ParticlesCount + 2] = p.b;
					g_particule_color_data[4 * ParticlesCount + 3] = p.a;

				}
				else{
					// Particles that just died will be put at the end of the buffer in SortParticles();
					p.cameradistance = -1.0f;
				}
			}
			else{
				// The snowflake has touched the terrain, increase the terrain height, so that the 
				// snowflakes pile up. Implemented by Jekabs.
				if (p.landed == false)
				{
					// Beffore incrementing the terrain height check whether this particle has landed already..
					p.landed = true;
					this->noiseValues[noiseIndex] += p.size;
				}

				// Fill the GPU buffer
				g_particule_position_size_data[4 * ParticlesCount + 0] = p.pos.x;
				g_particule_position_size_data[4 * ParticlesCount + 1] = p.pos.y;
				g_particule_position_size_data[4 * ParticlesCount + 2] = p.pos.z;

				g_particule_position_size_data[4 * ParticlesCount + 3] = p.size;

				g_particule_color_data[4 * ParticlesCount + 0] = p.r;
				g_particule_color_data[4 * ParticlesCount + 1] = p.g;
				g_particule_color_data[4 * ParticlesCount + 2] = p.b;
				g_particule_color_data[4 * ParticlesCount + 3] = p.a;
			}
		}

		ParticlesCount++;
	}

	SortParticles();

	// Update the buffers that OpenGL uses for rendering.
	// There are much more sophisticated means to stream data from the CPU to the GPU, 
	// but this is outside the scope of this tutorial.
	// http://www.opengl.org/wiki/Buffer_Object_Streaming

	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLfloat) * 4, g_particule_position_size_data);

	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLubyte) * 4, g_particule_color_data);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Use our shader
	glUseProgram(programID);

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	// Set our "myTextureSampler" sampler to user Texture Unit 0
	glUniform1i(TextureID, 0);

	// Same as the billboards tutorial
	glUniform3f(CameraRight_worldspace_ID, ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]);
	glUniform3f(CameraUp_worldspace_ID, ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]);

	glUniformMatrix4fv(ViewProjMatrixID, 1, GL_FALSE, &ViewProjectionMatrix[0][0]);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	// 2nd attribute buffer : positions of particles' centers
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		4,                                // size : x + y + z + size => 4
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);

	// 3rd attribute buffer : particles' colors
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
	glVertexAttribPointer(
		2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		4,                                // size : r + g + b + a => 4
		GL_UNSIGNED_BYTE,                 // type
		GL_TRUE,                          // normalized?    *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
		0,                                // stride
		(void*)0                          // array buffer offset
		);

	// These functions are specific to glDrawArrays*Instanced*.
	// The first parameter is the attribute buffer we're talking about.
	// The second parameter is the "rate at which generic vertex attributes advance when rendering multiple instances"
	// http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribDivisor.xml
	glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
	glVertexAttribDivisor(1, 1); // positions : one per quad (its center)                 -> 1
	glVertexAttribDivisor(2, 1); // color : one per quad                                  -> 1

	// Draw the particules !
	// This draws many times a small triangle_strip (which looks like a quad).
	// This is equivalent to :
	// for(i in ParticlesCount) : glDrawArrays(GL_TRIANGLE_STRIP, 0, 4), 
	// but faster.
	glDrawArraysInstanced(GL_POINTS, 0, 1, 4*ParticlesCount);

	// Reset vertex attribute divisors

	glVertexAttribDivisor(1, 0);
	glVertexAttribDivisor(2, 0);

	glDisable(GL_BLEND);
}


void SnowObject::defineUniforms()
{
	glBindVertexArray(VertexArrayID);
	glUseProgram(programID);

	CameraRight_worldspace_ID = glGetUniformLocation(programID, "CameraRight_worldspace");
	CameraUp_worldspace_ID = glGetUniformLocation(programID, "CameraUp_worldspace");
	ViewProjMatrixID = glGetUniformLocation(programID, "VP");

	// fragment shader
	TextureID = glGetUniformLocation(programID, "myTextureSampler");
}


int SnowObject::FindUnusedParticle()
{
	for (int i = LastUsedParticle; i<MaxParticles; i++){
		if (ParticlesContainer[i].life < 0){
			LastUsedParticle = i;
			return i;
		}
	}

	for (int i = 0; i<LastUsedParticle; i++){
		if (ParticlesContainer[i].life < 0){
			LastUsedParticle = i;
			return i;
		}
	}

	return -1; // All particles are taken say none can be re-used
}

// Function which finds the triangle of the terrain which is above / below
// at this point.
int SnowObject::findNoiseLocation(GLfloat x, GLfloat z)
{
	// Find the triangle above which this snowflake currently is located. Implemented by me.
	GLfloat terrainWidthStep = this->terrainWidth / GLfloat(this->terrainVertexCountX);
	GLfloat terrainHeightStep = this->terrainHeight / GLfloat(this->terrainVertexCountZ);

	int noiseIndexX = (x + 0.5*this->terrainWidth) / terrainWidthStep;
	int noiseIndexZ = (z + 0.5*this->terrainHeight) / terrainHeightStep;

	int noiseIndex = (noiseIndexX*this->terrainVertexCountZ + noiseIndexZ) * 4 + 3;
	int noiseArraySize = this->terrainVertexCountX*this->terrainVertexCountZ * 4; // 4 - ocatves for perlin noise
	
	if (noiseIndex >= 0 && noiseIndex < noiseArraySize)
	{
		return noiseIndex;
	}
	else
	{
		return -1;
	}
}