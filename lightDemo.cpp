//
// AVT demo light 
// based on demos from GLSL Core Tutorial in Lighthouse3D.com   
//
// This demo was built for learning purposes only.
// Some code could be severely optimised, but I tried to
// keep as simple and clear as possible.
//
// The code comes with no warranties, use it at your own risk.
// You may use it, or parts of it, wherever you want.
//

#include <math.h>
#include <sstream>
#include <iostream>
#include <stdio.h>  

#include <string>
#include <math.h> 
#include <cassert>
#include <array>
// include GLEW to access OpenGL 3.3 functions
#include <GL/glew.h>

// GLUT is the toolkit to interface with the OS
#include <GL/freeglut.h>

// Use Very Simple Libs
#include "VSShaderlib.h"
#include "AVTmathLib.h"
#include "VertexAttrDef.h"
#include "basic_geometry.h"
#include "Car.h"
#include "Table.h"
#include "LightSource.h"
#include "camera.h"
#include "Cheerio.h"
#include "ButterPacket.h"
#include "Orange.h"

#include "TGA.h"
#include "TextureMappedFont.h"

#define PI 3.1415289
#define CAPTION "AVT Light Demo"
#define frand()			((float)rand()/RAND_MAX)

#define SPOT_LIGHTS 	2
#define DIRECTIONAL 	1
#define POINT_LIGHTS 	6 //change this value when pointlights are added to the program

#define NUM_CHEERIOS 120
#define NUM_OBJS 10

#define SKYBOX 6

#define MAX_PARTICLES 10
#define PARTICLE 1
#define NUM_PACKETS 5
#define NUM_ORANGES 6

#define TOTAL_LIGHTS SPOT_LIGHTS + DIRECTIONAL + POINT_LIGHTS 

int WindowHandle = 0;
int WinX = 640, WinY = 480;

unsigned int FrameCount = 0;

float ratio = 0;
float* pratio = &ratio;
camera* cam = new camera();

VSShaderLib shader;

const int numObjs = NUM_OBJS + NUM_CHEERIOS + NUM_PACKETS + NUM_ORANGES + SKYBOX;

struct MyMesh mesh[numObjs];
int objId = 0; //id of the object mesh - to be used as index of mesh: mesh[objID] means the current mesh


Car* car;
Table* table;
LightSource* candles[POINT_LIGHTS];
std::array<Cheerio, NUM_CHEERIOS> trackLimit;
std::array<Object, SKYBOX> skybox;
std::array<ButterPacket, NUM_PACKETS> obstacles;
std::array<Orange, NUM_ORANGES> oranges;
enum direction { back, forward, left, right };
bool move_forward = false, move_back = false, rot_left = false, rot_right = false;

//External array storage defined in AVTmathLib.cpp

/// The storage for matrices
extern float mMatrix[COUNT_MATRICES][16];
extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];

/// The normal matrix
extern float mNormal3x3[9];

GLint pvm_uniformId;
GLint vm_uniformId;
GLint normal_uniformId;
GLint lPos_uniformId;
GLint lSpotDir_uniformId;
GLint lSpotCutOff_uniformId;
//GLint numLights_uniformId, numLights1_uniformId;
GLint spotOn_uniformId;
GLint directionalLightOn_uniformId;
GLint pointLightOn_uniformId;

GLint tex_loc, tex_loc1, tex_loc2, tex_locParticle, texMode_uniformId;
GLuint TextureArray[4];

// Font variables
/**TEXTUREMAPPEDFONT*/
TextureMappedFont* font1;
char l_string[MAX_PATH] = { '\0' };
int numberFonts = 1;

// Lights variables
bool spotLightsOn = true, canChangeSpot = true;

// Interface variables
bool pause = false, canPause = true, gameOver = false;
int numberLifes = 5, points = 0;

// Cameras Position
float camX, camY, camZ;
float FOcamX, FOcamY, FOcamZ;
float FPcamX, FPcamY, FPcamZ;

// Mouse Tracking Variables
int startX, startY, tracking = 0;

// Camera Spherical Coordinates
float alpha = 39.0f, beta = 51.0f;
float r = 10.0f;

// Frame counting and FPS computation
long myTime, timebase = 0, frame = 0;
char s[32];
bool isDirLightOn = true;
bool pointLightOn = true;
float directionalLightPos[4] = { 4.0f, 6.0f, 2.0f, 0.0f };//switched from the default pointlight(w = 1) to a directional light (w = 0)

//particle system
bool fireworks = false;

//test variables
int angle = 0;
int numCollisions = 0;

typedef struct {
	float	life;		// vida
	float	fade;		// fade
	float	r, g, b;    // color
	GLfloat x, y, z;    // posi��o
	GLfloat vx, vy, vz; // velocidade 
	GLfloat ax, ay, az; // acelera��o
} Particle;

Particle fireworkSystem[MAX_PARTICLES];
int dead_num_particles = 0;

// ------------------------------------------------------------
//
// Timer Callback Functions
//

void Timer(int value)
{
	angle++;
	glutPostRedisplay();
	glutTimerFunc(100, Timer, 0);
}

void timer(int value)
{
	std::ostringstream oss;
	oss << CAPTION << ": " << FrameCount << " FPS @ (" << WinX << "x" << WinY << ")";
	std::string s = oss.str();
	glutSetWindow(WindowHandle);
	glutSetWindowTitle(s.c_str());
	FrameCount = 0;
	glutTimerFunc(1000, timer, 0);
}

