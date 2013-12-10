
#include <iostream>
#include <sstream>
#include <string>

#include "engine.h"

#include "WorldObjectManager.h"

#define CAPTION "Hello New World"
#define PI 3.14159265359

const std::string ResourcesPath = "../resources/";
const std::string ShaderDir = "shaderSrc/";
const std::string ModelDir = "models/";
const std::string MaterialDir = "materials/";
const std::string ShaderPath = ResourcesPath + ShaderDir;
const std::string ModelPath = ResourcesPath + ModelDir;
const std::string MaterialPath = ResourcesPath + MaterialDir;

const std::string MaterialListFile = "materials.txt";


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
glm::mat4x4 proj;
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
bool rotateState = false;
bool moveCamara = false;
std::vector<std::string> selectedObject;
int pickedObject[] = {2, 3, 1, 6, 4, 5, 0};
int selectedObjectIndex = 0;
int lastSelectObjectIndex = 0;
int symmetryAxis = 0;

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
	myShader->addShader(GL_VERTEX_SHADER, ShaderPath + "myVS.glsl");
	myShader->addShader(GL_FRAGMENT_SHADER, ShaderPath + "myFS.glsl");
	myShader->addAttrib("inPosition", RenderModel::POSITION);
	myShader->addAttrib("inNormal", RenderModel::NORMAL);
	myShader->addAttrib("inTex", RenderModel::TEX);
	myShader->addUniform("Color");
	myShader->addUniform("ModelMatrix");
	myShader->addUniform("LightDirection");
	myShader->addUniformBlock("SharedMatrices", BINDPOINT, sharedMatricesBufferObject);
	myShader->createCompileLink();
	shaderManager->add("SimpleShader", myShader);

	glm::vec3 lightDir = glm::normalize(glm::vec3(-1.0f, 0.0f, -1.0f));

	myShader->bind();
	myShader->sendUniformVec3("LightDirection", lightDir);
	checkOpenGLError("Problem passing LightDirection.");
	myShader->unbind();

	ShaderProgram *selectShader = new ShaderProgram();
	selectShader->addShader(GL_VERTEX_SHADER, ShaderPath + "selectedVS.glsl");
	selectShader->addShader(GL_FRAGMENT_SHADER, ShaderPath + "selectedFS.glsl");
	selectShader->addAttrib("inPosition", RenderModel::POSITION);
	selectShader->addAttrib("inNormal", RenderModel::NORMAL);
	selectShader->addAttrib("inTex", RenderModel::TEX);
	selectShader->addUniform("Color");
	selectShader->addUniform("ModelMatrix");
	selectShader->addUniform("LightDirection");
	selectShader->addUniform("Time0_1");
	selectShader->addUniform("SelectedColor");
	selectShader->addUniformBlock("SharedMatrices", BINDPOINT, sharedMatricesBufferObject);
	selectShader->createCompileLink();
	shaderManager->add("SelectedShader", selectShader);

	glm::vec3 selectedColor = glm::vec3(0.8f, 0.0f, 0.8f);

	selectShader->bind();
	selectShader->sendUniformVec3("LightDirection", lightDir);
	selectShader->sendUniformVec3("SelectedColor", selectedColor);
	selectShader->unbind();

	ShaderProgram * phong = new ShaderProgram();
	phong->addShader(GL_VERTEX_SHADER, ShaderPath + "myPhongVS.glsl");
	phong->addShader(GL_FRAGMENT_SHADER, ShaderPath + "myPhongFS.glsl");
	phong->addAttrib("inPosition", RenderModel::POSITION);
	phong->addAttrib("inNormal", RenderModel::NORMAL);
	phong->addAttrib("inTex", RenderModel::TEX);
	phong->addUniform("Color");
	phong->addUniform("ModelMatrix");
	phong->addUniform("LightDirection");
	phong->addUniform("LightPosition");
	phong->addUniform("LightAmbient");
	phong->addUniform("LightDiffuse");
	phong->addUniform("LightSpecular");
	phong->addUniform("LightAttenuation");
	phong->addUniform("LightRangeLimit");
	phong->addUniform("MaterialEmit");
	phong->addUniform("MaterialAmbient");
	phong->addUniform("MaterialDiffuse");
	phong->addUniform("MaterialSpecular");
	phong->addUniform("MaterialShininess");
	phong->addUniformBlock("SharedMatrices", BINDPOINT, sharedMatricesBufferObject);
	phong->createCompileLink();
	shaderManager->add("PhongShader", phong);

	//glm::vec3 lightDir = glm::normalize(glm::vec3(-1.0f, 0.0f, -1.0f));
	glm::vec3 lampPos = glm::vec3(2.0f, -2.0f, 1.0f);
	glm::vec3 lampAmb = glm::vec3(0.1f);
	glm::vec3 lampDiff = glm::vec3(1.0f);
	glm::vec3 lampSpec = glm::vec3(1.0f);
	glm::vec3 lampAtte = glm::vec3(1.0f, 0.0f, 0.0f);
	float lampRange = -1.0f;

	phong->bind();
	phong->sendUniformVec3("LightDirection", lightDir);
	phong->sendUniformVec3("LightPosition", lampPos);
	phong->sendUniformVec3("LightAmbient", lampAmb);
	phong->sendUniformVec3("LightDiffuse", lampDiff);
	phong->sendUniformVec3("LightSpecular", lampSpec);
	phong->sendUniformVec3("LightAttenuation", lampAtte);
	phong->sendUniformFloat("LightRangeLimit", lampRange);
	checkOpenGLError("Problem passing LightDirection.");
	phong->unbind();
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
	currentTime = (float)glutGet(GLUT_ELAPSED_TIME) * 1E-03f;
	time += (currentTime - oldTime);
	oldTime = currentTime;
}

