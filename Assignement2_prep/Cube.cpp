/**
	A primitive graphics object - cube.
	Some code taken from lab4 example. The code was extracted into its own
	class and new functionality was added.

	@author Ian Martin, Modified by Jekabs Stikanss
	@version 1.0, 03/11/2014
*/

#include "Cube.h"

Cube::Cube()
{
	this->drawmode = 3;

	this->texID = 0;

	/* Define vertices for a cube in 12 triangles (these coordinates were taken from Ian's code examples.) */

	this->vertexPositions =
	{
		// Back
		-0.25f, 0.25f, -0.25f,
		-0.25f, -0.25f, -0.25f,
		0.25f, -0.25f, -0.25f,

		0.25f, -0.25f, -0.25f,
		0.25f, 0.25f, -0.25f,
		-0.25f, 0.25f, -0.25f,

		// Right Side
		0.25f, 0.25f, 0.25f,
		0.25f, -0.25f, 0.25f,
		0.25f, -0.25f, -0.25f,

		0.25f, -0.25f, -0.25f,
		0.25f, 0.25f, -0.25f,
		0.25f, 0.25f, 0.25f,

		// Front face
		-0.25f, 0.25f, 0.25f,
		-0.25f, -0.25f, 0.25f,
		0.25f, -0.25f, 0.25f,

		0.25f, -0.25f, 0.25f,
		0.25f, 0.25f, 0.25f,
		-0.25f, 0.25f, 0.25f,

		// Left side
		-0.25f, 0.25f, 0.25f,
		-0.25f, -0.25f, 0.25f,
		-0.25f, -0.25f, -0.25f,

		-0.25f, -0.25f, -0.25f,
		-0.25f, 0.25f, -0.25f,
		-0.25f, 0.25f, 0.25f,

		// bottom
		-0.25f, -0.25f, 0.25f,
		0.25f, -0.25f, 0.25f,
		0.25f, -0.25f, -0.25f,

		0.25f, -0.25f, -0.25f,
		-0.25f, -0.25f, -0.25f,
		-0.25f, -0.25f, 0.25f,

		// Top
		-0.25f, 0.25f, -0.25f,
		0.25f, 0.25f, -0.25f,
		0.25f, 0.25f, 0.25f,

		0.25f, 0.25f, 0.25f,
		-0.25f, 0.25f, 0.25f,
		-0.25f, 0.25f, -0.25f
	};

	/*this->vertexPositions =
	{
		// Bottom Vertices
		glm::vec3(-1.0, -1.0, )

		// Front face
		glm::vec3(-1.0, -1.0, 1.0),
		glm::vec3(-1.0, 1.0, 1.0),
		glm::vec3(1.0, 1.0, 1.0),
		glm::vec3(1.0, -1.0, 1.0),

		// Right side face
		glm::vec3(1.0, -1.0, 1.0),
		glm::vec3(1.0, 1.0, 1.0),
		glm::vec3(1.0, 1.0, -1.0),
		glm::vec3(1.0, -1.0, -1.0),

		// Back face
		glm::vec3(-1.0, -1.0, -1.0),
		glm::vec3(-1.0, 1.0, -1.0),
		glm::vec3(1.0, 1.0, -1.0),
		glm::vec3(1.0, -1.0, -1.0),

		// Left side face
		glm::vec3(-1.0, -1.0, 1.0),
		glm::vec3(-1.0, 1.0, 1.0),
		glm::vec3(-1.0, 1.0, -1.0),
		glm::vec3(-1.0, -1.0, -1.0),

		// Top face
		glm::vec3(-1.0, 1.0, 1.0),
		glm::vec3(-1.0, 1.0, -1.0),
		glm::vec3(1.0, 1.0, -1.0),
		glm::vec3(1.0, 1.0, 1.0),

		// Bottom face
		glm::vec3(-1.0, -1.0, 1.0),
		glm::vec3(-1.0, -1.0, -1.0),
		glm::vec3(1.0, -1.0, -1.0),
		glm::vec3(1.0, -1.0, 1.0),

	};
	*/
	// Generate vertex positions
	/*for (int i = 0; i < 2; i++)
	{
		GLfloat yPosition = -1.0f;

		if (i == 1)
		{
			yPosition = 1.0f;
		}

		this->vertexPositions.push_back(glm::vec3(-1.0, yPosition, 1.0));
		this->vertexPositions.push_back(glm::vec3(1.0, yPosition, 1.0));
		this->vertexPositions.push_back(glm::vec3(1.0, yPosition, -1.0));
		this->vertexPositions.push_back(glm::vec3(-1.0, yPosition, -1.0));
	}

	// Define Bottom surface indices.
	this->indices.push_back(0);
	this->indices.push_back(1);
	this->indices.push_back(2);

	this->indices.push_back(2);
	this->indices.push_back(3);
	this->indices.push_back(0);

	// Define Top surface indices
	this->indices.push_back(4);
	this->indices.push_back(5);
	this->indices.push_back(6);

	this->indices.push_back(6);
	this->indices.push_back(7);
	this->indices.push_back(4);


	// Define Side indices
	this->indices.push_back(0);
	this->indices.push_back(1);
	this->indices.push_back(4);

	this->indices.push_back(4);
	this->indices.push_back(1);
	this->indices.push_back(5);

	this->indices.push_back(5);
	this->indices.push_back(1);
	this->indices.push_back(2);

	this->indices.push_back(2);
	this->indices.push_back(6);
	this->indices.push_back(5);

	this->indices.push_back(2);
	this->indices.push_back(3);
	this->indices.push_back(6);

	this->indices.push_back(6);
	this->indices.push_back(3);
	this->indices.push_back(7);

	this->indices.push_back(3);
	this->indices.push_back(0);
	this->indices.push_back(7);

	this->indices.push_back(0);
	this->indices.push_back(4);
	this->indices.push_back(7);
	*/
	/*this->indices = 
	{
		// First triangle strip - sides.

		// Front Face.
		0, 1, 2,
		1, 2, 3,

		// Right side face
		4, 5, 6,
		5, 6, 7,

		// Back face

	}*/

	/*this->vertexNormals =
	{

	}*/


	this->vertexNormals = 
	{
		0, 0, -1.f, 0, 0, -1.f, 0, 0, -1.f,
		0, 0, -1.f, 0, 0, -1.f, 0, 0, -1.f,
		1.f, 0, 0, 1.f, 0, 0, 1.f, 0, 0,
		1.f, 0, 0, 1.f, 0, 0, 1.f, 0, 0,
		0, 0, 1.f, 0, 0, 1.f, 0, 0, 1.f,
		0, 0, 1.f, 0, 0, 1.f, 0, 0, 1.f,
		-1.f, 0, 0, -1.f, 0, 0, -1.f, 0, 0,
		-1.f, 0, 0, -1.f, 0, 0, -1.f, 0, 0,
		0, -1.f, 0, 0, -1.f, 0, 0, -1.f, 0,
		0, -1.f, 0, 0, -1.f, 0, 0, -1.f, 0,
		0, 1.f, 0, 0, 1.f, 0, 0, 1.f, 0,
		0, 1.f, 0, 0, 1.f, 0, 0, 1.f, 0,
	};

	this->makeVBO();
}