void refresh(int value)
{
	glutPostRedisplay();
	glutTimerFunc(1000 / 60, refresh, 0);
}

//used to update the fireworks
void iterate(int value)
{
	int i;
	float h;

	/* M�todo de Euler de integra��o de eq. diferenciais ordin�rias
	h representa o step de tempo; dv/dt = a; dx/dt = v; e conhecem-se os valores iniciais de x e v */

	h = 0.125f;
	//h = 0.033;
	if (fireworks) {

		for (i = 0; i < MAX_PARTICLES; i++)
		{
			fireworkSystem[i].x += (h*fireworkSystem[i].vx);
			fireworkSystem[i].y += (h*fireworkSystem[i].vy);
			fireworkSystem[i].z += (h*fireworkSystem[i].vz);
			fireworkSystem[i].vx += (h*fireworkSystem[i].ax);
			fireworkSystem[i].vy += (h*fireworkSystem[i].ay);
			fireworkSystem[i].vz += (h*fireworkSystem[i].az);
			fireworkSystem[i].life -= fireworkSystem[i].fade;
		}
		glutPostRedisplay();
		glutTimerFunc(33, iterate, 0);
	}
}
  
void timerOrange(int value)
{
	for (int i = 0; i < NUM_ORANGES; i++)
	{
		oranges[i].updateSpeed();
	}
	glutTimerFunc(3000, timerOrange, 0);
}

// ------------------------------------------------------------
//
// Reshape Callback Function
//

void changeSize(int w, int h) {
	// Prevent a divide by zero, when window is too short
	if (h == 0)
		h = 1;
	// set the viewport to be the entire window
	glViewport(0, 0, w, h);
	// set the projection matrix
	*pratio = (1.0f * w) / h;
	loadIdentity(PROJECTION);
	cam->updateProjection(pratio);
}


// ------------------------------------------------------------
//
// Render stufff
//

void checkMovements() {
	if (!pause && !gameOver) {
		if (move_forward) car->move(forward);
		else if (move_back) car->move(back);
		if (rot_left) car->rotate(left);
		else if (rot_right) car->rotate(right);
	}
}

void sendMaterial(int index) {
	GLint loc;
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
	glUniform4fv(loc, 1, mesh[index].mat.ambient);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
	glUniform4fv(loc, 1, mesh[index].mat.diffuse);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
	glUniform4fv(loc, 1, mesh[index].mat.specular);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
	glUniform1f(loc, mesh[index].mat.shininess);
}

