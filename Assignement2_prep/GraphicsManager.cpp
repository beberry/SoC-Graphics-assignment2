/**
	An object which is responsible for managing the whole application - draws the frames,
	reacts to callbacks.

	@author Jekabs Stikans
	@version 2.0, 28/10/2014
*/

#include "GraphicsManager.h"

GLuint program, snowShader;
GLuint vao;

/* Configuration properties */
GLuint emitmode;
GLuint drawmode;
GLuint fogmode;
GLuint wingCount;

/* Uniforms */
GLuint modelID, viewID, projectionID, normalMatrixID, lightPosID, terrainModeID, emitmodeID, textureID, textureModeID, specularModeID, fogmodeID, cloudModeID;

/* Graphics models */
Sphere *lightSourceModel;
Windmill *windmill;
Terrain *terrain;
SnowObject *snowModel;
Cube *skyBox;
Fern *fernModel;

/* Other configuration */
GLfloat light_x, light_y, light_z, vx, vy, vz, wingAngle, wingAngle_inc, head_angle, zoom, aspect_ratio;
int window_w, window_h;

/* Pre-define functions, so that the constructor could be defined at the top of the file. */
static void keyCallback(GLFWwindow* window, int k, int s, int action, int mods);
static void resizeWindow(GLFWwindow* window, int w, int h);
static void display();
static void errorCallback(int error, const char* description);

/* Constructor for this object. */
GraphicsManager::GraphicsManager()
{
	/* Initial configuration. */
	window_w = 1024;
	window_h = 500;

	wingCount = 5;
	fogmode   = 2;

	Glfw_wrap *glfw = new Glfw_wrap(window_w, window_h, "Assignment 2, JS");

	if (!ogl_LoadFunctions())
	{
		fprintf(stderr, "ogl_LoadFunctions() failed. Exiting\n");
	}
	else
	{
		cmdManager();

		glfw->setKeyCallback(keyCallback);
		glfw->setReshapeCallback(resizeWindow);
		glfw->setErrorCallback(errorCallback);
		glfw->setRenderer(display);

		/* Initialize glfw wrapper. s*/
		this->init(glfw);

		/* Start our event loop. */
		glfw->eventLoop();

		delete(glfw);
	}
}

/* De-constructor for this object. */
GraphicsManager::~GraphicsManager()
{
}

/* Initialization method - could be moved inside the constructor. */
void GraphicsManager::init(Glfw_wrap *glfw)
{
	/* General GLFW config. */
	aspect_ratio = window_w / window_h;

	/* Scene variables.*/
	zoom = 5.0f;
	emitmode = 0;

	light_x = 0.7;
	light_y = 0.7;
	light_z = 1.0;

	vx = 0;
	vx = 0;
	vz = 0.f;

	wingAngle     = 0.0f;
	wingAngle_inc = 2.0f;
	head_angle    = 0.0f;

	// Generate index (name) for one vertex array object
	glGenVertexArrays(1, &vao);

	// Create the vertex array object and make it current
	glBindVertexArray(vao);

	/* Create shader manager and load the shader programs. */

	ShaderManager *shaderManager = new ShaderManager();


	/* Create Snow model */
	try
	{
		snowShader = shaderManager->LoadShader("shaders/snow.vert", "shaders/snow.frag", "shaders/snow_geometry.glsl");
	}
	catch (std::exception &e)
	{
		std::cout << "Caught exception: " << e.what() << std::endl;
		std::cin.ignore();
		exit(0);
	}

	try
	{
		program = shaderManager->LoadShader("shaders/data.vert", "shaders/data.frag", NULL);
	}
	catch (std::exception &e)
	{
		std::cout << "Caught exception: " << e.what() << std::endl;
		std::cin.ignore();
		exit(0);
	}

	/* Define uniforms to send to vertex shader */
	normalMatrixID = glGetUniformLocation(program, "normalmatrix");
	modelID = glGetUniformLocation(program, "model");
	viewID = glGetUniformLocation(program, "view");
	projectionID = glGetUniformLocation(program, "projection");
	lightPosID = glGetUniformLocation(program, "lightPos");
	emitmodeID = glGetUniformLocation(program, "emitmode");
	textureID = glGetUniformLocation(program, "tex1");
	textureModeID = glGetUniformLocation(program, "textureMode");
	specularModeID = glGetUniformLocation(program, "specularMode");
	fogmodeID = glGetUniformLocation(program, "fogMode");
	terrainModeID = glGetUniformLocation(program, "terrainMode");
	cloudModeID = glGetUniformLocation(program, "cloudMode");

	/* Create a windmill object/ */
	windmill = new Windmill(wingCount, 4.0, 1.0, 0.73, 1.1, modelID, normalMatrixID, textureID, textureModeID, specularModeID);

	/* Create the terrain object. */
	terrain = new Terrain();
	terrain->setTexture("grass.jpg");
	terrain->create();

	/* Create the skyBox */
	skyBox = new Cube();
	CloudTexture *clTx = new CloudTexture();
	clTx->create(600, 600);
	skyBox->setTexID(clTx->getTexID());

	/* Create the light source object. */
	lightSourceModel = new Sphere(0.5, 0.5, false, textureID);
	lightSourceModel->makeVBO(20.0f, 30.0f);

	snowModel = new SnowObject();
	snowModel->create(snowShader, terrain->noiseValues, terrain->vertexCountX, terrain->vertexCountZ, terrain->width, terrain->height);

	/* Create the fern model */
	// The fern model is not ready yet.
	//fernModel = new Fern(modelID, 1);
}

