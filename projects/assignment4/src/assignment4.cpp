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

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "engine.h"

#define CAPTION "Hello New World"

int WinX = 640, WinY = 480;
int WindowHandle = 0;
unsigned int FrameCount = 0;

#define VERTICES 0
#define COLORS 1
#define PI 3.14159265359

GLuint VaoId, VboId[2];
GLuint VertexShaderId, FragmentShaderId, ProgramId;
GLint UboId, UniformId;
const GLuint UBO_BP = 0;
/*
mouse coordinates
*/
int mx, my, newMx, newMy;
/*
spherical coordinates of the camera
*/
float theta, phi;
/////////////////////////////////////////////////////////////////////// ERRORS

bool isOpenGLError() {
	bool isError = false;
	GLenum errCode;
	const GLubyte *errString;
	while ((errCode = glGetError()) != GL_NO_ERROR) {
		isError = true;
		errString = gluErrorString(errCode);
		std::cerr << "OpenGL ERROR [" << errString << "]." << std::endl;
	}
	return isError;
}

void checkOpenGLError(std::string error)
{
	if (isOpenGLError()) {
		std::cerr << error << std::endl;
		exit(EXIT_FAILURE);
	}
}

/////////////////////////////////////////////////////////////////////// SHADERs

const GLchar* VertexShader =
{
	"#version 330 core\n"

	"in vec4 in_Position;\n"
	"in vec4 in_Color;\n"
	"out vec4 ex_Color;\n"

	"uniform mat4 ModelMatrix;\n"

	"layout(std140) uniform SharedMatrices\n"
	"{\n"
	"	mat4 ViewMatrix;\n"
	"	mat4 ProjectionMatrix;\n"
	"};\n"

	"void main(void)\n"
	"{\n"
	"	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * in_Position;\n"
	"	ex_Color = in_Color;\n"
	"}\n"
};

const GLchar* FragmentShader =
{
	"#version 330 core\n"

	"in vec4 ex_Color;\n"
	"out vec4 out_Color;\n"

	"void main(void)\n"
	"{\n"
	"	out_Color = ex_Color;\n"
	"}\n"
};

void createShaderProgram()
{
	VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShaderId, 1, &VertexShader, 0);
	glCompileShader(VertexShaderId);

	FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShaderId, 1, &FragmentShader, 0);
	glCompileShader(FragmentShaderId);

	ProgramId = glCreateProgram();
	glAttachShader(ProgramId, VertexShaderId);
	glAttachShader(ProgramId, FragmentShaderId);

	glBindAttribLocation(ProgramId, VERTICES, "in_Position");
	glBindAttribLocation(ProgramId, COLORS, "in_Color");
	glLinkProgram(ProgramId);
	UniformId = glGetUniformLocation(ProgramId, "ModelMatrix");
	UboId = glGetUniformBlockIndex(ProgramId, "SharedMatrices");
	glUniformBlockBinding(ProgramId, UboId, UBO_BP);

	checkOpenGLError("ERROR: Could not create shaders.");
}

void destroyShaderProgram()
{
	glUseProgram(0);
	glDetachShader(ProgramId, VertexShaderId);
	glDetachShader(ProgramId, FragmentShaderId);

	glDeleteShader(FragmentShaderId);
	glDeleteShader(VertexShaderId);
	glDeleteProgram(ProgramId);

	checkOpenGLError("ERROR: Could not destroy shaders.");
}

/////////////////////////////////////////////////////////////////////// VAOs & VBOs

typedef struct {
	GLfloat XYZW[4];
	GLfloat RGBA[4];
} Vertex;

typedef GLfloat Matrix[16];

const Vertex Vertices[] =
{
	{ { 0.0f, 0.0f, 1.0f, 1.0f }, { 0.9f, 0.0f, 0.0f, 1.0f } }, // 0 - FRONT
	{ { 1.0f, 0.0f, 1.0f, 1.0f }, { 0.9f, 0.0f, 0.0f, 1.0f } }, // 1
	{ { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.9f, 0.0f, 0.0f, 1.0f } }, // 2
	{ { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.9f, 0.0f, 0.0f, 1.0f } }, // 2	
	{ { 0.0f, 1.0f, 1.0f, 1.0f }, { 0.9f, 0.0f, 0.0f, 1.0f } }, // 3
	{ { 0.0f, 0.0f, 1.0f, 1.0f }, { 0.9f, 0.0f, 0.0f, 1.0f } }, // 0

	{ { 1.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.9f, 0.0f, 1.0f } }, // 1 - RIGHT
	{ { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.9f, 0.0f, 1.0f } }, // 5
	{ { 1.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.9f, 0.0f, 1.0f } }, // 6
	{ { 1.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.9f, 0.0f, 1.0f } }, // 6	
	{ { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.9f, 0.0f, 1.0f } }, // 2
	{ { 1.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.9f, 0.0f, 1.0f } }, // 1

	{ { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.9f, 1.0f } }, // 2 - TOP
	{ { 1.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.9f, 1.0f } }, // 6
	{ { 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.9f, 1.0f } }, // 7
	{ { 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.9f, 1.0f } }, // 7	
	{ { 0.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.9f, 1.0f } }, // 3
	{ { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.9f, 1.0f } }, // 2

	{ { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.9f, 0.9f, 1.0f } }, // 5 - BACK
	{ { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.9f, 0.9f, 1.0f } }, // 4
	{ { 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.9f, 0.9f, 1.0f } }, // 7
	{ { 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.9f, 0.9f, 1.0f } }, // 7	
	{ { 1.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.9f, 0.9f, 1.0f } }, // 6
	{ { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.9f, 0.9f, 1.0f } }, // 5

	{ { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.9f, 0.0f, 0.9f, 1.0f } }, // 4 - LEFT
	{ { 0.0f, 0.0f, 1.0f, 1.0f }, { 0.9f, 0.0f, 0.9f, 1.0f } }, // 0
	{ { 0.0f, 1.0f, 1.0f, 1.0f }, { 0.9f, 0.0f, 0.9f, 1.0f } }, // 3
	{ { 0.0f, 1.0f, 1.0f, 1.0f }, { 0.9f, 0.0f, 0.9f, 1.0f } }, // 3	
	{ { 0.0f, 1.0f, 0.0f, 1.0f }, { 0.9f, 0.0f, 0.9f, 1.0f } }, // 7
	{ { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.9f, 0.0f, 0.9f, 1.0f } }, // 4

	{ { 0.0f, 0.0f, 1.0f, 1.0f }, { 0.9f, 0.9f, 0.0f, 1.0f } }, // 0 - BOTTOM
	{ { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.9f, 0.9f, 0.0f, 1.0f } }, // 4
	{ { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.9f, 0.9f, 0.0f, 1.0f } }, // 5
	{ { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.9f, 0.9f, 0.0f, 1.0f } }, // 5	
	{ { 1.0f, 0.0f, 1.0f, 1.0f }, { 0.9f, 0.9f, 0.0f, 1.0f } }, // 1
	{ { 0.0f, 0.0f, 1.0f, 1.0f }, { 0.9f, 0.9f, 0.0f, 1.0f } }  // 0
};