void drawObj(int index, int texMode) {
	glUniform1i(texMode_uniformId, texMode);
	glBindVertexArray(mesh[index].vao);
	glDrawElements(mesh[index].type, mesh[index].numIndexes, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void sendMatrices() {
	computeDerivedMatrix(PROJ_VIEW_MODEL);
	glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
	glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
	computeNormalMatrix3x3();
	glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);
}

// called when directional light is toggled on/off, sends the new value to the shader
void sendDirectionalLightToggle()
{
	if (isDirLightOn)
		glUniform1i(directionalLightOn_uniformId, 0);
	else
		glUniform1i(directionalLightOn_uniformId, 1);
	isDirLightOn = !isDirLightOn;
}

void toggleCandles()
{
	if (pointLightOn)
		glUniform1i(pointLightOn_uniformId, 0);
	else
		glUniform1i(pointLightOn_uniformId, 1);
	pointLightOn = !pointLightOn;
}

void checkHeadlights() {
	if (spotLightsOn) {
		glUniform1i(spotOn_uniformId, 1);
	}
	else {
		glUniform1i(spotOn_uniformId, 0);
	}
}

void checkLifes();

void checkCollisions() 
{
	int i;

	//check collision with table, if the car goes off the table, the player loses a life and respawns in the initial pos
	if (!car->checkCollision(*car, *table) && numberLifes > 0){
		numberLifes--;
		checkLifes();
	}
	//check if the car is colliding with any cheerio
	for (i = 0; i < trackLimit.size(); i++) 
	{
		if (car->checkCollision(*car, trackLimit[i])) {
			car->collided(trackLimit[i]);
			trackLimit[i].collided(car->position);
			numCollisions++;
		}
	}

	for (i = 0; i < obstacles.size(); i++)
	{
		if (car->checkCollision(*car, obstacles[i])) {
			car->collided(obstacles[i]);
			numCollisions++;
		}
	}
	for (i = 0; i < oranges.size(); i++)
	{
		if (car->checkCollision(*car, oranges[i])) {
			//car->collided(oranges[i]);
			oranges[i].collided();
			numberLifes--;
			checkLifes();
			car->respawn();
			numCollisions++;
		}
		for (int j = 0; j < obstacles.size(); j++) {
			if (oranges[i].checkCollision(oranges[i], obstacles[j]))
				oranges[i].collided();
		}
	}
}
  
void drawLights() {
	glUniform1i(directionalLightOn_uniformId, isDirLightOn); // update the switch in the shader (makes no sense)
	glUniform1i(pointLightOn_uniformId, pointLightOn);
	float res[4];
	float dir[4];
	//glUniform1i(numLights_uniformId, (GLint)TOTAL_LIGHTS);
	//glUniform1i(numLights1_uniformId, (GLint)TOTAL_LIGHTS);
	for (int i = 0; i < SPOT_LIGHTS; i++) {
		multMatrixPoint(VIEW, car->headlights[i]->l_position, res);
		glUniform4fv(glGetUniformLocation(shader.getProgramIndex(), ("lightsOUT[" + std::to_string(i) + "].l_pos").c_str()), 1, res);
		multMatrixPoint(VIEW, car->headlights[i]->direction, dir);
		glUniform4fv(glGetUniformLocation(shader.getProgramIndex(), ("lightsOUT[" + std::to_string(i) + "].l_spotDir").c_str()), 1, dir);
		//glUniform4fv(glGetUniformLocation(shader.getProgramIndex(), ("lightsOUT[" + std::to_string(i) + "].l_spotDir").c_str()), 1, car->headlights[i]->direction);
		glUniform1f(glGetUniformLocation(shader.getProgramIndex(), ("lightsOUT[" + std::to_string(i) + "].l_cutoff").c_str()), car->headlights[i]->spot_cutoff);
		glUniform1i(glGetUniformLocation(shader.getProgramIndex(), ("lightsOUT[" + std::to_string(i) + "].type").c_str()), 1);
		//std::cout << i << std::endl;
	}

	multMatrixPoint(VIEW, directionalLightPos, res);   //lightPos definido em World Coord so is converted to eye space
	glUniform4fv(glGetUniformLocation(shader.getProgramIndex(), ("lightsOUT[" + std::to_string(SPOT_LIGHTS) + "].l_pos").c_str()), 1, res);
	glUniform1i(glGetUniformLocation(shader.getProgramIndex(), ("lightsOUT[" + std::to_string(SPOT_LIGHTS) + "].type").c_str()), 2);
	//std::cout << SPOT_LIGHTS << std::endl;

	for (int j = 0; j < POINT_LIGHTS; j++) {
		//if (j == 2) { candles[j]->l_position[0] += angle; }
		multMatrixPoint(VIEW, candles[j]->l_position, res);   //lightPos definido em World Coord so is converted to eye space
		//std::cout << candles[j]->l_position[0] << ", " << candles[j]->l_position[1] << ", " << candles[j]->l_position[2] << std::endl;
		glUniform4fv(glGetUniformLocation(shader.getProgramIndex(), ("lightsOUT[" + std::to_string(j+SPOT_LIGHTS+DIRECTIONAL) + "].l_pos").c_str()), 1, res);
		glUniform1i(glGetUniformLocation(shader.getProgramIndex(), ("lightsOUT[" + std::to_string(j+SPOT_LIGHTS+DIRECTIONAL) + "].type").c_str()), 0);
		//std::cout << j + SPOT_LIGHTS + DIRECTIONAL << std::endl;
	}

}

void drawFonts() {
	std::string s = std::to_string(numberLifes);
	char const *pchar = s.c_str();

	std::string s1 = std::to_string(points);
	char const *pchar1 = s1.c_str();

	sprintf_s(l_string, "Lifes %s", pchar);
	font1->DrawString(WinX / 2 - 60, WinY - 20, l_string, false);
	sprintf_s(l_string, "Points %s", pchar1);
	font1->DrawString(WinX / 2 + 60, WinY - 20, l_string, false);

	if (!gameOver) {
		sprintf_s(l_string, "Pause %s", "");
		font1->DrawString(WinX / 2, WinY / 2, l_string, !pause);
	}
	else if (gameOver) {
		sprintf_s(l_string, "Game Over ", "");
		font1->DrawString(WinX / 2, WinY / 2, l_string, false);
	}
}

void activateTextures() {
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, TextureArray[2]);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, TextureArray[1]);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, TextureArray[3]);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, TextureArray[4]);

	glUniform1i(tex_loc, 0+numberFonts);
	glUniform1i(tex_loc1, 1+numberFonts);
}

void checkLifes() {
	if (numberLifes <= 0) {
		numberLifes = 0;
		gameOver = true;
	}
	else {
		car->respawn();
	}
}

void restart() {
	numberLifes = 5;
	points = 0;
	pause = false;
	canPause = true;
	spotLightsOn = true;
	canChangeSpot = true;
	isDirLightOn = true;
	car->respawn();
}

