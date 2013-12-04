///////////////////////////////////////////////////////////////////////
//
// Assignment 3 consists in the following:
//
// - Create the following changes to your scene, making it fully 3D:
//   - Extrude your TANs into the 3rd dimension. The TANs should have
//     slightly different "heights".
//   - The new faces of each TAN should share the same hue as the 
//     original top face color but have different levels of saturation 
//     and brightness (use an external app if needed).
//   - The shape is now built vertically (i.e. rather than horizontally
//     as in assignment 2) but still on top of the surface.
//   - When the TANs join to create your chosen shape, they should not 
//     be perfectly aligned in the new dimension.
//
// - Add the following functionality:
//   - Create a View Matrix from (eye, center, up) parameters.
//   - Create an Orthographic Projection Matrix from (left-right, 
//     bottom-top, near-far) parameters.
//   - Create a Perspective Projection Matrix from (fovy, aspect,
//     nearZ, farZ) parameters.
//   - Implement rotations through quaternions.
//
// - Add the following dynamics to the application:
//   - Create a spherical 3D camera controlled by the mouse allowing to 
//     visualize the scene through all its angles.
//   - Change perspective from orthographic to perspective and back as
//     a response to pressing the key 'p'.
//   - The scene starts with the 7 TANs in their original square 
//     configuration, laying flat and horizontally on the surface.
//   - Each time the 't' key is pressed, one of the TANs will move from 
//     its resting place to its position in your chosen shape, now 
//     presented vertically.
//
// Further suggestions:
//   - Use mouse interaction to select which TAN will be next moved 
//     into place or moved back to its resting position.
//   - Create an edit mode in which the user can edit the shape in
//     real-time (e.g. position, rotation, color).
//   - Allow to load and save the TANGRAM shapes from external files.
//
// (c) 2013 by Carlos Martinho
//
///////////////////////////////////////////////////////////////////////

#include <iostream>
#include <sstream>
#include <string>

#include "engine.h"

#include "WorldObjectManager.h"

#define CAPTION "Hello New World"
#define PI 3.14159265359

int WinX = 640, WinY = 480;
int WindowHandle = 0;
unsigned int FrameCount = 0;

/*
mouse coordinates
*/
int mx, my, newMx, newMy;
/*
spherical coordinates of the camera
*/
float theta, phi, raw;
glm::vec3 cameraCenter;
/* shared matrices bindpoint*/
const GLuint BINDPOINT = 0;
/* shared matrices buffer object id*/
GLuint sharedMatricesBufferObject = 0;
/* time counter vars */
float oldTime;
float currentTime;
float time;

WorldObjectManager *world = new WorldObjectManager();

/////////////////////////////////////////////////////////////////////// SHADERs
void createSharedUniformBlocks() {
	glGenBuffers(1, &sharedMatricesBufferObject);
	glBindBuffer(GL_UNIFORM_BUFFER, sharedMatricesBufferObject);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(GLfloat)* 2 * 16, 0, GL_STREAM_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, BINDPOINT, sharedMatricesBufferObject);
}

void createShaderProgram() {
	createSharedUniformBlocks();
	ShaderManager* shaderManager = ShaderManager::getInstance();
	ShaderProgram * myShader = new ShaderProgram();
	myShader->addShader(GL_VERTEX_SHADER, "../shaderSrc/myVS.glsl");
	myShader->addShader(GL_FRAGMENT_SHADER, "../shaderSrc/myFS.glsl");
	myShader->addAttrib("inPosition",RenderModel::POSITION);
	myShader->addAttrib("inNormal", RenderModel::NORMAL);
	myShader->addAttrib("inTex", RenderModel::TEX);
	myShader->addUniform("Color");
	myShader->addUniform("ModelMatrix");
	myShader->addUniformBlock("SharedMatrices", BINDPOINT, sharedMatricesBufferObject);
	myShader->createCompileLink();
	shaderManager->add("SimpleShader", myShader);
}

void destroyShaderProgram() {
	
}

/////////////////////////////////////////////////////////////////////// VAOs & VBOs

void createBufferObjects()
{
	
}

void destroyBufferObjects()
{
 
}

/////////////////////////////////////////////////////////////////////// SCENE

void writeSharedMatrices(glm::mat4 view, glm::mat4 projection) {
	glBufferSubData(GL_UNIFORM_BUFFER, 0, 16 * sizeof(GLfloat), glm::value_ptr(view));
	glBufferSubData(GL_UNIFORM_BUFFER, 16 * sizeof(GLfloat), 16 * sizeof(GLfloat), glm::value_ptr(projection));
}

glm::mat4x4 orbit() {
	float cosP = cos(phi);
	float cosT = cos(theta);
	float sinP = sin(phi);
	float sinT = sin(theta);
	glm::vec3 eye(raw * cosP * cosT + cameraCenter.x, raw * cosP * sinT + cameraCenter.y, raw * sinP + cameraCenter.z);
	glm::vec3 up(-sinP * cosT, -sinP * sinT, cosP);
	return glm::lookAt(eye, cameraCenter, up);
}