void sendTimeToShaders() {
	float time0_1 = std::fmod(time, 1.0f);

	ShaderManager * manager = ShaderManager::getInstance();
	ShaderProgram * selectedShader = manager->get("SelectedShader");
	selectedShader->bind();
	selectedShader->sendUniformFloat("Time0_1", time0_1);
	selectedShader->unbind();
}

//TODO remove
glm::mat4 testSubjectMM = glm::translate(glm::mat4(), glm::vec3(-4.0f, 4.0f, 1.0f));
RenderModel *testSubject = 0;
ColorMaterial *testSubjectMat = 0;

void drawScene() {
	glClearStencil(0); // this is the default value
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glm::mat4x4 view = orbit();
	writeSharedMatrices(view, proj);

	ShaderProgram* shader = ShaderManager::getInstance()->get("SimpleShader");
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	world->draw(shader);

	//TODO remove
	ShaderProgram* phong = ShaderManager::getInstance()->get("PhongShader");
	phong->bind();
	testSubjectMat->sendToShader(phong);
	phong->sendUniformMat4("ModelMatrix",testSubjectMM);
	testSubject->drawModel();
	phong->unbind();
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
	timeUpdate();
	sendTimeToShaders();
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
	WorldObject *selectdObj;

	newMx = x;
	newMy = y;
	float dx = (float)(newMx - mx);
	float dy = (float)(newMy - my);

	//move camera
	if (moveCamara){
		theta += 2 * (float)PI * (-dx / WinX);
		phi += 2 * (float)PI * (dy / WinY);

		if (theta < 0){
			theta += (float)(2 * PI);
		}
		if (theta > (float)2 * PI){
			theta -= (float)(2 * PI);
		}

		if (phi < 0){
			phi += (float)(2 * PI);
		}
		if (phi > (float)2 * PI){
			phi -= (float)(2 * PI);
		}

		//std::cout << "   theta:	   " << theta << "	   phi:	    " << phi << std::endl;
	}
	//move the selected object
	else if(selected && !rotateState){
		float step = 10.0f;
		float x, y;

		if (theta > (7.0f / 4.0f * (float)PI) || theta <= (1.0f / 4.0f * (float)PI)){
			x = step * (dy / WinY);
			y = step * (dx / WinX);
		}
		else if (theta > (1.0f / 4.0f * (float)PI) && theta <= (3.0f / 4.0f * (float)PI)){
			x = -step * (dx / WinX);
			y = step * (dy / WinY);
		}
		else if (theta > (3.0f / 4.0f * (float)PI) && theta <= (5.0f / 4.0f * (float)PI)){
			x = -step * (dy / WinY);
			y = -step * (dx / WinX);
		}
		else if (theta > (5.0f / 4.0f * (float)PI) && theta <= (7.0f / 4.0f * (float)PI)){
			x = step * (dx / WinX);
			y = -step * (dy / WinY);
		}
		else{
			std::cerr << "Error on move objects function" << std::endl;
		}

		selectdObj = tangram->at(selectedObject[selectedObjectIndex]);
		selectdObj->translate(glm::vec3(x, y, 0));
	}
	else if (selected && rotateState){
		float aux = (float)fmod(dx, 2);
		selectdObj = tangram->at(selectedObject[selectedObjectIndex]);
		if (dx < 0 && aux == 0){
			selectdObj->rotate(glm::quat(cosf((-(float)PI / 8.0f) / 2.0f), 0.0f, 0.0f, sinf((-(float)PI / 8.0f) / 2.0f)));
		}
		if (dx > 0 && aux == 0){
			selectdObj->rotate(glm::quat(cosf(((float)PI / 8.0f) / 2.0f), 0.0f, 0.0f, sinf(((float)PI / 8.0f) / 2.0f)));
		}
	}

	mx = newMx;
	my = newMy;
}