void renderScene(void) {

	GLint loc;
	FrameCount++;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// load identity matrices
	loadIdentity(VIEW);
	loadIdentity(MODEL);
	// set the camera using a function similar to gluLookAt
	//lookAt(camX, camY, camZ, 0, 0, 0, 0, 1, 0);
	if (cam->activeCam == cam->MOVINGPERSPECTIVE)
		cam->sendCamCoords(camX, camY, camZ);
	cam->updateLookAt(car->position);

	float particle_color[4];

	// use our shader
	shader.Use();

	//textures
	activateTextures();
	checkMovements();
	checkHeadlights();

	checkCollisions();
	drawLights();

	//draw car body	
	sendMaterial(car->objId);
	pushMatrix(MODEL);
	translate(MODEL,car->position[0],car->position[1],car->position[2]);
	rotate(MODEL,car->rotation,car->rotationAxis[0],car->rotationAxis[1],car->rotationAxis[2]);
	sendMatrices();
	drawObj(car->objId,0);

	//draw car wheels
	for (int i = 0; i < 4; i++) {
		Wheel*w = car->wheels[i];
		pushMatrix(MODEL);
		sendMaterial(w->objId);
		translate(MODEL, w->position[0], w->position[1], w->position[2]);
		rotate(MODEL,w->currentRot,w->movementRotAxis[0], w->movementRotAxis[1], w->movementRotAxis[2]);
		rotate(MODEL, w->rotation,w->rotationAxis[0],w->rotationAxis[1],w->rotationAxis[2]);
		sendMatrices();
		drawObj(w->objId,0);
		popMatrix(MODEL);

	}
	popMatrix(MODEL);

	//draw trackLimiter
	for (int i = 0; i < trackLimit.size(); i++) {
		pushMatrix(MODEL);
		sendMaterial(trackLimit[i].objId);
		translate(MODEL, trackLimit[i].position[0], trackLimit[i].position[1], trackLimit[i].position[2]);
		rotate(MODEL, trackLimit[i].rotation, trackLimit[i].rotationAxis[0], trackLimit[i].rotationAxis[1], trackLimit[i].rotationAxis[2]);
		sendMatrices();
		drawObj(trackLimit[i].objId, 0);
		popMatrix(MODEL);
	}
	//draw skybox
	for (int i = 0; i < skybox.size(); i++) {
		pushMatrix(MODEL);
		sendMaterial(skybox[i].objId);
		translate(MODEL, skybox[i].position[0], skybox[i].position[1], skybox[i].position[2]);
		//rotate(MODEL, w->currentRot, w->movementRotAxis[0], w->movementRotAxis[1], w->movementRotAxis[2]);
		//rotate(MODEL, skybox[i].rotation, skybox[i].rotationAxis[0], skybox[i].rotationAxis[1], skybox[i].rotationAxis[2]);
		rotate(MODEL, skybox[i].rotation, skybox[i].rotationAxis[0], skybox[i].rotationAxis[1], skybox[i].rotationAxis[2]);
		sendMatrices();
		drawObj(skybox[i].objId, 0);
		popMatrix(MODEL);
	}

	//draw obstacles
	for (int i = 0; i < obstacles.size(); i++) {
		pushMatrix(MODEL);
		sendMaterial(obstacles[i].objId);
		//
		translate(MODEL, obstacles[i].position[0], obstacles[i].position[1], obstacles[i].position[2]);
		rotate(MODEL, obstacles[i].rotation, obstacles[i].rotationAxis[0], obstacles[i].rotationAxis[1], obstacles[i].rotationAxis[2]);
		scale(MODEL, 2.0, 1.0, 1.0);
		sendMatrices();
		drawObj(obstacles[i].objId, 0);
		popMatrix(MODEL);
	}

	//draw oranges
	for (int i = 0; i < oranges.size(); i++) {
		pushMatrix(MODEL);
		sendMaterial(oranges[i].objId);
		oranges[i].update();
		translate(MODEL, oranges[i].position[0], oranges[i].position[1], oranges[i].position[2]);
		rotate(MODEL, oranges[i].rotation, oranges[i].rotationAxis[0], oranges[i].rotationAxis[1], oranges[i].rotationAxis[2]);
		sendMatrices();
		drawObj(oranges[i].objId, 0);
		popMatrix(MODEL);
	}
	
	//draw table
	//popMatrix(MODEL);
	sendMaterial(table->objId);
	//translate(MODEL, table->position[0], table->position[1], table->position[2]);
	rotate(MODEL,table->rotation,table->rotationAxis[0], table->rotationAxis[1], table->rotationAxis[2]);
	sendMatrices();
	drawObj(table->objId,2);
	shader.UnUse();

	drawFonts();

	if (fireworks) {
		// draw fireworks particles
		objId++;  //quad for particle



		/*//////////////////////////////

		glUniform1i(tex_loc, 0 + numberFonts);
		glUniform1i(tex_loc1, 1 + numberFonts);
		//////////////////////////////////*/
		//CHANGE THIS TEXTURE ARRAY INDEX
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureArray[0]); //particle.bmp associated to TU0 
		glDisable(GL_DEPTH_TEST); /* n�o interessa o z-buffer: as part�culas podem ser desenhadas umas por cima das outras sem problemas de ordena��o */

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		glUniform1i(tex_locParticle, 1);
		glUniform1i(texMode_uniformId, 2); // draw modulated textured particles 

		for (int i = 0; i < MAX_PARTICLES; i++)
		{
			if (fireworkSystem[i].life > 0.0f) /* s� desenha as que ainda est�o vivas */
			{

				/* A vida da part�cula representa o canal alpha da cor. Como o blend est� activo a cor final � a soma da cor rgb do fragmento multiplicada pelo
				alpha com a cor do pixel destino */

				particle_color[0] = fireworkSystem[i].r;
				particle_color[1] = fireworkSystem[i].g;
				particle_color[2] = fireworkSystem[i].b;
				particle_color[3] = fireworkSystem[i].life;

				// send the material - diffuse color modulated with texture
				loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
				glUniform4fv(loc, 1, particle_color);

				pushMatrix(MODEL);
				translate(MODEL, fireworkSystem[i].x, fireworkSystem[i].y, fireworkSystem[i].z);

				// send matrices to OGL
				computeDerivedMatrix(PROJ_VIEW_MODEL);
				glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
				glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
				computeNormalMatrix3x3();
				glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

				glBindVertexArray(mesh[objId].vao);
				glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
				popMatrix(MODEL);
			}
			else dead_num_particles++;
		}
		if (dead_num_particles == MAX_PARTICLES) {
			fireworks = false;
			dead_num_particles = 0;
			printf("All particles dead\n");
		}

	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glutSwapBuffers();
}

