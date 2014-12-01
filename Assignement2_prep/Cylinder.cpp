/**
	A primitive graphics object - cylinder.
	The variables can be modified, so that the object becomes more complex - cone, cylinder which changes it's width.

	@author Jekabs Stikans
	@version 1.0, 01/11/2014
*/
#include "Cylinder.h"

/* Construction */
Cylinder::Cylinder(GLfloat height, GLfloat maxTopRadius, GLfloat radiussCoeff, GLuint textureID)
{
	this->radiussCoeff   = radiussCoeff;
	this->height		 = height;
	this->maxTopRadius	 = maxTopRadius;
	this->drawmode	     = 3;
	this->vertexCount    = 0;
	this->textureID		 = textureID;
}

/* Destructor */
Cylinder::~Cylinder()
{
}

/* Create the vertex buffer object */
GLuint Cylinder::makeVBO(GLfloat numlats, GLfloat numlongs)
{
	GLuint i, j;

	this->numlats  = numlats;
	this->numlongs = numlongs;

	/* Calculate the number of vertices required in hemisphere */
	this->vertexCount = (numlats) * (numlongs);

	this->vertexPositions.resize(this->vertexCount * 3);

	makeUnitObject(numlats, numlongs);

	this->vertexNormals.resize(this->vertexCount);

	// init arr
	for (int i = 0; i < this->vertexCount; i++)
	{
		this->vertexNormals[i] = glm::vec3(0.0, 0.0, 0.0);
	}

	// -1 to ignore the last point in the strip. We have to ignore first and last point in the strip.
	
	int ind = 0;
	
	for (int lat = 0; lat < numlats-1; ++lat)
	{
		for (int lon = 0; lon < numlongs; ++lon)
		{
			int p0Index = 0;
			int p1Index = 0;
			int p2Index = 0;

			glm::vec3 crossP = glm::cross(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0));

			if ((ind+1) % 2 == 0)
			{
				// Triangle looks like this
				// p2*------* p1
				//    \    |
				//     \   |
				//      \  |
				//       \ |
				//        \|
				//         * p0 current point, so the other two points are one lat above.
				p0Index = ind;
				p1Index = ind + numlongs;
				p2Index = ind + 1;


				glm::vec3 p0 = glm::vec3(
					this->vertexPositions[p0Index * 3],
					this->vertexPositions[p0Index * 3 + 1],
					this->vertexPositions[p0Index * 3 + 2]
				);

				glm::vec3 p1 = glm::vec3(
					this->vertexPositions[p1Index * 3],
					this->vertexPositions[p1Index * 3 + 1],
					this->vertexPositions[p1Index * 3 + 2]
				);

				glm::vec3 p2 = glm::vec3(
					this->vertexPositions[p2Index * 3],
					this->vertexPositions[p2Index * 3 + 1],
					this->vertexPositions[p2Index * 3 + 2]
				);


				glm::vec3 vecA = p1 - p0;
				glm::vec3 vecB = p2 - p0;

				crossP = glm::cross(vecB, vecA);
			}
			else
			{
				// Triangle looks like this
				//    * p2
				//    |\
				//    | \
				//    |  \
				//    |   \
				//    |    \
				//  p0*------* p1   p0 - current point

				p0Index = ind;
				p1Index = ind + 1;
				p2Index = ind + numlongs;


				glm::vec3 p0 = glm::vec3(
					this->vertexPositions[p0Index * 3],
					this->vertexPositions[p0Index * 3 + 1], 
					this->vertexPositions[p0Index * 3 + 2]
				);

				glm::vec3 p1 = glm::vec3(
					this->vertexPositions[p1Index * 3], 
					this->vertexPositions[p1Index * 3 + 1], 
					this->vertexPositions[p1Index * 3 + 2]
				);

				glm::vec3 p2 = glm::vec3(
					this->vertexPositions[p2Index * 3], 
					this->vertexPositions[p2Index * 3 + 1], 
					this->vertexPositions[p2Index * 3 + 2]
				);

				glm::vec3 vecA = p1 - p0;
				glm::vec3 vecB = p2 - p0;

				crossP = glm::cross(vecA, vecB);
			}


			// Set the normals
			this->vertexNormals[p0Index] += crossP;
			this->vertexNormals[p1Index] += crossP;
			this->vertexNormals[p2Index] += crossP;
				

			if (ind % ((int)numlongs + 1) == 0)
			{
				// First triangle in this strip.
				this->vertexNormals[ind + (int)numlongs + 1] += crossP;
				this->vertexNormals[ind + (int)numlongs] += crossP;
			}

			if ((ind + 1) % (int)numlongs == 0)
			{
				// Last triangle in this strip.
				this->vertexNormals[ind - (int)numlongs + 1] += crossP;
				this->vertexNormals[ind + 1] += crossP;

			}

			ind++;
		}
	}
	
	// Normalize the surface normals
	for (int i = 0; i < this->vertexCount; i++)
	{
		this->vertexNormals[i] = glm::normalize(this->vertexNormals[i]);
	}

	/* Generate the vertex buffer object */
	glGenBuffers(1, &this->bufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, this->bufferObject);
	glBufferData(GL_ARRAY_BUFFER, this->vertexPositions.size()*sizeof(GLfloat), this->vertexPositions.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Store the normals in a buffer object */
	glGenBuffers(1, &this->normalsBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, this->normalsBufferObject);
	glBufferData(GL_ARRAY_BUFFER, this->vertexNormals.size()*sizeof(glm::vec3), this->vertexNormals.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	
	/* Generate texture coordinats */
	std::vector<GLfloat> texcoords1;

	GLfloat latstep = 1.0 / numlats;
	GLfloat longstep = 1.0 / numlongs;

	for (int i = 0; i < numlats; i++)
	{
		for (int x = 0; x < numlongs; x++)
		{
			texcoords1.push_back(x*longstep);
			texcoords1.push_back(i*latstep);
		}
	}

	/* Generate the texture coordinate buffer */
	glGenBuffers(1, &this->textureBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->textureBuffer);
	glBufferData(GL_ARRAY_BUFFER, texcoords1.size()*sizeof(GLfloat), texcoords1.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Calculate the number of indices in our index array and allocate memory for it */
	GLuint numindices = (numlongs*2+2) * (numlats-1);

	GLuint* pindices = new GLuint[numindices];

	// fill "indices" to define triangle strips
	GLuint index = 0;		// Current index
	GLuint start = 0;		// Start index for each latitude row

	for (j = 0; j < numlats-1; j++)
	{

		for (i = 0; i < numlongs; i++)
		{
			pindices[index++] = start + i;
			pindices[index++] = start + i + numlongs;
		}

		pindices[index++] = start; // close the triangle strip loop by going back to the first vertex in the loop
		pindices[index++] = start + numlongs; // close the triangle strip loop by going back to the first vertex in the loop


		start += numlongs;
	}
	
	// Generate a buffer for the indices
	glGenBuffers(1, &this->elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numindices * sizeof(GLuint), pindices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	delete pindices;
	return this->vertexCount;
}

void Cylinder::makeUnitObject(GLuint numlats, GLuint numlongs)
{
	GLfloat DEG_TO_RADIANS = 3.141592f / 180.f;
	GLuint vnum = 0;
	GLfloat x, y, z, lat_radians, lon_radians;

	GLfloat latstep = this->height / (numlats-1);
	GLfloat longstep = 360.f / numlongs;

	int latI = 0;

	GLfloat radiuss = this->maxTopRadius;

	for (GLfloat i = 0.0; latI < numlats; i -= latstep)
	{
		latI++;

		/* Define vertices along latitude lines */
		int p = 0;

		for (GLfloat lon = -180.f; lon < 180.f, p < numlongs; lon += longstep)
		{
			lon_radians = (lon)* DEG_TO_RADIANS;

			x = cos(lon_radians)*radiuss;
			y = i;
			z = sin(lon_radians)*radiuss;


			/* Define the vertex */
			this->vertexPositions[vnum * 3]     = x; 
			this->vertexPositions[vnum * 3 + 1] = y; 
			this->vertexPositions[vnum * 3 + 2] = z;
			
			vnum++;
			p++;
		}

		radiuss = radiuss * this->radiussCoeff;
	}
}

/* Draw the cylinder. */
void Cylinder::draw()
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
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, this->textureBuffer);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

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
		glBindTexture(GL_TEXTURE_2D, this->texID);

		/* Draw the latitude triangle strips */
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementBuffer);

		GLuint lat_offset = 4 * (this->numlongs * 2 + 2);

		for (i = 0; i < numlats; i++)
		{
			glDrawElements(GL_TRIANGLE_STRIP, this->numlongs * 2 + 2, GL_UNSIGNED_INT, (GLvoid*)(lat_offset*i));
		}
	}
}

/* Return the vertex positions for this object */
std::vector<GLfloat>* Cylinder::getVertexPositions()
{
	return &this->vertexPositions;
}

/* Set the draw mode for this object. */
void Cylinder::setDrawmode(int drawmode)
{
	this->drawmode = drawmode;
}

/* Set the texture for this object. Originally written by Ian Martin, modified by Jekabs Stikans. */
void Cylinder::setTexture(std::string textureName)
{
	try
	{
		/* Not actually needed if using one texture at a time */
		glActiveTexture(GL_TEXTURE0);

		std::string fileLoc = "textures/" + textureName;

		/* load an image file directly as a new OpenGL texture */
		this->texID = SOIL_load_OGL_texture(fileLoc.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);

		/* check for an error during the load process */
		if (this->texID == 0)
		{
			printf("TexID SOIL loading error: '%s'\n", SOIL_last_result());
		}
	}
	catch (std::exception &e)
	{
		printf("\nImage file loading failed.");
	}

	/* Define the texture behaviour parameters */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}