/* The display callback method which redraws the scene. */
void display()
{
	/* Set the angle for wings */
	windmill->setWingAngle(wingAngle);
	wingAngle += wingAngle_inc;

	if (wingAngle > 360.0f || wingAngle < -360.0f)
	{
		wingAngle = std::fmod(wingAngle, 360.0f);
	}

	/* Anti aliasing. */
	glEnable(GL_MULTISAMPLE);

	/* Background colour for the scene. */
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	/* Clear the colour and frame buffers. */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Enable depth test. */
	glEnable(GL_DEPTH_TEST);

	/* Use the our loaded shader program. */
	glUseProgram(program);

	/* Projection matrix. */
	glm::mat4 Projection = glm::perspective(30.0f, aspect_ratio, 0.1f, 100.0f);

	/* Define view. */
	glm::mat4 View = glm::lookAt(
		glm::vec3(0, 0, zoom),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0)
		);

	// Apply rotations to the view position
	View = glm::rotate(View, -vx, glm::vec3(1, 0, 0));
	View = glm::rotate(View, -vy, glm::vec3(0, 1, 0));
	View = glm::rotate(View, -vz, glm::vec3(0, 0, 1));

	/* Set the light position. */
	glm::vec4 lightPos = View * glm::vec4(light_x, light_y, light_z, 1.0);

	/* Define a stack objects for model transformations. */
	glm::mat4 model = glm::mat4(1.0f);
	std::stack<glm::mat4> modelTranslate;
	std::stack<glm::mat4> modelScale;
	std::stack<glm::mat4> modelRotate;

	modelTranslate.push(glm::mat4(1.0f));
	modelScale.push(glm::mat4(1.0f));
	modelRotate.push(glm::mat4(1.0f));

	glm::mat3 gl_NormalMatrix = glm::mat3(1.0f);

	/* Individual Objects */

	model = modelTranslate.top() * modelScale.top() * modelRotate.top();
	gl_NormalMatrix = glm::transpose(glm::inverse(glm::mat3(View*model)));

	// Send uniform variables to the shader,
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(projectionID, 1, GL_FALSE, &Projection[0][0]);
	glUniform4fv(lightPosID, 1, glm::value_ptr(lightPos));
	glUniform1ui(emitmodeID, emitmode);
	glUniform1ui(terrainModeID, 0);
	glUniform1ui(fogmodeID, fogmode);
	glUniform1ui(cloudModeID, 0);
	glUniformMatrix3fv(normalMatrixID, 1, GL_FALSE, &gl_NormalMatrix[0][0]);

	/* Draw our Windmill */

	glm::mat4 tmpModel = model;
	tmpModel = glm::translate(tmpModel, glm::vec3(-1.0, -0.3, 2.0));
	tmpModel = glm::scale(tmpModel, glm::vec3(0.15, 0.15, 0.15));

	modelScale.push(glm::scale(modelScale.top(), glm::vec3(0.4, 0.4, 0.4)));
	windmill->draw(View, tmpModel, modelTranslate, modelScale, modelRotate);
	modelScale.pop();

	/* END Windmill */

	/* Start SkyBox. */
	tmpModel = glm::scale(model, glm::vec3(20, 20, 20));
	gl_NormalMatrix = glm::transpose(glm::inverse(glm::mat3(View*tmpModel)));
	glUniform1ui(textureModeID, 1);
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &tmpModel[0][0]);
	glUniformMatrix3fv(normalMatrixID, 1, GL_FALSE, &gl_NormalMatrix[0][0]);
	glUniform1ui(cloudModeID, 1);

	skyBox->draw();
	glUniform1ui(cloudModeID, 0);
	/* END SkyBox */

	/* Start Terrain */

	gl_NormalMatrix = glm::transpose(glm::inverse(glm::mat3(View*model)));
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix3fv(normalMatrixID, 1, GL_FALSE, &gl_NormalMatrix[0][0]);
	glUniform1ui(specularModeID, 1);
	glUniform1ui(textureModeID, 1);
	glUniform1ui(emitmodeID, 0);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(2);
	glUniform1ui(terrainModeID, 1);

	terrain->draw();

	glUniform1ui(terrainModeID, 0);
	glUniform1ui(textureModeID, 0);

	/* END Terrain */

	/* Start Fern Model */
	//fernModel->draw();
	/* END Fern Model*/

	/* START LIGHT Sphere */
	modelTranslate.push(glm::translate(modelTranslate.top(), glm::vec3(light_x, light_y, light_z)));
	modelScale.push(glm::scale(modelScale.top(), glm::vec3(0.1f, 0.1f, 0.1f)));

	model = modelTranslate.top() * modelScale.top() * modelRotate.top();
	gl_NormalMatrix = glm::transpose(glm::inverse(glm::mat3(View*model)));

	/* Send those uniforms which have changed. */
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix3fv(normalMatrixID, 1, GL_FALSE, &gl_NormalMatrix[0][0]);

	/* Draw light object. */
	emitmode = 1;
	glUniform1ui(emitmodeID, emitmode);
	lightSourceModel->draw();

	modelScale.pop();
	modelTranslate.pop();
	emitmode = 0;

	glDisableVertexAttribArray(0);
	glUseProgram(0);

	/* Draw snow animation. */
	snowModel->drawParticles(Projection, View);
}