void initParticles(void)
{
	GLfloat v, theta, phi;
	int i;

	for (i = 0; i<MAX_PARTICLES; i++)
	{
		v = 0.8*frand() + 0.2;
		phi = frand()*PI;
		theta = 2.0*frand()*PI;

		fireworkSystem[i].x = 0.0f;
		fireworkSystem[i].y = 2.0f;
		fireworkSystem[i].z = 0.0f;
		fireworkSystem[i].vx = v * cos(theta) * sin(phi);
		fireworkSystem[i].vy = v * cos(phi);
		fireworkSystem[i].vz = v * sin(theta) * sin(phi);
		fireworkSystem[i].ax = 0.1f; /* simular um pouco de vento */
		fireworkSystem[i].ay = -0.15f; /* simular a acelera��o da gravidade */
		fireworkSystem[i].az = 0.0f;

		/* tom amarelado que vai ser multiplicado pela textura que varia entre branco e preto */
		fireworkSystem[i].r = 0.882f;
		fireworkSystem[i].g = 0.552f;
		fireworkSystem[i].b = 0.211f;

		fireworkSystem[i].life = 1.0f;		/* vida inicial */
		fireworkSystem[i].fade = 0.005f;	    /* step de decr�scimo da vida para cada itera��o */
	}
}

// ------------------------------------------------------------
//
// Events from the Keyboard
//

void processKeys(unsigned char key, int xx, int yy)
{
	//std::cout << key << std::endl;
	switch (key) {

	case 27:
		glutLeaveMainLoop();
		break;

	case 'c':
		toggleCandles();
		printf("Camera Spherical Coordinates (%f, %f, %f)\n", alpha, beta, r);
		break;
	case 'm': glEnable(GL_MULTISAMPLE); break;
	//case 'n': glDisable(GL_MULTISAMPLE); break;
	case 'a':
		move_forward = false;
		move_back = true;
		break;
	case 'q':
		move_forward = true; 
		move_back = false;
		break;
	case 'o': 
		rot_left = true; 
		rot_right = false; 
		break;
	case 'p': 
		rot_left = false; 
		rot_right = true; 
		break;
	case 'h':
		if (canChangeSpot && !pause) {
			spotLightsOn = !spotLightsOn;
			canChangeSpot = false;
		}
		break; 
	case '1':
		if (!pause) {
			cam->setFixedOrtho();
			cam->setCameraType(cam->FIXEDORTHO);
		}
		break;
	case '2':
		if (!pause) {
			cam->setFixedPerspective(pratio);
			cam->setCameraType(cam->FIXEDPERSPECTIVE);
		}
		break;
	case '3':
		if (!pause) {
			cam->setMovingPerspective(pratio);
			cam->setCameraType(cam->MOVINGPERSPECTIVE);
		}
		break;
	case 'n':
		if(!pause)		sendDirectionalLightToggle();
		break;
	case 's':
		if (canPause) {
			pause = !pause;
			canPause = false;
		}
		break;
	case 'r':
		if (gameOver) {
			gameOver = !gameOver;
			restart();
		}
		break;
	case 'e':
		fireworks = true;
		initParticles();
		glutTimerFunc(0, iterate, 0);  //timer for particle system
		break;
	}

}

void processKeysUp(unsigned char key, int xx, int yy)
{
	switch (key) {

	case 'a':
		move_back = false;
		break;
	case 'q':
		move_forward = false;
		break;
	case 'o':
		rot_left = false;
		break;
	case 'p':
		rot_right = false;
		break;
	case 'h':
		canChangeSpot = true;
		break;
	case 's':
		canPause = true;
		break;
	}

}


// ------------------------------------------------------------
//
// Mouse Events
//

void processMouseButtons(int button, int state, int xx, int yy)
{
	// start tracking the mouse
	if (state == GLUT_DOWN) {
		startX = xx;
		startY = yy;
		if (button == GLUT_LEFT_BUTTON)
			tracking = 1;
		else if (button == GLUT_RIGHT_BUTTON)
			tracking = 2;
	}

	//stop tracking the mouse
	else if (state == GLUT_UP) {
		if (tracking == 1) {
			alpha -= (xx - startX);
			beta += (yy - startY);
		}
		else if (tracking == 2) {
			r += (yy - startY) * 0.01f;
			if (r < 0.1f)
				r = 0.1f;
		}
		tracking = 0;
	}
}

// Track mouse motion while buttons are pressed

void processMouseMotion(int xx, int yy)
{

	int deltaX, deltaY;
	float alphaAux, betaAux;
	float rAux;

	deltaX = -xx + startX;
	deltaY = yy - startY;

	// left mouse button: move camera
	if (tracking == 1) {
		alphaAux = alpha + deltaX;
		betaAux = beta + deltaY;

		if (betaAux > 85.0f)
			betaAux = 85.0f;
		else if (betaAux < -85.0f)
			betaAux = -85.0f;
		rAux = r;
	}
	// right mouse button: zoom
	else if (tracking == 2) {

		alphaAux = alpha;
		betaAux = beta;
		rAux = r + (deltaY * 0.01f);
		if (rAux < 0.1f)
			rAux = 0.1f;
	}

	camX = rAux * sin(alphaAux * 3.14f / 180.0f) * cos(betaAux * 3.14f / 180.0f);
	camZ = rAux * cos(alphaAux * 3.14f / 180.0f) * cos(betaAux * 3.14f / 180.0f);
	camY = rAux *   						       sin(betaAux * 3.14f / 180.0f);

	//  uncomment this if not using an idle func
	//	glutPostRedisplay();
}


