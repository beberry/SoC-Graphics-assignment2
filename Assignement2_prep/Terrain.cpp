#include "Terrain.h"


Terrain::Terrain()
{
	this->vertexCountX = 100;
	this->vertexCountZ = 100;
	this->perlin_octaves = 4;
	this->height_scale = 1.f;
	this->height = 20.0;
	this->width = 20.0;
	this->drawmode = 3;
}


Terrain::~Terrain()
{
}


void Terrain::create()
{
	/* Create array of vertices */
	GLuint vertexCount = this->vertexCountX * this->vertexCountZ;
	this->vertexPositions.resize(vertexCount);
	this->vertexNormals.resize(vertexCount);

	/* First calculate the noise array which we'll use for our vertex height values */
	calculateNoise(1.4f, 2.f);

	/* Debug code to check that noise values are sensible */
	for (int i = 0; i < (this->vertexCountX*this->vertexCountZ*perlin_octaves); i++)
		{
			printf("\n noise[%d] = %f", i, this->noiseValues[i]);
		}

	/* Define starting (x,z) positions and the step changes */
	GLfloat xpos = -width / 2.f;
	GLfloat xpos_step = width / GLfloat(this->vertexCountX);
	GLfloat zpos_step = height / GLfloat(this->vertexCountZ);
	GLfloat zpos_start = -height / 2.f;

	/* Define the vertex positions and the initial normals for a flat surface */
	for (GLuint x = 0; x < this->vertexCountX; x++)
	{
		GLfloat zpos = zpos_start;
		for (GLuint z = 0; z < this->vertexCountZ; z++)
		{
			GLfloat height = this->noiseValues[(x*this->vertexCountZ + z) * 4 + 3];
			this->vertexPositions[x*this->vertexCountX + z] = glm::vec3(xpos, (height - 1.f)*height_scale, zpos);
			this->vertexNormals[x*this->vertexCountX + z] = glm::vec3(0, 1.0f, 0);		// Normals for a flat surface
			zpos += zpos_step;
		}
		xpos += xpos_step;
	}

	/* Define vertices for triangle strips */
	for (GLuint x = 0; x < this->vertexCountX - 1; x++)
	{
		GLuint top = x * this->vertexCountZ;
		GLuint bottom = top + this->vertexCountZ;
		for (GLuint z = 0; z < this->vertexCountZ; z++)
		{
			this->indices.push_back(top++);
			this->indices.push_back(bottom++);
		}
	}

	this->calcNormals();
	this->createVBO();
}

/* A method which calculates the vertex positions. */
void Terrain::calcHeight()
{
	GLuint vertexCount = this->vertexCountX * this->vertexCountZ;

}

/* A method which calculates the normals for the current height map. */
void Terrain::calcNormals()
{
	GLuint element_pos = 0;
	glm::vec3 AB, AC, cross_product;

	// Loop through each triangle strip  
	for (GLuint x = 0; x < this->vertexCountX - 1; x++)
	{
		// Loop along the strip
		for (GLuint tri = 0; tri < this->vertexCountZ * 2 - 2; tri++)
		{
			// Extract the vertex indices from the element array 
			GLuint v1 = this->indices[element_pos];
			GLuint v2 = this->indices[element_pos + 1];
			GLuint v3 = this->indices[element_pos + 2];

			// Define the two vectors for the triangle
			AB = this->vertexPositions[v2] - this->vertexPositions[v1];
			AC = this->vertexPositions[v3] - this->vertexPositions[v1];

			// Calculate the cross product
			cross_product = glm::cross(AB, AC);

			// Add this normal to the vertex normal for all three vertices in the triangle
			this->vertexNormals[v1] += cross_product;
			this->vertexNormals[v2] += cross_product;
			this->vertexNormals[v3] += cross_product;

			// Move on to the next vertex along the strip
			element_pos++;
		}

		// Jump past the lat two element positions to reach the start of the strip
		element_pos += 2;
	}

	// Normalise the normals
	for (GLuint v = 0; v < this->vertexCountX * this->vertexCountZ; v++)
	{
		this->vertexNormals[v] = glm::normalize(this->vertexNormals[v]);
	}
}

