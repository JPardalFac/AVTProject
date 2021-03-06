////
//// AVT demo light 
//// based on demos from GLSL Core Tutorial in Lighthouse3D.com   
////
//// This demo was built for learning purposes only.
//// Some code could be severely optimised, but I tried to
//// keep as simple and clear as possible.
////
//// The code comes with no warranties, use it at your own risk.
//// You may use it, or parts of it, wherever you want.
////
//
//#include <math.h>
//#include <sstream>
//#include <iostream>
//#include <stdio.h>  
//
//#include <string>
//#include <math.h> 
//#include <cassert>
//#include <array>
//// include GLEW to access OpenGL 3.3 functions
//#include <GL/glew.h>
//
//// GLUT is the toolkit to interface with the OS
//#include <GL/freeglut.h>
//
//// Use Very Simple Libs
//#include "VSShaderlib.h"
//#include "AVTmathLib.h"
////#include "VertexAttrDef.h"
////#include "basic_geometry.h"
//#include "Car.h"
//#include "Table.h"
//#include "LightSource.h"
//#include "camera.h"
//#include "Cheerio.h"
//
//#include "TGA.h"
//#include "TextureMappedFont.h"
//#include "ObjLoader.h"
//
//#define PI 3.1415289
//#define CAPTION "AVT Light Demo"
//#define SPOT_LIGHTS 	2
//#define DIRECTIONAL 	1
//#define POINT_LIGHTS 	3 //change this value when pointlights are added to the program
//
//#define NUM_CHEERIOS 100
//#define NUM_OBJS 10
//
//#define TOTAL_LIGHTS SPOT_LIGHTS + DIRECTIONAL + POINT_LIGHTS
//
//int WindowHandle = 0;
//int WinX = 640, WinY = 480;
//
//unsigned int FrameCount = 0;
//
//float ratio = 0;
//float* pratio = &ratio;
//camera* cam = new camera();
//
//VSShaderLib shader;
//
//const int numObjs = 1;//NUM_OBJS + NUM_CHEERIOS;
//					  //struct MyMesh mesh[numObjs];
//int objId = 0; //id of the object mesh - to be used as index of mesh: mesh[objID] means the current mesh
//			   //const aiScene* objs[numObjs];
//
//ObjectFromLoad *objects[numObjs];
//
//Car* car;
//Table* table;
//LightSource* candles[POINT_LIGHTS];
//std::array<Cheerio, NUM_CHEERIOS> trackLimit;
//enum direction { back, forward, left, right };
//bool move_forward = false, move_back = false, rot_left = false, rot_right = false;
//
////External array storage defined in AVTmathLib.cpp
//
///// The storage for matrices
//extern float mMatrix[COUNT_MATRICES][16];
//extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];
//
///// The normal matrix
//extern float mNormal3x3[9];
//
//GLint pvm_uniformId;
//GLint vm_uniformId;
//GLint normal_uniformId;
//GLint lPos_uniformId;
//GLint lSpotDir_uniformId;
//GLint lSpotCutOff_uniformId;
//GLint numLights_uniformId, numLights1_uniformId;
//GLint spotOn_uniformId;
//GLint directionalLightOn_uniformId;
//
//
//GLint tex_loc, tex_loc1, tex_loc2, texMode_uniformId;
//GLuint TextureArray[3];
//
//// Font variables
///**TEXTUREMAPPEDFONT*/
//TextureMappedFont* font1;
//char l_string[MAX_PATH] = { '\0' };
//int numberFonts = 1;
//
//// Lights variables
//bool spotLightsOn = true, canChangeSpot = true;
//
//// Interface variables
//bool pause = false, canPause = true, gameOver = false;
//int numberLifes = 5, points = 0;
//
//// Cameras Position
//float camX, camY, camZ;
//float FOcamX, FOcamY, FOcamZ;
//float FPcamX, FPcamY, FPcamZ;
//
//// Mouse Tracking Variables
//int startX, startY, tracking = 0;
//
//// Camera Spherical Coordinates
//float alpha = 39.0f, beta = 51.0f;
//float r = 10.0f;
//
//// Frame counting and FPS computation
//long myTime, timebase = 0, frame = 0;
//char s[32];
//bool isDirLightOn = true;
//float directionalLightPos[4] = { 4.0f, 6.0f, 2.0f, 0.0f };//switched from the default pointlight(w = 1) to a directional light (w = 0)
//
//
//
//														  //test variables
//int angle = 0;
//int numCollisions = 0;
//
//void Timer(int value)
//{
//	//if(numberLifes > 0)
//	//	numberLifes -= 1;
//	angle++;
//	glutPostRedisplay();
//	glutTimerFunc(100, Timer, 0);
//}
//
//void timer(int value)
//{
//	std::ostringstream oss;
//	oss << CAPTION << ": " << FrameCount << " FPS @ (" << WinX << "x" << WinY << ")";
//	std::string s = oss.str();
//	glutSetWindow(WindowHandle);
//	glutSetWindowTitle(s.c_str());
//	FrameCount = 0;
//	glutTimerFunc(1000, timer, 0);
//}
//
//void refresh(int value)
//{
//	glutPostRedisplay();
//	glutTimerFunc(1000 / 60, refresh, 0);
//}
//
//// ------------------------------------------------------------
////
//// Reshape Callback Function
////
//
//void changeSize(int w, int h) {
//	// Prevent a divide by zero, when window is too short
//	if (h == 0)
//		h = 1;
//	// set the viewport to be the entire window
//	glViewport(0, 0, w, h);
//
//	// set the projection matrix
//	*pratio = (1.0f * w) / h;
//	loadIdentity(PROJECTION);
//	cam->updateProjection(pratio);
//
//
//}
//
//
//// ------------------------------------------------------------
////
//// Render stufff
////
//
//void checkMovements() {
//	if (!pause && !gameOver) {
//		if (move_forward) car->move(forward);
//		else if (move_back) car->move(back);
//		if (rot_left) car->rotate(left);
//		else if (rot_right) car->rotate(right);
//	}
//}
//
//
//void activateTextures() {
//	glActiveTexture(GL_TEXTURE1);
//	glBindTexture(GL_TEXTURE_2D, TextureArray[2]);
//
//	glActiveTexture(GL_TEXTURE2);
//	glBindTexture(GL_TEXTURE_2D, TextureArray[1]);
//
//	glUniform1i(tex_loc, 0+numberFonts);
//	glUniform1i(tex_loc1, 1+numberFonts);
//}
//
//void sendMatrices() {
//	computeDerivedMatrix(PROJ_VIEW_MODEL);
//	glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
//	glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
//	computeNormalMatrix3x3();
//	glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);
//}
//
////called when directional light is toggled on/off, sends the new value to the shader
//void sendDirectionalLightToggle()
//{
//	if (isDirLightOn)
//		glUniform1i(directionalLightOn_uniformId, 0);
//	else
//		glUniform1i(directionalLightOn_uniformId, 1);
//	isDirLightOn = !isDirLightOn;
//}
//
//void checkHeadlights() {
//	if (spotLightsOn) {
//		glUniform1i(spotOn_uniformId, 1);
//	}
//	else {
//		glUniform1i(spotOn_uniformId, 0);
//	}
//}
//void checkLifes();
//void checkCollisions()
//{
//	//check collision with table, if the car goes off the table, the player loses a life and respawns in the initial pos
//	if (!car->checkCollision(*car, *table) && numberLifes > 0) {
//		numberLifes--;
//		checkLifes();
//	}
//	//check if the car is colliding with any cheerio
//	for (int i = 0; i < trackLimit.size(); i++)
//	{
//		if (car->checkCollision(*car, trackLimit[i])) {
//			car->collided();
//			numCollisions++;
//		}
//	}
//}
//
//void drawLights() {
//	glUniform1i(directionalLightOn_uniformId, isDirLightOn); // update the switch in the shader (makes no sense)
//
//	float res[4];
//	glUniform1i(numLights_uniformId, (GLint)TOTAL_LIGHTS);
//	glUniform1i(numLights1_uniformId, (GLint)TOTAL_LIGHTS);
//	for (int i = 0; i < SPOT_LIGHTS; i++) {
//		multMatrixPoint(VIEW, new float[4]{ 0.f + i,.5f,0,1 }, res); //car->headlights[i]->l_position
//		glUniform4fv(glGetUniformLocation(shader.getProgramIndex(), ("lightsIn[" + std::to_string(i) + "].l_pos").c_str()), 1, res);
//		glUniform4fv(glGetUniformLocation(shader.getProgramIndex(), ("lightsIn[" + std::to_string(i) + "].l_spotDir").c_str()), 1, new float[4]{ 0.f,1,0,0 });
//		glUniform1f(glGetUniformLocation(shader.getProgramIndex(), ("lightsOUT[" + std::to_string(i) + "].l_cutoff").c_str()), 10);
//		glUniform1i(glGetUniformLocation(shader.getProgramIndex(), ("lightsOUT[" + std::to_string(i) + "].type").c_str()), 1);
//	}
//
//	multMatrixPoint(VIEW, directionalLightPos, res);   //lightPos definido em World Coord so is converted to eye space
//	glUniform4fv(glGetUniformLocation(shader.getProgramIndex(), ("lightsIn[" + std::to_string(SPOT_LIGHTS) + "].l_pos").c_str()), 1, res);
//	glUniform1i(glGetUniformLocation(shader.getProgramIndex(), ("lightsOUT[" + std::to_string(SPOT_LIGHTS) + "].type").c_str()), 2);
//	//std::cout << SPOT_LIGHTS << std::endl;
//	for (int j = 0; j < POINT_LIGHTS; j++) {
//		//if (j == 2) { candles[j]->l_position[0] += angle; }
//		multMatrixPoint(VIEW, candles[j]->l_position, res);   //lightPos definido em World Coord so is converted to eye space
//															  //std::cout << candles[j]->l_position[0] << ", " << candles[j]->l_position[1] << ", " << candles[j]->l_position[2] << std::endl;
//		glUniform4fv(glGetUniformLocation(shader.getProgramIndex(), ("lightsIn[" + std::to_string(j + SPOT_LIGHTS + DIRECTIONAL) + "].l_pos").c_str()), 1, res);
//		glUniform1i(glGetUniformLocation(shader.getProgramIndex(), ("lightsOUT[" + std::to_string(j + SPOT_LIGHTS + DIRECTIONAL) + "].type").c_str()), 0);
//		//std::cout << j + SPOT_LIGHTS + DIRECTIONAL << std::endl;
//	}
//
//}
//
//void drawFonts() {
//	std::string s = std::to_string(numberLifes);
//	char const *pchar = s.c_str();
//
//	std::string s1 = std::to_string(points);
//	char const *pchar1 = s1.c_str();
//
//	sprintf_s(l_string, "Lifes %s", pchar);
//	font1->DrawString(WinX / 2 - 60, WinY - 20, l_string, false);
//	sprintf_s(l_string, "Points %s", pchar1);
//	font1->DrawString(WinX / 2 + 60, WinY - 20, l_string, false);
//
//	if (!gameOver) {
//		sprintf_s(l_string, "Pause %s", "");
//		font1->DrawString(WinX / 2, WinY / 2, l_string, !pause);
//	}
//	else if (gameOver) {
//		sprintf_s(l_string, "Game Over ", "");
//		font1->DrawString(WinX / 2, WinY / 2, l_string, false);
//	}
//}
//
//void checkLifes() {
//	if (numberLifes <= 0) {
//		numberLifes = 0;
//		gameOver = true;
//	}
//	else {
//		car->respawn();
//	}
//}
//
//void restart() {
//	numberLifes = 5;
//	points = 0;
//	pause = false;
//	canPause = true;
//	spotLightsOn = true;
//	canChangeSpot = true;
//	isDirLightOn = true;
//	car->respawn();
//}
//
//void renderScene(void) {
//	FrameCount++;
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//
//	// load identity matrices
//	loadIdentity(VIEW);
//	loadIdentity(MODEL);
//	// set the camera using a function similar to gluLookAt
//	//lookAt(camX, camY, camZ, 0, 0, 0, 0, 1, 0);
//	if (cam->activeCam == cam->MOVINGPERSPECTIVE)
//		cam->sendCamCoords(camX, camY, camZ);
//	cam->updateLookAt(car->position);
//	// use our shader
//	shader.Use();
//
//	//glStencilFunc(GL_EQUAL, 0x1, 0x1);
//	//glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
//
//	//textures
//	checkMovements();
//	//checkHeadlights();
//
//	//checkCollisions();
//	drawLights();
//
//	for (int i = 0; i < numObjs; i++) {
//		//objects[i]->sendMaterial(shader.getProgramIndex());
//		pushMatrix(MODEL);
//		//scale(MODEL,objects[i]->scaleFactor, objects[i]->scaleFactor, objects[i]->scaleFactor);
//		translate(MODEL,car->position[0], car->position[1], car->position[2]);
//		rotate(MODEL, car->rotation, car->rotationAxis[0], car->rotationAxis[1], car->rotationAxis[2]);
//		sendMatrices();
//		car->drawObject(shader.getProgramIndex());
//	}
//
//	//draw car body	
//	//sendMaterial(car->objId);
//	//pushMatrix(MODEL);
//	//translate(MODEL,car->position[0],car->position[1],car->position[2]);
//	//rotate(MODEL,car->rotation,car->rotationAxis[0],car->rotationAxis[1],car->rotationAxis[2]);
//	//sendMatrices();
//	//drawObj(car->objId,0);
//
//	////draw car wheels
//	//for (int i = 0; i < 4; i++) {
//	//	Wheel*w = car->wheels[i];
//	//	pushMatrix(MODEL);
//	//	sendMaterial(w->objId);
//	//	translate(MODEL, w->position[0], w->position[1], w->position[2]);
//	//	rotate(MODEL,w->currentRot,w->movementRotAxis[0], w->movementRotAxis[1], w->movementRotAxis[2]);
//	//	rotate(MODEL, w->rotation,w->rotationAxis[0],w->rotationAxis[1],w->rotationAxis[2]);
//	//	sendMatrices();
//	//	drawObj(w->objId,0);
//	//	popMatrix(MODEL);
//
//	//}
//
//
//	//popMatrix(MODEL);
//	////draw trackLimiter
//	//for (int i = 0; i < trackLimit.size(); i++) {
//	//	pushMatrix(MODEL);
//	//	sendMaterial(trackLimit[i].objId);
//	//	translate(MODEL, trackLimit[i].position[0], trackLimit[i].position[1], trackLimit[i].position[2]);
//	//	rotate(MODEL, trackLimit[i].rotation, trackLimit[i].rotationAxis[0], trackLimit[i].rotationAxis[1], trackLimit[i].rotationAxis[2]);
//	//	sendMatrices();
//	//	drawObj(trackLimit[i].objId, 0);
//	//	popMatrix(MODEL);
//	//}
//
//	////draw table
//	////popMatrix(MODEL);	
//	//sendMaterial(table->objId);
//	////translate(MODEL, table->position[0], table->position[1], table->position[2]);
//	//rotate(MODEL,table->rotation,table->rotationAxis[0], table->rotationAxis[1], table->rotationAxis[2]);
//	//sendMatrices();
//	//drawObj(table->objId,2);
//	shader.UnUse();
//
//	drawFonts();
//
//	//glBindTexture(GL_TEXTURE_2D, 0);
//	glutSwapBuffers();
//}
//
//// ------------------------------------------------------------
////
//// Events from the Keyboard
////
//
//void processKeys(unsigned char key, int xx, int yy)
//{
//	//std::cout << key << std::endl;
//	switch (key) {
//
//	case 27:
//		glutLeaveMainLoop();
//		break;
//
//	case 'c':
//		printf("Camera Spherical Coordinates (%f, %f, %f)\n", alpha, beta, r);
//		break;
//	case 'm': glEnable(GL_MULTISAMPLE); break;
//		//case 'n': glDisable(GL_MULTISAMPLE); break;
//	case 'a':
//		move_forward = false;
//		move_back = true;
//		break;
//	case 'q':
//		move_forward = true;
//		move_back = false;
//		break;
//	case 'o':
//		rot_left = true;
//		rot_right = false;
//		break;
//	case 'p':
//		rot_left = false;
//		rot_right = true;
//		break;
//	case 'h':
//		if (canChangeSpot && !pause) {
//			spotLightsOn = !spotLightsOn;
//			canChangeSpot = false;
//		}
//		break;
//	case '1':
//		if (!pause) {
//			cam->setFixedOrtho();
//			cam->setCameraType(cam->FIXEDORTHO);
//		}
//		break;
//	case '2':
//		if (!pause) {
//			cam->setFixedPerspective(pratio);
//			cam->setCameraType(cam->FIXEDPERSPECTIVE);
//		}
//		break;
//	case '3':
//		if (!pause) {
//			cam->setMovingPerspective(pratio);
//			cam->setCameraType(cam->MOVINGPERSPECTIVE);
//		}
//		break;
//	case 'n':
//		if (!pause)		sendDirectionalLightToggle();
//		break;
//	case 's':
//		if (canPause) {
//			pause = !pause;
//			canPause = false;
//		}
//		break;
//	case 'r':
//		if (gameOver) {
//			gameOver = !gameOver;
//			restart();
//		}
//		break;
//	}
//
//}
//
//void processKeysUp(unsigned char key, int xx, int yy)
//{
//	switch (key) {
//
//	case 'a':
//		move_back = false;
//		break;
//	case 'q':
//		move_forward = false;
//		break;
//	case 'o':
//		rot_left = false;
//		break;
//	case 'p':
//		rot_right = false;
//		break;
//	case 'h':
//		canChangeSpot = true;
//		break;
//	case 's':
//		canPause = true;
//		break;
//	}
//
//}
//
//
//// ------------------------------------------------------------
////
//// Mouse Events
////
//
//void processMouseButtons(int button, int state, int xx, int yy)
//{
//	// start tracking the mouse
//	if (state == GLUT_DOWN) {
//		startX = xx;
//		startY = yy;
//		if (button == GLUT_LEFT_BUTTON)
//			tracking = 1;
//		else if (button == GLUT_RIGHT_BUTTON)
//			tracking = 2;
//	}
//
//	//stop tracking the mouse
//	else if (state == GLUT_UP) {
//		if (tracking == 1) {
//			alpha -= (xx - startX);
//			beta += (yy - startY);
//		}
//		else if (tracking == 2) {
//			r += (yy - startY) * 0.01f;
//			if (r < 0.1f)
//				r = 0.1f;
//		}
//		tracking = 0;
//	}
//}
//
//// Track mouse motion while buttons are pressed
//
//void processMouseMotion(int xx, int yy)
//{
//
//	int deltaX, deltaY;
//	float alphaAux, betaAux;
//	float rAux;
//
//	deltaX = -xx + startX;
//	deltaY = yy - startY;
//
//	// left mouse button: move camera
//	if (tracking == 1) {
//
//
//		alphaAux = alpha + deltaX;
//		betaAux = beta + deltaY;
//
//		if (betaAux > 85.0f)
//			betaAux = 85.0f;
//		else if (betaAux < -85.0f)
//			betaAux = -85.0f;
//		rAux = r;
//	}
//	// right mouse button: zoom
//	else if (tracking == 2) {
//
//		alphaAux = alpha;
//		betaAux = beta;
//		rAux = r + (deltaY * 0.01f);
//		if (rAux < 0.1f)
//			rAux = 0.1f;
//	}
//
//	camX = rAux * sin(alphaAux * 3.14f / 180.0f) * cos(betaAux * 3.14f / 180.0f);
//	camZ = rAux * cos(alphaAux * 3.14f / 180.0f) * cos(betaAux * 3.14f / 180.0f);
//	camY = rAux *   						       sin(betaAux * 3.14f / 180.0f);
//
//	//  uncomment this if not using an idle func
//	//	glutPostRedisplay();
//}
//
//
//void mouseWheel(int wheel, int direction, int x, int y) {
//
//	r += direction * 0.1f;
//	if (r < 0.1f)
//		r = 0.1f;
//
//	camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
//	camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
//	camY = r *   						     sin(beta * 3.14f / 180.0f);
//
//	//  uncomment this if not using an idle func
//	//	glutPostRedisplay();
//}
//
//// --------------------------------------------------------
////
//// Shader Stuff
////
//
//
//GLuint setupShaders() {
//
//	// Shader for models
//	shader.init();
//	shader.loadShader(VSShaderLib::VERTEX_SHADER, "..\\shaders\\lights.vert");
//	shader.loadShader(VSShaderLib::FRAGMENT_SHADER, "..\\shaders\\lights.frag");
//
//	// set semantics for the shader variables
//	glBindFragDataLocation(shader.getProgramIndex(), 0, "colorOut");
//	glBindAttribLocation(shader.getProgramIndex(), VERTEX_COORD_ATTRIB, "position");
//	glBindAttribLocation(shader.getProgramIndex(), NORMAL_ATTRIB, "normal");
//	glBindAttribLocation(shader.getProgramIndex(), TEXTURE_COORD_ATTRIB, "texCoord");
//
//	glLinkProgram(shader.getProgramIndex());
//
//	pvm_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_pvm");
//	vm_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_viewModel");
//	normal_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_normal");
//	//lights
//	numLights_uniformId = glGetUniformLocation(shader.getProgramIndex(), "numLights");
//	numLights1_uniformId = glGetUniformLocation(shader.getProgramIndex(), "numLights1");
//	spotOn_uniformId = glGetUniformLocation(shader.getProgramIndex(), "spotOn");
//	lPos_uniformId = glGetUniformLocation(shader.getProgramIndex(), "l_pos");
//	directionalLightOn_uniformId = glGetUniformLocation(shader.getProgramIndex(), "directionalLightOn");
//	glUniform1i(directionalLightOn_uniformId, 1);
//
//	tex_loc = glGetUniformLocation(shader.getProgramIndex(), "texmap");
//	tex_loc1 = glGetUniformLocation(shader.getProgramIndex(), "texmap1");
//	texMode_uniformId = glGetUniformLocation(shader.getProgramIndex(), "texMode"); // different modes of texturing
//
//	printf("InfoLog for Hello World Shader\n%s\n\n", shader.getAllInfoLogs().c_str());
//
//	return(shader.isProgramLinked());
//}
//
//void createLights() {
//
//	for (int i = 0; i < POINT_LIGHTS; i++) {
//		candles[i] = new LightSource();
//		float pos[4] = { rand() % 50, .5f, rand() % 50, 1.0f };
//		candles[i]->setPoint(pos);
//	}
//
//}
//
////void createObjects() {
////
////	float pos[3] = { .5f, 0.7f, -.5f };
////	float rot[3] = { 0,1,0 };
////
////	for (int i = 0; i < numObjs; i++) {
////		objects[i] = new ObjectFromLoad();
////		if (!objects[i]->Import3DFromFile(modelname,pos, 0,rot ))
////			std::cout << "FAIL LOAD OBJ" << std::endl;
////		else {
////			//objects[i]->LoadGLTextures();
////			objects[i]->genVAOsAndUniformBuffer();
////		}
////	}
////}
//
//void createTextures() {
//	glGenTextures(3, TextureArray);
//	TGA_Texture(TextureArray, "..//stone.tga", 0);
//	TGA_Texture(TextureArray, "..//course1.tga", 1);
//	TGA_Texture(TextureArray, "..//lightwood.tga", 2);
//}
//
//void createCar() {
//	car = new Car();
//	car->init(carFile,2, new float[3]{ .5f, 0.7f, -.5f }, new float[3]{ 0,1,0 }, 0);
//}
//
//// ------------------------------------------------------------
////
//// Model loading and OpenGL setup
////
//// Replace the model name by your model's filename
//void init()
//{
//	// set the camera position based on its spherical coordinates
//	camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
//	camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
//	camY = r *   						     sin(beta * 3.14f / 180.0f);
//
//
//	//createObjects();
//	createTextures();
//	createCar();
//	//createTable();
//	//createTrack();
//	createLights(); 
//
//
//	// some GL settings
//	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_CULL_FACE);
//	glEnable(GL_MULTISAMPLE);
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
//
//	/**TEXTUREMAPPEDFONT*/
//	font1 = new TextureMappedFont("..//font1.bmp");
//	//glClearStencil(0x0);
//	//glEnable(GL_STENCIL_TEST);
//
//
//
//
//}
//
//// ------------------------------------------------------------
////
//// Main function
////
//
//
//int main(int argc, char **argv) {
//
//	//  GLUT initialization
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GL_STENCIL | GL_BLEND | GLUT_RGBA | GLUT_MULTISAMPLE);
//
//	glutInitContextVersion(3, 3);
//	glutInitContextProfile(GLUT_CORE_PROFILE);
//	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);
//
//	glutInitWindowPosition(100, 100);
//	glutInitWindowSize(WinX, WinY);
//	WindowHandle = glutCreateWindow(CAPTION);
//
//
//	//  Callback Registration
//	glutDisplayFunc(renderScene);
//	glutReshapeFunc(changeSize);
//	//glutIdleFunc(renderScene);
//
//	//	Mouse and Keyboard Callbacks
//	glutKeyboardFunc(processKeys);
//	glutKeyboardUpFunc(processKeysUp);
//	glutMouseFunc(processMouseButtons);
//	glutMotionFunc(processMouseMotion);
//	glutMouseWheelFunc(mouseWheel);
//	glutTimerFunc(0, timer, 0);
//	glutTimerFunc(0, refresh, 0);
//	glutTimerFunc(0, Timer, 0);
//
//	//	return from main loop
//	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
//
//	//	Init GLEW
//	glewExperimental = GL_TRUE;
//	glewInit();
//
//	printf("Vendor: %s\n", glGetString(GL_VENDOR));
//	printf("Renderer: %s\n", glGetString(GL_RENDERER));
//	printf("Version: %s\n", glGetString(GL_VERSION));
//	printf("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
//
//	if (!setupShaders())
//		return(1);
//
//	init();
//
//	//  GLUT main loop
//	glutMainLoop();
//	for (int i = 0; i < numObjs; i++) {
//		objects[i]->clear();
//	}
//	
//
//	return(0);
//
//}
//