void mouseWheel(int wheel, int direction, int x, int y) {

	r += direction * 0.1f;
	if (r < 0.1f)
		r = 0.1f;

	camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camY = r *   						     sin(beta * 3.14f / 180.0f);

	//  uncomment this if not using an idle func
	//	glutPostRedisplay();
}

// --------------------------------------------------------
//
// Shader Stuff
//

GLuint setupShaders() {

	// Shader for models
	shader.init();
	shader.loadShader(VSShaderLib::VERTEX_SHADER, "..\\shaders\\lights.vert");
	shader.loadShader(VSShaderLib::FRAGMENT_SHADER, "..\\shaders\\lights.frag");
  
	// set semantics for the shader variables
	glBindFragDataLocation(shader.getProgramIndex(), 0, "colorOut");
	glBindAttribLocation(shader.getProgramIndex(), VERTEX_COORD_ATTRIB, "position");
	glBindAttribLocation(shader.getProgramIndex(), NORMAL_ATTRIB, "normal");
	glBindAttribLocation(shader.getProgramIndex(), TEXTURE_COORD_ATTRIB, "texCoord");

	glLinkProgram(shader.getProgramIndex());

	pvm_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_pvm");
	vm_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_viewModel");
	normal_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_normal");
	//lights
	//numLights_uniformId = glGetUniformLocation(shader.getProgramIndex(), "numLights");
	//numLights1_uniformId = glGetUniformLocation(shader.getProgramIndex(), "numLights1");
	spotOn_uniformId = glGetUniformLocation(shader.getProgramIndex(), "spotOn");
	lPos_uniformId = glGetUniformLocation(shader.getProgramIndex(), "l_pos");
	directionalLightOn_uniformId = glGetUniformLocation(shader.getProgramIndex(), "directionalLightOn");
	glUniform1i(directionalLightOn_uniformId, 1);
	pointLightOn_uniformId = glGetUniformLocation(shader.getProgramIndex(), "pointLightOn");
	glUniform1i(pointLightOn_uniformId, 1);

	tex_loc = glGetUniformLocation(shader.getProgramIndex(), "texmap");
	tex_loc1 = glGetUniformLocation(shader.getProgramIndex(), "texmap1");
	tex_locParticle = glGetUniformLocation(shader.getProgramIndex(), "texParticle");
	texMode_uniformId = glGetUniformLocation(shader.getProgramIndex(), "texMode"); // different modes of texturing

	printf("InfoLog for Hello World Shader\n%s\n\n", shader.getAllInfoLogs().c_str());

	return(shader.isProgramLinked());
}

void createCar2() {
	
	car = new Car();
	car->init(0, car->initialPos, car->RotAxis, car->initialRot);
	//car.setColor(mesh);
	objId = car->objId;
	memcpy(mesh[car->objId].mat.ambient, car->amb, 4 * sizeof(float));
	memcpy(mesh[car->objId].mat.diffuse, car->diff, 4 * sizeof(float));
	memcpy(mesh[car->objId].mat.specular, car->spec, 4 * sizeof(float));
	memcpy(mesh[car->objId].mat.emissive, car->emissive, 4 * sizeof(float));
	mesh[car->objId].mat.shininess = car->shininess;
	mesh[car->objId].mat.texCount = car->texcount;
	createCube();

	for (int i = 0; i < 4; i++) {
		Wheel* w = car->wheels[i];
		objId = w->objId;
		memcpy(mesh[w->objId].mat.ambient, w->amb, 4 * sizeof(float));
		memcpy(mesh[w->objId].mat.diffuse, w->diff, 4 * sizeof(float));
		memcpy(mesh[w->objId].mat.specular, w->spec, 4 * sizeof(float));
		memcpy(mesh[w->objId].mat.emissive, w->emissive, 4 * sizeof(float));
		mesh[w->objId].mat.shininess = w->shininess;
		mesh[w->objId].mat.texCount = w->texcount;
		createTorus(w->initValues[0], w->initValues[1], w->initValues[2], w->initValues[3]);
	}
	objId++;
	memcpy(mesh[objId].mat.ambient, car->amb, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, car->diff, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, car->spec, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, car->emissive, 4 * sizeof(float));
	mesh[objId].mat.shininess = car->shininess;
	mesh[objId].mat.texCount = car->texcount;
	createCube();
	//createTorus(0.1f, 0.2f, 15, 5);
		//	


}

void createTable() {
	float rot = -90;
	float rotAxis[3] = { 1.0f,0.0f,0.0f };
	float pos[3] = {0,-0.1f,0};
	objId++;
	table = new Table();
	table->init(objId,pos,rotAxis,rot);
	memcpy(mesh[table->objId].mat.ambient, table->amb,4*sizeof(float));
	memcpy(mesh[table->objId].mat.diffuse, table->diff,4*sizeof(float));
	memcpy(mesh[table->objId].mat.specular, table->spec,4*sizeof(float));
	memcpy(mesh[table->objId].mat.emissive, table->emissive,4*sizeof(float));
	mesh[table->objId].mat.shininess = table->shininess;
	mesh[table->objId].mat.texCount = table->texcount;
	createQuad(table->x,table->y);
}