/* Creating the vertex and normal buffer objects. */
void Terrain::createVBO()
{
	/* Generate the vertex buffer object */
	glGenBuffers(1, &this->bufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, this->bufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * this->vertexPositions.size(), this->vertexPositions.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Store the normals in a buffer object */
	glGenBuffers(1, &this->normalsBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, this->normalsBufferObject);
	glBufferData(GL_ARRAY_BUFFER, this->vertexNormals.size() * sizeof(glm::vec3), this->vertexNormals.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Store the indices in a indice buffer object */
	glGenBuffers(1, &this->indiceBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indiceBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), this->indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/* A method which draws the terrain object using the previously defined vertex buffer. */
void Terrain::draw()
{
	int size;	// Used to get the byte size of the element (vertex index) array

	// Describe our vertices array to OpenGL (it can't guess its format automatically)
	glBindBuffer(GL_ARRAY_BUFFER, this->bufferObject);
	glVertexAttribPointer(
		0,  // attribute index
		3,                  // number of elements per vertex, here (x,y,z)
		GL_FLOAT,           // the type of each element
		GL_FALSE,           // take our values as-is
		0,                  // no extra data between each position
		0                   // offset of first element
		);

	glBindBuffer(GL_ARRAY_BUFFER, this->normalsBufferObject);
	glVertexAttribPointer(
		2, // attribute
		3,                  // number of elements per vertex, here (x,y,z)
		GL_FLOAT,           // the type of each element
		GL_FALSE,           // take our values as-is
		0,                  // no extra data between each position
		0                   // offset of first element
		);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indiceBufferObject);
	//glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	

	glPointSize(3.f);

	// Enable this line to show model in wireframe
	if (drawmode == 1)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	/* Draw the triangle strips */
	for (int i = 0; i < this->vertexCountX - 1; i++)
	{
		GLuint location = sizeof(GLuint) * (i * this->vertexCountZ * 2);
		glDrawElements(GL_TRIANGLE_STRIP, this->vertexCountZ * 2, GL_UNSIGNED_INT, (GLvoid*)(location));
	}
}

/* Define the terrian heights */
/* Uses code adapted from OpenGL Shading Language Cookbook: Chapter 8 */
void Terrain::calculateNoise(GLfloat freq, GLfloat scale)
{
	/* Create the array to store the noise values */
	/* The size is the number of vertices * number of octaves */
	this->noiseValues = new GLfloat[this->vertexCountX * this->vertexCountZ * this->perlin_octaves];

	for (int i = 0; i < (this->vertexCountX * this->vertexCountZ * this->perlin_octaves); i++) this->noiseValues[i] = 0;

	GLfloat xfactor = 1.f / (this->vertexCountX - 1);
	GLfloat zfactor = 1.f / (this->vertexCountZ - 1);

	for (int row = 0; row < this->vertexCountZ; row++)
	{
		for (int col = 0; col < this->vertexCountX; col++)
		{
			GLfloat x = xfactor * col;
			GLfloat z = zfactor * row;
			GLfloat sum = 0;
			GLfloat curent_scale = scale;
			GLfloat current_freq = freq;

			// Compute the sum for each octave
			for (int oct = 0; oct < 4; oct++)
			{
				glm::vec2 p(x*current_freq, z*current_freq);
				GLfloat val = glm::perlin(p) / curent_scale;
				sum += val;
				GLfloat result = (sum + 1.f) / 2.f;

				// Store the noise value in our noise array
				this->noiseValues[(row * this->vertexCountX + col) * 4 + oct] = result;

				// Move to the next frequency and scale
				current_freq *= 2.f;
				curent_scale *= scale;
			}

		}
	}
}


/* Set the drawmode for this object. */
void Terrain::setDrawmode(int drawmode)
{
	this->drawmode = drawmode;
}