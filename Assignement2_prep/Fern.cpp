#include "Fern.h"

Fern::Fern(GLuint modelID, int level)
{
	this->modelID = modelID;
	this->lSystem = "F[+F][-F]\0";
	this->yAngle  = 0.0f;
	this->xAngle  = 0.0f;
	this->level	  = level;


	/* Create our quad and texture */
	glGenBuffers(1, &this->bufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, this->bufferObject);

	// Create dat for our quad with vertices, normals and texturee coordinates 
	static const GLfloat quad_data[] =
	{
		// Vertex positions
		0, 0, 0,
		0.2, 0, 0,
		0.2, 1.f, 0,
		0, 1.f, 0,

		// Normals
		0, 0, 1.f,
		0, 0, 1.f,
		0, 0, 1.f,
		0, 0, 1.f,

		// Texture coordinates. Note we only need two per vertex but have a
		// redundant third to fit the texture coords in the same buffer for this simple object
		0.0f, 0.0f, 0,
		1.0f, 0.0f, 0,
		1.0f, 1.0f, 0,
		0.0f, 1.0f, 0,
	};

	// Copy the data into the buffer. See how this example combines the vertices, normals and texture
	// coordinates in the same buffer and uses the last parameter of  glVertexAttribPointer() to
	// specify the byte offset into the buffer for each vertex attribute set.
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_data), quad_data, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)(0));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)(12 * sizeof(float)));
	//glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)(24 * sizeof(float)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(2);
	//glEnableVertexAttribArray(3);
}


Fern::~Fern()
{
}


void Fern::make(int level, int leaves)
{
	int current;
	current = 0;
	
	while (this->lSystem[current])
	{
		switch (this->lSystem[current])
		{
			case 'F':
			
				if (level == 0)
				{
					if (leaves && ((this->lSystem[current + 1] == ']') || (this->lSystem[current + 1] == 0)))
					{
						this->drawBranch(0);
					}
					else
					{
						this->drawBranch(1);
					}

					this->transformations.top() = glm::translate(this->transformations.top(), glm::vec3(0, 1.0f, 0));
				}
				else
				{
					if ((this->lSystem[current + 1] == ']') || (this->lSystem[current + 1] == 0))
					{
						this->make(level - 1, leaves);
					}
					else
					{
						this->make(level - 1, 0);
					}
				}
				
				break;
			
			case '[':
				this->transformations.push(glm::scale(this->transformations.top(), glm::vec3(0.7, 0.7, 0.7)));
				
				break;

			case ']':
				this->transformations.pop();
				
				break;

			case '+':
				// rotate 45 degrees
				this->transformations.top() = glm::rotate(this->transformations.top(), -45.f + this->xAngle, glm::vec3(0, 0, 1.f));
				
				break;

			case '-':
				this->transformations.top() = glm::rotate(this->transformations.top(), 30.f + this->yAngle, glm::vec3(0, 0, 1.f));
				
				break;
			
			default:
				break;
		}

		current++;
	}
}

/* Draw a single branch */
void Fern::drawBranch(int leaf)
{
	glm::mat4 transform = this->transformations.top();
	transform = transform * this->model;

	glUniformMatrix4fv(this->modelID, 1, GL_FALSE, &(transform[0][0]));

	/* Draw our textured quad*/
	//glBindTexture(GL_TEXTURE_2D, this->texID);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}


void Fern::draw()
{
	GLuint i;

	/* Draw the vertices as GL_POINTS */
	glBindBuffer(GL_ARRAY_BUFFER, this->bufferObject);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	/* Bind the sphere normals */
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, this->normalsBufferObject);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	/* BInd the texture data */
	/*glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, this->textureBuffer);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	*/
	glPointSize(3.f);


	this->transformations.push(glm::mat4(1.0f));

	// Define the model transformations 
	model = glm::mat4(1.0f);

	this->make(this->level, 1);
}

/* Set the draw mode for this object. */
void Fern::setDrawmode(int drawmode)
{
	this->drawmode = drawmode;
}