void createTrack() 
{
	float spaceBetCheerios = 1.7;
	float initialDeviationX = table->x / 3;

	float rot = 0;
	float rotAxis[3] = { 1.0f,0.0f,0.0f };
	float pos[3] = { - initialDeviationX, 0, 0 };


	for (int i = 0; i < trackLimit.size(); i++)
	{
		if (i < 10){	// z++
			pos[0] += spaceBetCheerios;
			pos[2] += spaceBetCheerios;
		}else if (i >= 10 && i < 20)
		{
			pos[0] += spaceBetCheerios;
			pos[2] -= spaceBetCheerios;
		}
		else if (i >= 20 && i < 30)
		{
			pos[0] -= spaceBetCheerios;
			pos[2] -= spaceBetCheerios;
		}
		else if (i >= 30 && i < 40)
		{
			pos[0] -= spaceBetCheerios; //update the x coord of each new cheerio
			pos[2] += spaceBetCheerios;
		}
		if (i == 40) {
			pos[0] -= 10;
			pos[2] += spaceBetCheerios;
		}
		else if (i > 40 && i < 55) 
		{
			pos[0] += spaceBetCheerios;
			pos[2] += spaceBetCheerios;
		}if (i == 55)
			pos[0] += 5;
		else if(i> 55 && i <70)
		{
			pos[0] += spaceBetCheerios;
			pos[2] -= spaceBetCheerios;
		}
		if (i == 70)
			pos[2] -= 5;
		else if (i > 70 && i <85)
		{
			pos[0] -= spaceBetCheerios;
			pos[2] -= spaceBetCheerios;
		}
		else if (i >= 85 && i < 100)
		{
			pos[0] -= spaceBetCheerios; //update the x coord of each new cheerio
			pos[2] += spaceBetCheerios;
		}
			objId++;
		trackLimit[i] = Cheerio(objId, pos, rotAxis, rot);
		memcpy(mesh[trackLimit[i].objId].mat.ambient, trackLimit[i].amb, 4 * sizeof(float));
		memcpy(mesh[trackLimit[i].objId].mat.diffuse, trackLimit[i].diff, 4 * sizeof(float));
		memcpy(mesh[trackLimit[i].objId].mat.specular, trackLimit[i].spec, 4 * sizeof(float));
		memcpy(mesh[trackLimit[i].objId].mat.emissive, trackLimit[i].emissive, 4 * sizeof(float));
		mesh[trackLimit[i].objId].mat.shininess = trackLimit[i].shininess;
		mesh[trackLimit[i].objId].mat.texCount = trackLimit[i].texcount;
		createTorus(trackLimit[i].initValues[0], trackLimit[i].initValues[1], trackLimit[i].initValues[2], trackLimit[i].initValues[3]);
	}
}

void createLights() {

	for (int i = 0; i < POINT_LIGHTS; i++) {
		candles[i] = new LightSource();
		float pos[4] = { rand() % 40 -20, .5f, rand() % 40 -20, 1.0f };
		candles[i]->setPoint(pos);
	}

}

void createTextures() {
	glGenTextures(3, TextureArray);
	TGA_Texture(TextureArray, "..//stone.tga", 0);
	TGA_Texture(TextureArray, "..//course1.tga", 1);
	TGA_Texture(TextureArray, "..//lightwood.tga", 2);
	//TGA_Texture(TextureArray, "..//katsbits-rock5//rocks.tga", 3);
}

void createObstacles()
{

	float rot = 0;
	float rotAxis[3] = { 0.0f, 1.0f, 0.0f };
	float pos[3] = { 0.0, 0.5, 0.0 };


	for (int i = 0; i < obstacles.size(); i++)
	{	
		pos[0] = (rand() % 40 - 20);
		pos[2] = (rand() % 40 - 20);

		rot = rand() % 91;
		//std::cout << pos[0] << "||" << pos[2] << '\n';
		objId++;
		obstacles[i] = ButterPacket(objId, pos, rotAxis, rot);
		memcpy(mesh[obstacles[i].objId].mat.ambient, obstacles[i].amb, 4 * sizeof(float));
		memcpy(mesh[obstacles[i].objId].mat.diffuse, obstacles[i].diff, 4 * sizeof(float));
		memcpy(mesh[obstacles[i].objId].mat.specular, obstacles[i].spec, 4 * sizeof(float));
		memcpy(mesh[obstacles[i].objId].mat.emissive, obstacles[i].emissive, 4 * sizeof(float));
		mesh[obstacles[i].objId].mat.shininess = obstacles[i].shininess;
		mesh[obstacles[i].objId].mat.texCount = obstacles[i].texcount;
		createCube();
	}
}

void createOranges()
{

	float rot = 0;
	float rotAxis[3] = { 0.0f, 1.0f, 0.0f };
	float pos[3] = { 0.0, 1.0, 0.0 };

	for (int i = 0; i < oranges.size(); i++)
	{
		pos[0] = (rand() % 40 - 20);
		pos[2] = (rand() % 40 - 20);

		//std::cout << pos[0] << "||" << pos[2] << '\n';
		objId++;
		oranges[i] = Orange(objId, pos, rotAxis, rot);
		memcpy(mesh[oranges[i].objId].mat.ambient, oranges[i].amb, 4 * sizeof(float));
		memcpy(mesh[oranges[i].objId].mat.diffuse, oranges[i].diff, 4 * sizeof(float));
		memcpy(mesh[oranges[i].objId].mat.specular, oranges[i].spec, 4 * sizeof(float));
		memcpy(mesh[oranges[i].objId].mat.emissive, oranges[i].emissive, 4 * sizeof(float));
		mesh[oranges[i].objId].mat.shininess = oranges[i].shininess;
		mesh[oranges[i].objId].mat.texCount = oranges[i].texcount;
		createSphere(oranges[i].radius, oranges[i].divisions);
  }
}

