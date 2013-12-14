#include "OpenGLGlobalsManager.h"

bool OpenGLGlobalsManager::GLFrontFaceCCW=true;
bool OpenGLGlobalsManager::GLFrontFaceCCWApplied = true;
void OpenGLGlobalsManager::flipGLFrontFace()
{
	GLFrontFaceCCW = !GLFrontFaceCCW;
	GLFrontFaceCCWApplied = !GLFrontFaceCCWApplied;
	//applyGLFrontFace();
}
void  OpenGLGlobalsManager::setGLFrontFaceCCW()
{
	GLFrontFaceCCWApplied = GLFrontFaceCCW;
	GLFrontFaceCCW = true;
	//applyGLFrontFace();
}
void  OpenGLGlobalsManager::setGLFrontFaceCW()
{
	GLFrontFaceCCWApplied = !GLFrontFaceCCW;
	GLFrontFaceCCW = false;
	//applyGLFrontFace();
}
void  OpenGLGlobalsManager::applyGLFrontFace()
{
	if (GLFrontFaceCCWApplied)
		return;
	if (GLFrontFaceCCW)
		glFrontFace(GL_CCW);
	else
		glFrontFace(GL_CW);
	GLFrontFaceCCWApplied = true;
}
bool OpenGLGlobalsManager::isGLFrontFaceCCW()
{
	return GLFrontFaceCCW;
}