void drawScene() {
	ShaderProgram* shader = ShaderManager::getInstance()->get("SimpleShader");
	shader->bind();

	glm::mat4x4 view = orbit();
	glm::mat4x4 proj = glm::ortho(-3.f, 3.f, -3.f, 3.f, 0.f, 10.f);
	writeSharedMatrices(view, proj);
	
	glm::mat4x4 model = glm::mat4(1.0);
	shader->sendUniformMat4("ModelMatrix",model);
	
	glm::vec3 color = glm::vec3(.5f);
	shader->sendUniformVec3("Color", color);

	/*RenderModel* rendermodel = RenderModelManager::instance()->getRenderModel("BigTri1");
	rendermodel->drawModel();*/
	world->draw(shader);
	shader->unbind();
}

/////////////////////////////////////////////////////////////////////// CALLBACKS

void cleanup()
{
	destroyShaderProgram();
	destroyBufferObjects();
}

void display()
{
	++FrameCount;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawScene();
	glutSwapBuffers();
}

void idle()
{
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	WinX = w;
	WinY = h;
	glViewport(0, 0, WinX, WinY);
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

void mouseMotion(int x, int y)  {
	newMx = x;
	newMy = y;
	float dx = newMx - mx;
	float dy =  newMy - my;
	theta += 2 * PI * (-dx / WinX);
	phi += 2 * PI * (dy / WinY);
	std::cout << "   theta:	   " << theta << "	   phi:	    " << phi << std::endl;
	
	mx = newMx;
	my = newMy;
}

void mousePressed(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		mx = x;
		my = y;
	}
}

/////////////////////////////////////////////////////////////////////// SETUP

void loadModels() {
	ModelLoader modelLoader;

	RenderModelManager* renderManager = RenderModelManager::instance();
	renderManager->addRenderModel("Square",modelLoader.loadModel("../resources/Square.obj"));
	world->add(new WorldObject(renderManager->getRenderModel("Square")));

	renderManager->addRenderModel("MedTri", modelLoader.loadModel("../resources/MedTri.obj"));
	world->add(new WorldObject(renderManager->getRenderModel("MedTri")));

	renderManager->addRenderModel("BigTri1", modelLoader.loadModel("../resources/BigTri.obj"));
	world->add(new WorldObject(renderManager->getRenderModel("BigTri1")));

	renderManager->addRenderModel("BigTri2", modelLoader.loadModel("../resources/BigTri.obj"));
	world->add(new WorldObject(renderManager->getRenderModel("BigTri2")));

	renderManager->addRenderModel("SmallTri1", modelLoader.loadModel("../resources/SmallTri.obj"));
	world->add(new WorldObject(renderManager->getRenderModel("SmallTri1")));

	renderManager->addRenderModel("SmallTri2", modelLoader.loadModel("../resources/SmallTri.obj"));
	world->add(new WorldObject(renderManager->getRenderModel("SmallTri2")));

	renderManager->addRenderModel("Quad", modelLoader.loadModel("../resources/Quad.obj"));
	world->add(new WorldObject(renderManager->getRenderModel("Quad")));
}


void setupCallbacks()
{
	glutCloseFunc(cleanup);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutTimerFunc(0, timer, 0);
	glutMouseFunc(mousePressed);
	glutMotionFunc(mouseMotion);
}

void setupOpenGL() {
	std::cerr << "CONTEXT: OpenGL v" << glGetString(GL_VERSION) << std::endl;
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glDepthRange(0.0, 1.0);
	glClearDepth(1.0);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
}

void setupGLEW() {
	glewExperimental = GL_TRUE;
	GLenum result = glewInit();
	if (result != GLEW_OK) {
		std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
		exit(EXIT_FAILURE);
	}
	GLenum err_code = glGetError();
}

void setupGLUT(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutInitWindowSize(WinX, WinY);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	WindowHandle = glutCreateWindow(CAPTION);
	if (WindowHandle < 1) {
		std::cerr << "ERROR: Could not create a new rendering window." << std::endl;
		exit(EXIT_FAILURE);
	}
}

void cameraSetup() 
{
	theta = 0.0f;
	phi = 0.0f;
	raw = 3.0f;
	cameraCenter = glm::vec3(0.0f);
}

void initTime() 
{
	oldTime = glutGet(GLUT_ELAPSED_TIME) * 1E-03;
	time = 0.0f;
}

void init(int argc, char* argv[])
{
	setupGLUT(argc, argv);
	setupGLEW();
	setupOpenGL();
	createShaderProgram();
	createBufferObjects();
	setupCallbacks();
	loadModels();
	cameraSetup();
}

int main(int argc, char* argv[])
{
	init(argc, argv);
	glutMainLoop();
	exit(EXIT_SUCCESS);
}

///////////////////////////////////////////////////////////////////////