/////////////////////////////////////////////////////////////////////// SCENE OBJECT MANIPULATION

void changeSelectedObjectShader()
{
	world->setObjectShader(selectedObject[lastSelectObjectIndex], 0);
	if (selected) {
		ShaderManager *manager = ShaderManager::getInstance();
		ShaderProgram *selectedShader = manager->get("SelectedShader");
		world->setObjectShader(selectedObject[selectedObjectIndex], selectedShader);
	}
}

void mousePressed(int button, int state, int x, int y) {
	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN){
		GLuint index;
		glReadPixels(x, WinY - y - 1, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);

		if (index >= 2){
			selected = true;
			lastSelectObjectIndex = selectedObjectIndex;
			selectedObjectIndex = pickedObject[index-2];
			changeSelectedObjectShader();

			mx = x;
			my = y;
		}
	}
	
	if (button == GLUT_RIGHT_BUTTON && selected){
		if (state == GLUT_DOWN){
			rotateState = true;
			mx = x;
			my = y;
		}
		else{
			rotateState = false;
		}
	}
	if (button == GLUT_LEFT_BUTTON){
		if (state == GLUT_DOWN){
			if (selected){
				selected = false;
				lastSelectObjectIndex = selectedObjectIndex;
				selectedObjectIndex = 0;
				changeSelectedObjectShader();
			}
			
			moveCamara = true;
			mx = x;
			my = y;
		}
		else{
			moveCamara = false;
		}
	}
}


void keyboardKey(unsigned char key, int x, int y) {
	if (key == 'c'){
		cameraCenter = glm::vec3(0.0f);
		proj = glm::ortho(-8.5f, 8.5f, -8.5f, 8.5f, 0.f, 50.f);
	}

	if (key == 'v'){
		std::map<std::string, WorldObject*> tangramObject = *tangram;
		
		cameraCenter = tangramObject["BackPlane"]->getPosition();
		proj = glm::ortho(-4.0f, 4.0f, -4.0f, 4.0f, 0.f, 50.f);
	}

	if (key == 'a'){
		symmetryAxis++;
		symmetryAxis = symmetryAxis % 3;
		std::cout << symmetryAxis << std::endl;
		world->setSymmetryAxis(symmetryAxis);
	}
	if (key == 's'){
		world->save(symmetryAxis);
	}
	if (key == 'l'){
		world->load(&symmetryAxis);
	}

}

void SpecialkeyboardKey(int key, int x, int y){
	
}

/////////////////////////////////////////////////////////////////////// SETUP
void buildTangram() {
	std::map<std::string, WorldObject*> tangramObject = *tangram;
	WorldObject* aux;
	MaterialManager *manager = MaterialManager::instance();
	ColorMaterial *matAux = 0;
	
	/* Square */
	aux = tangramObject["Square"];
	matAux = manager->get("Square.mtl");
	aux->setColor(*matAux);
	aux->setPosition(glm::vec3(-3.5f, 4.0f, 0.0f));
	aux->setQuaternion(glm::quat(cosf(0), 0.0f, 0.0f, sinf(0)));
	/*Medium Triangle*/
	aux = tangramObject["MedTri"];
	matAux = manager->get("MedTri.mtl");
	aux->setColor(*matAux);
	aux->setPosition(glm::vec3(-3.0f, 3.0f, 0.0f));
	aux->setQuaternion(glm::quat(cosf(0), 0.0f, 0.0f, sinf(0)));
	/* Big Triangle 1 */
	aux = tangramObject["BigTri1"];
	matAux = manager->get("BigTri1.mtl");
	aux->setColor(*matAux);
	aux->setPosition(glm::vec3(-4.0f, 4.0f, 0.0f));
	aux->setQuaternion(glm::quat(cosf(((float)PI / 2) / 2), 0.0f, 0.0f, sinf(((float)PI / 2) / 2)));
	/* Big Triangle 2 */
	aux = tangramObject["BigTri2"];
	matAux = manager->get("BigTri2.mtl");
	aux->setColor(*matAux);
	aux->setPosition(glm::vec3(-4.0f, 4.0f, 0.0f));
	aux->setQuaternion(glm::quat(cosf(0), 0.0f, 0.0f, sinf(0)));
	/* Small Triangle 1 */
	aux = tangramObject["SmallTri1"];
	matAux = manager->get("SmallTri1.mtl");
	aux->setColor(*matAux);
	aux->setPosition(glm::vec3(-3.5f, 4.5f, 0.0f));
	aux->setQuaternion(glm::quat(cosf(0), 0.0f, 0.0f, sinf(0)));
	/* Small Triangle 2 */
	aux = tangramObject["SmallTri2"];
	matAux = manager->get("SmallTri2.mtl");
	aux->setColor(*matAux);
	aux->setPosition(glm::vec3(-4.0f, 4.0f, 0.0f));
	aux->setQuaternion(glm::quat(cosf((-(float)PI / 2) / 2), 0.0f, 0.0f, sinf((-(float)PI / 2) / 2)));
	/* Parallelogram */
	aux = tangramObject["Quad"];
	matAux = manager->get("Quad.mtl");
	aux->setColor(*matAux);
	aux->setPosition(glm::vec3(-5.0f, 3.0f, 0.0f));
	aux->setQuaternion(glm::quat(cosf(0), 0.0f, 0.0f, sinf(0)));
	/* Back Plane */
	aux = tangramObject["BackPlane"];
	matAux = manager->get("BackPlane.mtl");
	aux->setColor(*matAux);
	aux->setPosition(glm::vec3(-4.0f, 4.0f, 0.0f));
}



