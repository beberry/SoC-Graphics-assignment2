/**
	Class which generates a 2D texture iusing Perlin noise, to
	simulate clouds.

	@author Jekabs Stikans (Some parts have been taken from the example code by Ian Martin)
	@version 1.0, 03/12/2014
*/

#include "CloudTexture.h"


CloudTexture::CloudTexture()
{
}


CloudTexture::~CloudTexture()
{
}

// The method which generates the texture.
void CloudTexture::create(GLuint width, GLuint height)
{
	// Set the width and height of the texture.
	this->width = width;
	this->height = height;

	// Generate the noise.
	this->generateNoise(5, 1.2, 4);

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &this->texID);
	glBindTexture(GL_TEXTURE_2D, this->texID);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, this->width, this->height);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, this->width, this->height, GL_RGBA, GL_UNSIGNED_BYTE, this->noisValues);


	/* Define the texture behaviour parameters */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

}

// Return the index number of the texture.
GLuint CloudTexture::getTexID()
{
	return this->texID;
}

// Parts of this method are taken from Ian Martins terrain example.
void CloudTexture::generateNoise(GLfloat pFrequency, GLfloat pScale, GLuint octaves)
{
	this->noisValues = new GLubyte[this->width * this->height * 4];

	GLfloat xfactor = 1.f / (this->width - 1);
	GLfloat yfactor = 1.f / (this->height - 1);

	for (int row = 0; row < this->height; row++)
	{
		for (int col = 0; col < this->width; col++)
		{
			GLfloat x = xfactor * col;
			GLfloat y = yfactor * row;
			GLfloat sum = 0.0f;
			GLfloat freq = pFrequency;
			GLfloat scale = pScale;

			// Sum up the values of all octaves.
			for (int oct = 0; oct < octaves; oct++)
			{
				glm::vec2 p(x*freq, y*freq);
				GLfloat val = 0.0f;

				val = glm::perlin(p, glm::vec2(freq)) / scale;
				sum += val;

				// Increase the frequency and scale.
				freq  *= 2.f;
				scale *= pScale;

				GLfloat result = (val + 1.f) / 2.0f;

				// Save space and store each ocatve in r, g, b or a fields.
				this->noisValues[(row * this->width + col) * 4 + oct] = (GLubyte)(result * 255.f);

			}
		}
	}

}