void createBufferObjects()
{
	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);

	glGenBuffers(2, VboId);
	glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(VERTICES);
	glVertexAttribPointer(VERTICES, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(COLORS);
	glVertexAttribPointer(COLORS, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)sizeof(Vertices[0].XYZW));

	glBindBuffer(GL_UNIFORM_BUFFER, VboId[1]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(Matrix)* 2, 0, GL_STREAM_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, UBO_BP, VboId[1]);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glDisableVertexAttribArray(VERTICES);
	glDisableVertexAttribArray(COLORS);

	checkOpenGLError("ERROR: Could not create VAOs and VBOs.");
}

void destroyBufferObjects()
{
	glDisableVertexAttribArray(VERTICES);
	glDisableVertexAttribArray(COLORS);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glDeleteBuffers(2, VboId);
	glDeleteVertexArrays(1, &VaoId);
	checkOpenGLError("ERROR: Could not destroy VAOs and VBOs.");
}

/////////////////////////////////////////////////////////////////////// SCENE

const Matrix I = {
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
};

const Matrix ModelMatrix = {
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 1.0f
}; // Column Major

// Eye(5,5,5) Center(0,0,0) Up(0,1,0)
const Matrix ViewMatrix1 = {
	0.70f, -0.41f, 0.58f, 0.00f,
	0.00f, 0.82f, 0.58f, 0.00f,
	-0.70f, -0.41f, 0.58f, 0.00f,
	0.00f, 0.00f, -8.70f, 1.00f
}; // Column Major

// Eye(-5,-5,-5) Center(0,0,0) Up(0,1,0)
const Matrix ViewMatrix2 = {
	-0.70f, -0.41f, -0.58f, 0.00f,
	0.00f, 0.82f, -0.58f, 0.00f,
	0.70f, -0.41f, -0.58f, 0.00f,
	0.00f, 0.00f, -8.70f, 1.00f
}; // Column Major

// Orthographic LeftRight(-2,2) TopBottom(-2,2) NearFar(1,10)
const Matrix ProjectionMatrix1 = {
	0.50f, 0.00f, 0.00f, 0.00f,
	0.00f, 0.50f, 0.00f, 0.00f,
	0.00f, 0.00f, -0.22f, 0.00f,
	0.00f, 0.00f, -1.22f, 1.00f
}; // Column Major

// Perspective Fovy(30) Aspect(640/480) NearZ(1) FarZ(10)
const Matrix ProjectionMatrix2 = {
	2.79f, 0.00f, 0.00f, 0.00f,
	0.00f, 3.73f, 0.00f, 0.00f,
	0.00f, 0.00f, -1.22f, -1.00f,
	0.00f, 0.00f, -2.22f, 0.00f
}; // Column Major

void drawScene()
{
	glBindBuffer(GL_UNIFORM_BUFFER, VboId[1]);
	
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Matrix), ViewMatrix2);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Matrix), sizeof(Matrix), ProjectionMatrix2);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindVertexArray(VaoId);
	glUseProgram(ProgramId);

	glUniformMatrix4fv(UniformId, 1, GL_FALSE, I/*ModelMatrix*/);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glUseProgram(0);
	glBindVertexArray(0);

	checkOpenGLError("ERROR: Could not draw scene.");
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

void init(int argc, char* argv[])
{
	setupGLUT(argc, argv);
	setupGLEW();
	setupOpenGL();
	createShaderProgram();
	createBufferObjects();
	setupCallbacks();
	theta = 0.0f;
	phi = 0.0f;
}

int main(int argc, char* argv[])
{
	init(argc, argv);
	glutMainLoop();
	exit(EXIT_SUCCESS);
}

///////////////////////////////////////////////////////////////////////