/* Window resize callback. */
static void resizeWindow(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	aspect_ratio = ((float)w / 640.f*4.f) / ((float)h / 480.f*3.f);
}

/* An error callback function to output GLFW errors*/
static void errorCallback(int error, const char* description)
{
	fputs(description, stderr);
}

/* change view angle, exit upon ESC */
static void keyCallback(GLFWwindow* window, int k, int s, int action, int mods)
{
	if (k == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	/* Valid input START*/
	if (k == 'K') wingAngle_inc += 0.05f;
	if (k == 'L') wingAngle_inc -= 0.05f;
	if (k == 'Z') zoom -= 0.07f;
	if (k == 'X') zoom += 0.07f;

	if (zoom < 0.0)
	{
		zoom = 0.001;
	}

	if (k == 'F' && action != GLFW_PRESS)
	{
		fogmode++;

		if (fogmode > 2)
		{
			fogmode = 0;
		}

		switch (fogmode)
		{
			case 1: std::cout << "\n\tLinear fog." << std::endl; break;
			case 2: std::cout << "\n\tFog from the super bible." << std::endl; break;

			default: std::cout << "\n\tFog disabled." << std::endl; break;
		}
	}


	if (k == 'O' || k == 'P')
	{
		/* Set the angle for windmill head */

		if (k == 'O')
		{
			head_angle += 2;
		}
		else
		{
			head_angle -= 2;
		}

		if (head_angle > 360.0f || head_angle < -360.0f)
		{
			head_angle = std::fmod(head_angle, 360.0);
		}

		windmill->setHeadAngle(head_angle);
	}

	/* Cycle between drawing vertices, mesh and filled polygons */
	if (k == 'N' && action != GLFW_PRESS)
	{
		drawmode++;
		if (drawmode > 2) drawmode = 0;

		lightSourceModel->setDrawmode(drawmode);
		windmill->setDrawmode(drawmode);
		terrain->setDrawmode(drawmode);
		skyBox->setDrawmode(drawmode);
	}

	if (k == '1') light_x -= 0.05f;
	if (k == '2') light_x += 0.05f;
	if (k == '3') light_y += 0.05f;
	if (k == '4') light_y -= 0.05f;
	if (k == '5') light_z -= 0.05f;
	if (k == '6') light_z += 0.05f;

	if (k == '7') vx -= 1.f;
	if (k == '8') vx += 1.f;
	if (k == '9') vy -= 1.f;
	if (k == '0') vy += 1.f;
	if (k == 'U') vz -= 1.f;
	if (k == 'I') vz += 1.f;

	/* Valid input END */
}

void GraphicsManager::cmdManager()
{
	std::cout << "J.Stikans, Dundee" << std::endl;

	std::cout << "\n\n-----------------General-----------------------------------" << std::endl;
	std::cout << "\tZ - Move the view position in on Z axis." << std::endl;
	std::cout << "\tX - Move the view position out on Z axis.\n" << std::endl;
	std::cout << "\tK - Increase counterclockwise roatation speed for the windmill sails." << std::endl;
	std::cout << "\tL - Increase clockwise roatation speed for the windmill sails.\n" << std::endl;
	std::cout << "\tO - Rotate the windmill head (roof and sails) counterclockwise." << std::endl;
	std::cout << "\tP - Rotate the windmill head (roof and sails) clockwise." << std::endl;
	std::cout << "\n\n-----------------Lighting---------------------------------" << std::endl;
	std::cout << "\t1 - Move light source to the left on X axis." << std::endl;
	std::cout << "\t2 - Move light source to the right on X axis." << std::endl;
	std::cout << "\t3 - Move light source up on Y axis." << std::endl;
	std::cout << "\t4 - Move light source downd on Y axis." << std::endl;
	std::cout << "\t5 - Move light source further away on Z axis." << std::endl;
	std::cout << "\t6 - Move light source closer on Z axis" << std::endl;
	std::cout << "\n\n-----------------Whole scene------------------------------" << std::endl;
	std::cout << "\t7, 8 - Rotate the scene around X axis." << std::endl;
	std::cout << "\t9, 0 - Rotate the scene around Y axis." << std::endl;
	std::cout << "\tU, I - Rotate the scene around Z axis." << std::endl;
	std::cout << "\n\n-----------------Other------------------------------------" << std::endl;
	std::cout << "\tN - Toggle between draw modes." << std::endl;
	std::cout << "\tF - Cycle through fog modes." << std::endl;
}