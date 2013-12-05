
#include "utils.h"
#include "GL\glew.h"
#include <iostream>
#include <fstream>
#include <sstream>

const float PI_CONSTANT = 3.14159265359f;
float deg2Rad(float degrees)
{
	return (degrees / 180.0f) * PI_CONSTANT;
}

int positiveModulo(int i, int n) {
	return (i % n + n) % n;
}


std::string readFromFile(const std::string &filePath)
{
	std::ostringstream readText;
	std::ifstream fileStream(filePath, std::ios::in);
	if (fileStream.is_open())
	{
		std::string Line;
		while (getline(fileStream, Line))
			readText << Line << std::endl;
		fileStream.close();
	}

	return readText.str();
}

bool isOpenGLError() {
	bool isError = false;
	GLenum errCode;
	const GLubyte *errString;
	while ((errCode = glGetError()) != GL_NO_ERROR) {
		isError = true;
		errString = gluErrorString(errCode);
		std::cout << "OpenGL ERROR [" << errString << "]." << std::endl;
	}
	return isError;
}

void checkOpenGLError(std::string error)
{
	if (isOpenGLError()) {
		std::cout << error << std::endl;
		exit(EXIT_FAILURE);
	}
}