Cube::~Cube()
{
}



/* A method which allows changing the vetex positions of this object. */
void Cube::setVertexPositions(std::vector<GLfloat> vertexPositions)
{
	/* Set the new vertex positions. */
	this->vertexPositions = vertexPositions;

	this->makeVBO();
}

void Cube::makeVBO()
{

	/*Generate the vertex buffer object */
	glGenBuffers(1, &this->bufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, this->bufferObject);
	glBufferData(GL_ARRAY_BUFFER, this->vertexPositions.size()*sizeof(GLfloat), this->vertexPositions.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Store the normals in a buffer object */
	glGenBuffers(1, &this->normalsBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, this->normalsBufferObject);
	glBufferData(GL_ARRAY_BUFFER, this->vertexNormals.size()*sizeof(GLfloat), this->vertexNormals.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (this->texID > 0)
	{
		/* Generate the texture coordinate buffer */
		glGenBuffers(1, &this->textureBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, this->textureBuffer);
		glBufferData(GL_ARRAY_BUFFER, this->texCoords.size()*sizeof(GLfloat), this->texCoords.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

/* Draw cube object. */
void Cube::draw()
{
	/* Bind cube vertices. Note that this is in attribute index 0 */
	glBindBuffer(GL_ARRAY_BUFFER, this->bufferObject);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	/* Bind cube normals. Note that this is in attribute index 2 */
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, this->normalsBufferObject);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);


	glActiveTexture(GL_TEXTURE0);

	if (this->texID > 0)
	{
		/* BInd the texture data */
		glEnableVertexAttribArray(3);
		glBindBuffer(GL_ARRAY_BUFFER, this->textureBuffer);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	}

	glBindTexture(GL_TEXTURE_2D, this->texID);

	/* Define the texture behaviour parameters */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	glPointSize(3.f);

	// Enable this line to show model in wireframe
	if (this->drawmode == 1)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (this->drawmode == 2)
	{
		glDrawArrays(GL_POINTS, 0, this->vertexCount);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}

/* Set the drawmode for this object. */
void Cube::setDrawmode(int drawmode)
{
	this->drawmode = drawmode;
}

/* Set the texID */
void Cube::setTexID(GLuint texID)
{
	this->texCoords.clear();

	for (int i = 0; i < 12; i++)
	{

		if (i % 2 == 0)
		{
			this->texCoords.push_back(1.0f);
			this->texCoords.push_back(0.0f);

			this->texCoords.push_back(1.0f);
			this->texCoords.push_back(1.0f);

			this->texCoords.push_back(0.0f);
			this->texCoords.push_back(1.0f);

		}
		else
		{
			this->texCoords.push_back(0.0f);
			this->texCoords.push_back(1.0f);

			this->texCoords.push_back(0.0f);
			this->texCoords.push_back(0.0f);

			this->texCoords.push_back(1.0f);
			this->texCoords.push_back(0.0f);

		}
	}

	this->texID = texID;


	int b = this->texCoords.size();
	int k = 1;
	/* Generate the texture coordinate buffer */
	/*glGenBuffers(1, &this->textureBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->textureBuffer);
	glBufferData(GL_ARRAY_BUFFER, 12*6*sizeof(GLfloat), this->texCoords.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);*/
	this->makeVBO();
}