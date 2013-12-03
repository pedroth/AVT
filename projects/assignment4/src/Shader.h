#pragma once

#include <string>
#include <GL\glew.h>

class Shader
{
public:
	// Initializes client-side object (no OpenGL calls).
	// This object will only be properly initialized after calling
	// createShader and then compileShader methods.
	Shader(GLenum shaderType, const std::string &shaderSource);
	// Issues destruction of the OpenGL shader object associated with this.
	~Shader();
	// Returns OpenGL shader name of this.
	GLuint shaderName();
	// Create the new OpenGL shader object and specify it's source.
	// If shader object was already created nothing is done.
	void createShader();
	// Issues GLSL source compilation.
	// returns GL_COMPILE_STATUS for this shader.
	GLint compile();
	// Fetches the compilation log info for this shader.
	// If the log is empty then returns an empty string.
	std::string compileLogInfo() const;
private:
	GLuint m_shaderName;
	GLenum m_shaderType;
	std::string m_shaderSource;
};

