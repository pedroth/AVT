#pragma once

#include <GL\glew.h>

class OpenGLGlobalsManager {
	
private:
	static bool GLFrontFaceCCW;
	static bool GLFrontFaceCCWApplied;
public:
	static void applyGLFrontFace();
	static void flipGLFrontFace();
	static void setGLFrontFaceCCW();
	static void setGLFrontFaceCW();
	static bool isGLFrontFaceCCW();
};