void loadModels() {
	ModelLoader modelLoader;

	tangram = new std::map<std::string, WorldObject*>();

	WorldObject * aux;
	std::string name;

	RenderModelManager* renderManager = RenderModelManager::instance();

	renderManager->addRenderModel(modelLoader.loadModel("Square", ModelPath + "Square.obj"));
	aux = new WorldObject("Square", renderManager->getRenderModel("Square"));
	world->add(aux);
	tangram->operator[]("Square") = aux;
	selectedObject.push_back("Square");

	renderManager->addRenderModel(modelLoader.loadModel("MedTri", ModelPath + "MedTri.obj"));
	aux = new WorldObject("MedTri", renderManager->getRenderModel("MedTri"));
	world->add(aux);
	tangram->operator[]("MedTri") = aux;
	selectedObject.push_back("MedTri");

	renderManager->addRenderModel(modelLoader.loadModel("BigTri1", ModelPath + "BigTri.obj"));
	aux = new WorldObject("BigTri1", renderManager->getRenderModel("BigTri1"));
	world->add(aux);
	tangram->operator[]("BigTri1") = aux;
	selectedObject.push_back("BigTri1");

	renderManager->addRenderModel(modelLoader.loadModel("BigTri2", ModelPath + "BigTri.obj"));
	aux = new WorldObject("BigTri2", renderManager->getRenderModel("BigTri2"));
	world->add(aux);
	tangram->operator[]("BigTri2") = aux;
	selectedObject.push_back("BigTri2");

	renderManager->addRenderModel(modelLoader.loadModel("SmallTri1", ModelPath + "SmallTri.obj"));
	aux = new WorldObject("SmallTri1", renderManager->getRenderModel("SmallTri1"));
	world->add(aux);
	tangram->operator[]("SmallTri1") = aux;
	selectedObject.push_back("SmallTri1");

	renderManager->addRenderModel(modelLoader.loadModel("SmallTri2", ModelPath + "SmallTri.obj"));
	aux = new WorldObject("SmallTri2", renderManager->getRenderModel("SmallTri2"));
	world->add(aux);
	tangram->operator[]("SmallTri2") = aux;
	selectedObject.push_back("SmallTri2");

	renderManager->addRenderModel(modelLoader.loadModel("Quad", ModelPath + "Quad.obj"));
	aux = new WorldObject("Quad", renderManager->getRenderModel("Quad"));
	world->add(aux);
	tangram->operator[]("Quad") = aux;
	selectedObject.push_back("Quad");

	renderManager->addRenderModel(modelLoader.loadModel("BackPlane", ModelPath + "BackPlane.obj"));
	aux = new WorldObject("BackPlane", renderManager->getRenderModel("BackPlane"));
	world->add(aux);
	tangram->operator[]("BackPlane") = aux;

	//TODO remove
	testSubject = modelLoader.loadModel("TestSubject", ModelPath + "Sphere.obj");
}

void loadMaterials()
{
	std::string matsListStr = readFromFile(MaterialPath + MaterialListFile);
	std::istringstream stream(matsListStr);
	std::vector<std::string> matList;
	std::string matEntry;
	while (getline(stream, matEntry))
		matList.push_back(matEntry);
	MaterialManager *manager = MaterialManager::instance();
	manager->loadFileList(MaterialPath, matList);

	//TODO remove
	testSubjectMat = manager->get("Monkey.mtl");
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
	std::map<std::string, WorldObject*> tangramObject = *tangram;

	theta = (3 * (float)PI)/2.0f;
	phi = (float)PI / 4.0f;
	raw = 10.0f;
	cameraCenter = tangramObject["BackPlane"]->getPosition();
	proj = glm::ortho(-4.f, 4.f, -4.f, 4.f, 0.f, 50.f);
}

void initTime()
{
	oldTime = (float)glutGet(GLUT_ELAPSED_TIME) * 1E-03f;
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
	loadMaterials();
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
