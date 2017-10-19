/////////////////////////////////////////////////////////////////////////
////
//// 
//// (c) 2014 by João Madeiras Pereira
////
/////////////////////////////////////////////////////////////////////////
//
//#include <iostream>
//#include <sstream>
//#include <string>
//#include <fstream>
//
//#include <GL/glew.h>
//#include <GL/freeglut.h>
//
//#include "cube.h"
//#include "AVTmathLib.h"
//
//#define CAPTION "Exercise 1"
//#define PATH "..\\files\\"
//
//using namespace std;
//int WinX = 640, WinY = 480;
//int WindowHandle = 0;
//unsigned int FrameCount = 0;
//
//#define VERTEX_COORD_ATTRIB 0
//#define NORMAL_ATTRIB 1
//#define TEXTURE_COORD_ATTRIB 2
//
//GLuint VaoId, VboId[2];
//GLuint VertexShaderId, FragmentShaderId, ProgramId;
//GLint UniformId;
//
//struct MyMesh mesh[4];
//int objId;
//
//extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];
//// Camera Position
//float camX, camY, camZ;
//
//// Mouse Tracking Variables
//int startX, startY, tracking = 0;
//
//// Camera Spherical Coordinates
//float alpha = 39.0f, beta = 51.0f; //a-39 b-51
//float r = 10.0f;
//
//// ------------------------------------------------------------
////
//// Mouse Events
////
//
//void processMouseButtons(int button, int state, int xx, int yy)
//{
//	// start tracking the mouse
//	if (state == GLUT_DOWN)  {
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
//	deltaX =  - xx + startX;
//	deltaY =    yy - startY;
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
////  uncomment this if not using an idle func
////	glutPostRedisplay();
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
////  uncomment this if not using an idle func
////	glutPostRedisplay();
//}
//
//// --------------------------------------------------------
////
//
///////////////////////////////////////////////////////////////////////// ERRORS
//
//bool isOpenGLError() {
//	bool isError = false;
//	GLenum errCode;
//	const GLubyte *errString;
//	while ((errCode = glGetError()) != GL_NO_ERROR) {
//		isError = true;
//		errString = gluErrorString(errCode);
//		std::cerr << "OpenGL ERROR [" << errString << "]." << std::endl;
//	}
//	return isError;
//}
//
//void checkOpenGLError(std::string error)
//{
//	if(isOpenGLError()) {
//		std::cerr << error << std::endl;
//		//exit(EXIT_FAILURE);
//	}
//}
//
///////////////////////////////////////////////////////////////////////// SHADERs
//const GLchar* VertexShader;
//const GLchar* FragmentShader;
//
//string readFile(std::string fileName) {
//	string line;
//	string file;
//	ifstream myfile(PATH + fileName + ".txt");
//
//	if (myfile.is_open())
//	{
//		while (getline(myfile, line))
//		{
//			file.append(line + '\n');
//		}
//		myfile.close();
//	}
//
//	else cout << "Unable to open file";
//	return file;
//}
//
//GLuint addShader(string fileName, const GLenum shader_type) {
//	string text = readFile(fileName);
//	const GLchar* code = text.c_str();
//	GLuint id = glCreateShader(shader_type);
//	glShaderSource(id, 1, &code, 0);
//	glCompileShader(id);
//
//
//	/**CHECK COMPILATION ERRORS*/
//	GLint success;
//	GLchar infoLog[512];
//	glGetShaderiv(id,GL_COMPILE_STATUS, &success);
//
//	if (!success) {
//		glGetShaderInfoLog(id,512,NULL,infoLog);
//		cout << "ERROR::SHADER:: " << fileName << " -> "<< infoLog << endl;
//		return -1;
//	}
//
//	return id;
//}
//
//void createShaderProgram()
//{
//	VertexShaderId = addShader("vertex", GL_VERTEX_SHADER);
//	FragmentShaderId = addShader("fragment",GL_FRAGMENT_SHADER);
//
//	ProgramId = glCreateProgram();
//	glAttachShader(ProgramId, VertexShaderId);
//	glAttachShader(ProgramId, FragmentShaderId);
//
//	glBindAttribLocation(ProgramId, VERTEX_COORD_ATTRIB, "in_Position");
//	
//	glLinkProgram(ProgramId);
//
//	/**CHECK LINKAGE ERRORS*/
//	GLint success;
//	GLchar infoLog[512];
//	glGetProgramiv(ProgramId,GL_LINK_STATUS, &success);
//	if (!success) {
//		glGetProgramInfoLog(ProgramId,512,NULL,infoLog);
//		cout << "ERROR::SHADER::LINKAGE-> " << infoLog << endl;
//	}
//	else {
//		UniformId = glGetUniformLocation(ProgramId, "Matrix");
//	}
//	checkOpenGLError("ERROR: Could not create shaders.");
//}
//
//void destroyShaderProgram()
//{
//	glUseProgram(0);
//	glDetachShader(ProgramId, VertexShaderId);
//	glDetachShader(ProgramId, FragmentShaderId);
//
//	glDeleteShader(FragmentShaderId);
//	glDeleteShader(VertexShaderId);
//	glDeleteProgram(ProgramId);
//
//	checkOpenGLError("ERROR: Could not destroy shaders.");
//}
//
//
//
//
///////////////////////////////////////////////////////////////////////// VAOs & VBOs
//
//
//void createBufferObjects()
//{
//	glGenVertexArrays(1, &VaoId);
//	glBindVertexArray(VaoId);
//
//	glGenBuffers(2,VboId);
//	glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)+sizeof(normals)+sizeof(texCoords),NULL, GL_STATIC_DRAW);
//		glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(vertices),vertices);
//		glBufferSubData(GL_ARRAY_BUFFER,sizeof(vertices), sizeof(normals), normals);
//		glBufferSubData(GL_ARRAY_BUFFER,sizeof(vertices)+sizeof(normals),sizeof(texCoords), texCoords);
//
//	glEnableVertexAttribArray(VERTEX_COORD_ATTRIB);
//	glVertexAttribPointer(VERTEX_COORD_ATTRIB,4,GL_FLOAT,0,0,0);	
//	glEnableVertexAttribArray(NORMAL_ATTRIB);
//	glVertexAttribPointer(NORMAL_ATTRIB, 3, GL_FLOAT, 0, 0, (void*)sizeof(vertices));
//	glEnableVertexAttribArray(TEXTURE_COORD_ATTRIB);
//	glVertexAttribPointer(TEXTURE_COORD_ATTRIB, 2, GL_FLOAT, 0, 0, (void*)(sizeof(vertices)+sizeof(normals)));
//
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboId[1]);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faceIndex), faceIndex, GL_STATIC_DRAW);
//
//// unbind the VAO
//	glBindVertexArray(0);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//	/*glDisableVertexAttribArray(VERTICES);
//	glDisableVertexAttribArray(COLORS);*/
//
//	checkOpenGLError("ERROR: Could not create VAOs and VBOs.");
//}
//
//void destroyBufferObjects()
//{
//	glDisableVertexAttribArray(VERTEX_COORD_ATTRIB);
//	glDisableVertexAttribArray(NORMAL_ATTRIB);
//	glDisableVertexAttribArray(TEXTURE_COORD_ATTRIB);
//
//
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//	glBindVertexArray(0);
//
//	glDeleteBuffers(2, VboId);
//	glDeleteVertexArrays(1, &VaoId);
//	checkOpenGLError("ERROR: Could not destroy VAOs and VBOs.");
//}
//
///////////////////////////////////////////////////////////////////////// SCENE
//
//typedef GLfloat Matrix[16];
//
//const Matrix I = {
//	1.0f,  0.0f,  0.0f,  0.0f,
//	0.0f,  1.0f,  0.0f,  0.0f,
//	0.0f,  0.0f,  1.0f,  0.0f,
//	0.0f,  0.0f,  0.0f,  1.0f
//}; // Row Major (GLSL is Column Major)
//
//const Matrix M = {
//	1.0f,  0.0f,  0.0f, -1.0f,
//	0.0f,  1.0f,  0.0f, -1.0f,
//	0.0f,  0.0f,  1.0f,  0.0f,
//	0.0f,  0.0f,  0.0f,  1.0f
//}; // Row Major (GLSL is Column Major)
//
//
//void renderScene()
//{
//	glBindVertexArray(VaoId);
//	glUseProgram(ProgramId);
//
//	//glUniformMatrix4fv(UniformId, 1, GL_TRUE, I);
//	//glDrawElements(GL_TRIANGLES, faceCount * 3, GL_UNSIGNED_INT, (GLvoid*)0);
//
//	//glUniformMatrix4fv(UniformId, 1, GL_TRUE, M);
//	//glDrawElements(GL_TRIANGLES, faceCount * 3, GL_UNSIGNED_INT, (GLvoid*)0);
//
//	loadIdentity(VIEW);
//	loadIdentity(MODEL);
//	lookAt(camX, camY, camZ, 0, 0, 0, 0, 1, 0);
//
//
//	computeDerivedMatrix(PROJ_VIEW_MODEL);
//	glUniformMatrix4fv(UniformId, 1, GL_TRUE, mCompMatrix[VIEW_MODEL]) ;
//	glDrawElements(GL_TRIANGLES, faceCount*3, GL_UNSIGNED_INT, (GLvoid*)0);
//
//	glUniformMatrix4fv(UniformId, 1, GL_TRUE, mCompMatrix[PROJ_VIEW_MODEL]);
//	glDrawElements(GL_TRIANGLES, faceCount*3, GL_UNSIGNED_INT, (GLvoid*)0);
//
//	glUseProgram(0);
//	glBindVertexArray(0);
//
//	checkOpenGLError("ERROR: Could not draw scene.");
//}
//
///////////////////////////////////////////////////////////////////////// CALLBACKS
//
//void cleanup()
//{
//	destroyShaderProgram();
//	destroyBufferObjects();
//}
//
//void display()
//{
//	++FrameCount;
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	renderScene();
//	glutSwapBuffers();
//}
//
//void idle()
//{
//	glutPostRedisplay();
//}
//
//void refresh(int value) {
//	glutPostRedisplay();
//	glutTimerFunc(1000/60,refresh,0);
//}
//
//void reshape(int w, int h)
//{
//	WinX = w;
//	WinY = h;
//	//glViewport(0, 0, w, h);
//	if (WinX>WinY)	
//	{	
//		glViewport((WinX - WinY) / 2, 0, WinY, WinY);	
//	}	
//	else
//		glViewport(0, (WinY - WinX) / 2, WinX, WinX);
//
//	float ratio = (1.0f * w) / h;
//	loadIdentity(PROJECTION);
//	perspective(53.13, ratio, 0.01f, 1000.0f);
//	//glMatrixMode(GL_PROJECTION); // Switch to the projection matrix so that we can manipulate how our scene is viewed  
//	//glLoadIdentity(); // Reset the projection matrix to the identity matrix so that we don't get any artifacts (cleaning up)  
//	//gluPerspective(60, (GLfloat)WinX / (GLfloat)WinY, 1.0, 100.0); // Set the Field of view angle (in degrees), the aspect ratio of our window, and the new and far planes  
//	//glMatrixMode(GL_MODELVIEW);
//	//glViewport(0, 0, WinX, WinY);
//}
//
//void timer(int value)
//{
//	std::ostringstream oss;
//	oss << CAPTION << ": " << FrameCount << " FPS @ (" << WinX << "x" << WinY << ")";
//	std::string s = oss.str();
//	glutSetWindow(WindowHandle);
//	glutSetWindowTitle(s.c_str());
//    FrameCount = 0;
//    glutTimerFunc(1000, timer, 0);
//}
//
///////////////////////////////////////////////////////////////////////// SETUP
//
//void setupCallbacks() 
//{
//	glutCloseFunc(cleanup);
//	glutDisplayFunc(display);
//	//glutIdleFunc(idle);
//	glutReshapeFunc(reshape);
//	glutTimerFunc(0,timer,0);
//	glutTimerFunc(0,refresh,0);
//}
//
//void setupOpenGL() {
//	std::cerr << "CONTEXT: OpenGL v" << glGetString(GL_VERSION) << std::endl;
//	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
//	glEnable(GL_DEPTH_TEST);
//	glDepthFunc(GL_LEQUAL);
//	glDepthMask(GL_TRUE);
//	glDepthRange(0.0, 1.0);
//	glClearDepth(1.0);
//	glEnable(GL_CULL_FACE);
//	glCullFace(GL_BACK);
//	glFrontFace(GL_CCW);
//}
//
//void setupGLEW() {
//	glewExperimental = GL_TRUE;
//	GLenum result = glewInit() ; 
//	if (result != GLEW_OK) { 
//		std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
//		exit(EXIT_FAILURE);
//	} 
//	GLenum err_code = glGetError();
//	printf ("Vendor: %s\n", glGetString (GL_VENDOR));
//	printf ("Renderer: %s\n", glGetString (GL_RENDERER));
//	printf ("Version: %s\n", glGetString (GL_VERSION));
//	printf ("GLSL: %s\n", glGetString (GL_SHADING_LANGUAGE_VERSION));
//
//}
//
//void setupGLUT(int argc, char* argv[])
//{
//	glutInit(&argc, argv);
//	
//	glutInitContextVersion(3, 3);
//	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
//	glutInitContextProfile(GLUT_CORE_PROFILE);
//
//	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_GLUTMAINLOOP_RETURNS);
//	
//	glutInitWindowSize(WinX, WinY);
//	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
//	WindowHandle = glutCreateWindow(CAPTION);
//	if(WindowHandle < 1) {
//		std::cerr << "ERROR: Could not create a new rendering window." << std::endl;
//		exit(EXIT_FAILURE);
//	}
//}
//
//void init(int argc, char* argv[])
//{
//
//	camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
//	camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
//	camY = r *   						     sin(beta * 3.14f / 180.0f);
//	setupGLUT(argc, argv);
//	setupGLEW();
//	setupOpenGL();
//	createShaderProgram();
//	createBufferObjects();
//
//	//	Mouse and Keyboard Callbacks
//	//glutKeyboardFunc(processKeys);
//	glutMouseFunc(processMouseButtons);
//	glutMotionFunc(processMouseMotion);
//	glutMouseWheelFunc(mouseWheel);
//
//	setupCallbacks();
//}
//
//int main(int argc, char* argv[])
//{
//	init(argc, argv);
//	glutMainLoop();	
//	exit(EXIT_SUCCESS);
//}
//
/////////////////////////////////////////////////////////////////////////
////glutPosDisplay