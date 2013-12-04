
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
/* map of tangram objects */
std::map<std::string, WorldObject *> * tangram;

bool selected = false;

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
	myShader->addUniform("LightDirection");
	myShader->addUniformBlock("SharedMatrices", BINDPOINT, sharedMatricesBufferObject);
	myShader->createCompileLink();
	shaderManager->add("SimpleShader", myShader);

	myShader->bind();
	glm::vec3 lightDir = glm::normalize(glm::vec3(-1.0f, 0.0f, -1.0f));
	myShader->sendUniformVec3("LightDirection", lightDir);
	checkOpenGLError("Problem passing LightDirection.");
	myShader->unbind();
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

void timeUpdate() {
	currentTime = glutGet(GLUT_ELAPSED_TIME) * 1E-03;
	time += (currentTime - oldTime);
	oldTime = currentTime;
}

void drawScene() {
	timeUpdate();

	ShaderProgram* shader = ShaderManager::getInstance()->get("SimpleShader");
	shader->bind();

	glm::mat4x4 view = orbit();
	glm::mat4x4 proj = glm::ortho(-3.f, 3.f, -3.f, 3.f, 0.f, 10.f);

	writeSharedMatrices(view, proj);

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
	float dy = newMy - my;

	//move camera
	if (!selected){	
		theta += 2 * PI * (-dx / WinX);
		phi += 2 * PI * (dy / WinY);
		//std::cout << "   theta:	   " << theta << "	   phi:	    " << phi << std::endl;
	}
	//move the selected object
	else{
		
	}

	mx = newMx;
	my = newMy;
}

void mousePressed(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		mx = x;
		my = y;
	}
}

/////////////////////////////////////////////////////////////////////// SCENE OBJECT MANIPULATION

void keyboardKey(unsigned char key, int x, int y) {

	if (key == 's'){
		selected = !selected;
	}


	
}

void SpecialkeyboardKey(int key, int x, int y){
	if (selected){
		if (key == GLUT_KEY_LEFT){

		}
		if (key == GLUT_KEY_RIGHT){

		}
	}

}

/////////////////////////////////////////////////////////////////////// SETUP
void buildTangram() {
	std::map<std::string, WorldObject*> tangramObject = *tangram;
	WorldObject* aux;
	/* Square */
	aux = tangramObject["Square"];
	aux->setColor();
}


void loadModels() {
	ModelLoader modelLoader;
	
	tangram = new std::map<std::string, WorldObject*>();

	WorldObject * aux;
	std::string name;

	RenderModelManager* renderManager = RenderModelManager::instance();
	
	renderManager->addRenderModel("Square",modelLoader.loadModel("../resources/Square.obj"));
	aux = new WorldObject(renderManager->getRenderModel("Square"));
	world->add(aux);
	tangram->operator[]("Square") = aux;

	renderManager->addRenderModel("MedTri", modelLoader.loadModel("../resources/MedTri.obj"));
	aux = new WorldObject(renderManager->getRenderModel("MedTri"));
	world->add(aux);
	tangram->operator[]("MedTri") = aux;

	renderManager->addRenderModel("BigTri1", modelLoader.loadModel("../resources/BigTri.obj"));
	aux = new WorldObject(renderManager->getRenderModel("BigTri1"));
	world->add(aux);
	tangram->operator[]("BigTri1") = aux;

	renderManager->addRenderModel("BigTri2", modelLoader.loadModel("../resources/BigTri.obj"));
	aux = new WorldObject(renderManager->getRenderModel("BigTri2"));
	world->add(aux);
	tangram->operator[]("BigTri2") = aux;

	renderManager->addRenderModel("SmallTri1", modelLoader.loadModel("../resources/SmallTri.obj"));
	aux = new WorldObject(renderManager->getRenderModel("SmallTri1"));
	world->add(aux);
	tangram->operator[]("SmallTri1") = aux;

	renderManager->addRenderModel("SmallTri2", modelLoader.loadModel("../resources/SmallTri.obj"));
	aux = new WorldObject(renderManager->getRenderModel("SmallTri2"));
	world->add(aux);
	tangram->operator[]("SmallTri2") = aux;

	renderManager->addRenderModel("Quad", modelLoader.loadModel("../resources/Quad.obj"));
	aux = new WorldObject(renderManager->getRenderModel("Quad"));
	world->add(aux);
	tangram->operator[]("Quad") = aux;

	renderManager->addRenderModel("BackPlane", modelLoader.loadModel("../resources/BackPlane.obj"));
	aux = new WorldObject(renderManager->getRenderModel("BackPlane"));
	world->add(aux);
	tangram->operator[]("BackPlane") = aux;
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
	glutKeyboardFunc(keyboardKey);
	glutSpecialFunc(SpecialkeyboardKey);
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
	buildTangram();
	cameraSetup();
	initTime();
}

int main(int argc, char* argv[])
{
	init(argc, argv);
	glutMainLoop();
	exit(EXIT_SUCCESS);
}

///////////////////////////////////////////////////////////////////////