//needs specific materials, a texture, verify that transparency will not oclude game
void createSkybox() {
	float padding = table->x / 5;
	float wallSizeX = table->x * 5;
	float wallSizeY = 80.0f;

	float initialX = 0;
	float initialZ = - table->y * 1.5; 

	float rot = 0;
	float pos[] = {initialX, 0, initialZ};
	float rotAxis[] = { 0, 1.0f, 0 };

	int wallsUsedSoFar = 0;
	
	//values of position and rotation for each wall of the skybox
	for(int i = 0; i < skybox.size()-1; i++){ //taking the last quad (ground quad), since that has a different size
		switch (i) {
		case 0:	//back wall, initial values will do
			pos[0] = initialX;
			pos[2] = initialZ;
			rot = 0;
			break;	 
		case 1: //front wall, blocks the view =(
			pos[0] = 0; 
			pos[2] = -initialZ;
			rot = 0;
			break;
		case 2: //left wall
			pos[0] = - table->x * 1.5;
			pos[2] = 0;
			rot = 90;
			break;
		case 3: //right wall
			pos[0] = table->x * 1.5;
			pos[2] = 0;
			rot = 90;
			break;
		}
		objId++;
		skybox[i] = Cheerio(objId, pos, rotAxis, rot);
		memcpy(mesh[skybox[i].objId].mat.ambient, car[0].amb, 4 * sizeof(float));
		memcpy(mesh[skybox[i].objId].mat.diffuse, car[0].diff, 4 * sizeof(float));
		memcpy(mesh[skybox[i].objId].mat.specular, trackLimit[0].spec, 4 * sizeof(float));
		memcpy(mesh[skybox[i].objId].mat.emissive, trackLimit[0].emissive, 4 * sizeof(float));
		mesh[skybox[i].objId].mat.shininess = trackLimit[0].shininess;
		mesh[skybox[i].objId].mat.texCount = trackLimit[0].texcount;

		createQuad(wallSizeX, wallSizeY);
		wallsUsedSoFar++;
	}

	//floor and ceiling
	for(int i = wallsUsedSoFar; i <= skybox.size(); i++){
		//position the ground bellow the table
		pos[0] = 0;
		if(i == 5)			//floor
			pos[1] = -3;
		if(i == 6)
			pos[1] = 20;	// ceiling ;this value needs to be bigger than the eye position in camera.h
		pos[2] = 0;

		//set the rotation to the xx axis
		rotAxis[1] = 0;		
		rotAxis[0] = 1.0f;
	
		//set the rotation angle
		rot = -90;
		objId++;

		int index = skybox.size() - 2; //starts as if i== 5, with this index we access the ground
		if(i== 6)
			index = skybox.size() - 1;	//with this index we access the floor

		skybox[index] = Cheerio(objId, pos, rotAxis, rot);

		memcpy(mesh[skybox[index].objId].mat.ambient, car[0].amb, 4 * sizeof(float));
		memcpy(mesh[skybox[index].objId].mat.diffuse, car[0].diff, 4 * sizeof(float));
		memcpy(mesh[skybox[index].objId].mat.specular, trackLimit[0].spec, 4 * sizeof(float));
		memcpy(mesh[skybox[index].objId].mat.emissive, trackLimit[0].emissive, 4 * sizeof(float));
		mesh[skybox[index].objId].mat.shininess = trackLimit[0].shininess;
		mesh[skybox[index].objId].mat.texCount = trackLimit[0].texcount;

		createQuad(table->x * 3 , table->y * 3);
	}

}

// ------------------------------------------------------------
//
// Model loading and OpenGL setup
//

void init()
{
	// set the camera position based on its spherical coordinates
	camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camY = r *   						     sin(beta * 3.14f / 180.0f);

	int texcount = 0;

	createTextures();
	createCar2();
	createTable();
	createTrack();
	createLights();
	createSkybox();
	createObstacles();
	createOranges();


	// some GL settings
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	/**TEXTUREMAPPEDFONT*/
	font1 = new TextureMappedFont("..//font1.bmp");
	//BMP_Texture(TextureArray, "particula.bmp", 1); //n�o estamos a usar glbmp para carregar texturas


	objId++;
	mesh[objId].mat.texCount = texcount;
	createQuad(2, 2);
}

// ------------------------------------------------------------
//
// Main function
//


int main(int argc, char **argv) {

	//  GLUT initialization
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);

	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WinX, WinY);
	WindowHandle = glutCreateWindow(CAPTION);


	//  Callback Registration
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	//glutIdleFunc(renderScene);

//	Mouse and Keyboard Callbacks
	glutKeyboardFunc(processKeys);
	glutKeyboardUpFunc(processKeysUp);
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);
	glutMouseWheelFunc(mouseWheel);
	glutTimerFunc(0, timer, 0);
	glutTimerFunc(0, refresh, 0);
	glutTimerFunc(0, Timer, 0);
	glutTimerFunc(0, timerOrange, 0);

	//	return from main loop
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	//	Init GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));
	printf("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	if (!setupShaders())
		return(1);

	init();

	//  GLUT main loop
	glutMainLoop();

	return(0);

}

