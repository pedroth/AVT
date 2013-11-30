#include "Shader.h"


Shader::Shader(GLenum shaderType, const std::string &shaderSource)
: m_shaderName(0), m_shaderType(shaderType), m_shaderSource(shaderSource)
{
}


Shader::~Shader()
{
	// Release OpenGL shader object
	glDeleteShader(m_shaderName);
}


// Create the new OpenGL shader object and specify it's source.
void Shader::createShader()
{
	if (m_shaderName != 0)
		return;
	// Create OpenGL shader object
	m_shaderName = glCreateShader(m_shaderType);
	// Send shader source to OpenGL
	const char *shaderSourceCstr = m_shaderSource.c_str();
	glShaderSource(m_shaderName, 1, &shaderSourceCstr, 0);
}


// Issues GLSL source compilation.
// returns GL_COMPILE_STATUS for this shader.
GLint Shader::compile()
{
	// Issue shader compilation
	glCompileShader(m_shaderName);
	// return GL_COMPILE_STATUS
	GLint compileStatus = GL_FALSE;
	glGetShaderiv(m_shaderName, GL_COMPILE_STATUS, &compileStatus);
	return compileStatus;
}


// Fetches the compilation log info for this shader.
// If the log is empty then returns an empty string.
std::string Shader::compileLogInfo() const
{
	// Get log info size
	GLint logInfoSize;
	glGetShaderiv(m_shaderName, GL_INFO_LOG_LENGTH, &logInfoSize);
	// Get shader compilation info log string
	if (logInfoSize == 0)
		return std::string();
	// FIXME using a char array for interfacing with glGetShaderInfoLog
	char *logInfoArray = new char[logInfoSize];
	glGetShaderInfoLog(m_shaderName, logInfoSize, 0, logInfoArray);
	std::string logInfoString(logInfoArray);
	delete[] logInfoArray;
	return logInfoString;
}


// Returns OpenGL shader name of this.
GLuint Shader::shaderName()
{
	return m_shaderName;
}