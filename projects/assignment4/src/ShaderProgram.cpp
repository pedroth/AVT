#include "ShaderProgram.h"

#include <iostream>
#include "utils.h"


// Creates a new program and its shaders.
ShaderProgram::ShaderProgram()
: m_programName(0), m_shaders(), m_uniforms(), m_attribs(), m_uniformBlocks()
{
}


// Detach shaders, destroy shaders.
ShaderProgram::~ShaderProgram()
{
	glUseProgram(0);
	for (std::map<GLenum, Shader *>::iterator it = m_shaders.begin(); it != m_shaders.end(); ++it)
	{
		glDetachShader(m_programName, (it->second)->shaderName());
		delete it->second;
	}

	glDeleteProgram(m_programName);
}


void ShaderProgram::addShader(GLenum shaderType, const std::string &shaderPath)
{
	Shader *oldShader = m_shaders[shaderType];
	if (oldShader != 0)
		delete oldShader;
	std::string shaderSource = readFromFile(shaderPath);
	m_shaders[shaderType] = new Shader(shaderType, shaderSource);
}


void ShaderProgram::addAttrib(const std::string &name, GLuint index)
{
	AttribData attrib;
	attrib.name = name; attrib.index = index;
	m_attribs[name] = attrib;
}


void ShaderProgram::addUniform(const std::string &uniformName)
{
	UniformData uni;
	uni.name = uniformName; uni.id = -1;
	m_uniforms[uniformName] = uni;
}


void ShaderProgram::addUniformBlock(const std::string &blockName, GLuint bindPoint, GLuint bufferID)
{
	UniformBlockData uniBlock;
	uniBlock.name = blockName; 
	uniBlock.bindPoint = bindPoint; 
	uniBlock.bufferID = bufferID;
	m_uniformBlocks[blockName] = uniBlock;
}


// Create, compile and link program.
void ShaderProgram::createCompileLink()
{
	m_programName = glCreateProgram();
	for (std::map<GLenum, Shader *>::iterator it = m_shaders.begin(); it != m_shaders.end(); ++it)
	{
		(it->second)->createShader();
		glAttachShader(m_programName,(it->second)->shaderName());
	}

	checkOpenGLError("Error attaching shaders.");

	bool shaderCompilationFailed = false;
	for (std::map<GLenum, Shader *>::iterator it = m_shaders.begin(); it != m_shaders.end(); ++it)
	{
		GLint compileStatus = (it->second)->compile();
		if (compileStatus == GL_FALSE){
			shaderCompilationFailed = true;
			displayShaderCompileLog("Shader compilation failed", *(it->second));
		}
	}

	checkOpenGLError("Error compiling shaders.");
	
	// fail if some compilation failed
	if (shaderCompilationFailed)
		std::cout << "ERROR: Shader compilation failed" << std::endl;

	// "bind" vertex attribute channels
	for (auto& attrib : m_attribs){
		glBindAttribLocation(m_programName, attrib.second.index, attrib.second.name.c_str());
	}

	checkOpenGLError("Error binding attrib locations.");

	// link program and check for errors
	GLint linkResult = linkProgram();
	if (linkResult == GL_FALSE) {
		displayProgramLinkingLog("Shader Program linking failed");
	}

	// get uniform ids
	for (std::map<std::string, UniformData>::iterator it = m_uniforms.begin(); it != m_uniforms.end(); ++it) {
		const std::string &uniName = it->second.name;
		GLint loc = glGetUniformLocation(m_programName, uniName.c_str());
		it->second.id = loc;

		if (loc == -1) {
			std::cerr << "[Warning] Uniform not found: '" << uniName << "'" << std::endl;
		}
	}

	checkOpenGLError("Error getting unfiorm locations.");

	// bind uniform blocks (the buffer still needs to be assigned to the same bind point)
	for (std::map<std::string, UniformBlockData>::iterator it = m_uniformBlocks.begin(); it != m_uniformBlocks.end(); ++it) {
		const std::string &blockName = it->second.name;
		GLuint blockIndex = glGetUniformBlockIndex(m_programName, blockName.c_str());
		glUniformBlockBinding(m_programName, blockIndex, it->second.bindPoint);
	}

	checkOpenGLError("Error binding uniform blocks.");
}

GLint ShaderProgram::linkProgram()
{
	// Link program
	glLinkProgram(m_programName);
	// Get GL_LINK_STATUS
	GLint linkResult = GL_FALSE;
	glGetProgramiv(m_programName, GL_LINK_STATUS, &linkResult);
	return linkResult;
}

std::string ShaderProgram::linkingInfoLog()
{
	// Get log info size
	GLint logInfoSize;
	glGetProgramiv(m_programName, GL_INFO_LOG_LENGTH, &logInfoSize);
	// Get shader compilation info log string
	if (logInfoSize == 0)
		return std::string();
	// FIXME using a char array for interfacing with glGetProgramInfoLog
	char *logInfoArray = new char[logInfoSize];
	glGetProgramInfoLog(m_programName, logInfoSize, 0, logInfoArray);
	std::string logInfoString(logInfoArray);
	delete[] logInfoArray;
	return logInfoString;
}

void ShaderProgram::displayProgramLinkingLog(const std::string &message)
{
	std::cout << message << std::endl;
	std::cout << "Printing info log:" << std::endl;
	std::cout << linkingInfoLog() << std::endl;
}


void ShaderProgram::displayShaderCompileLog(const std::string &message, const Shader &shader)
{
	std::cout << message << std::endl;
	std::cout << "Printing info log:" << std::endl;
	std::cout << shader.compileLogInfo() << std::endl;
}


GLint ShaderProgram::getUniformId(const std::string &uniformName)
{
	return m_uniforms[uniformName].id;
}


void ShaderProgram::sendUniformMat4(const std::string &uniformName, const glm::mat4x4 &mat)
{
	if (!uniformExists(uniformName))
		return;
	GLint uniformId = m_uniforms[uniformName].id;
	glUniformMatrix4fv(uniformId, 1, GL_FALSE, glm::value_ptr(mat));
}


void ShaderProgram::sendUniformMat3(const std::string &uniformName, const glm::mat3 &mat) {
	if (!uniformExists(uniformName))
		return;
	GLint uniformId = m_uniforms[uniformName].id;
	glUniformMatrix3fv(uniformId, 1, GL_FALSE, glm::value_ptr(mat));
}


void ShaderProgram::sendUniformVec3(const std::string &uniformName, const glm::vec3 &vec)
{
	if (!uniformExists(uniformName))
		return;
	GLint uniformId = m_uniforms[uniformName].id;
	glUniform3f(uniformId, vec.x, vec.y, vec.z);
}

void ShaderProgram::sendUniformFloat(const std::string &uniformName, const float &f)
{
	GLint uniformId = m_uniforms[uniformName].id;
	glUniform1f(uniformId, f);
}


// Returns this program's OpenGL name.
GLuint ShaderProgram::programName()
{
	return m_programName;
}


void ShaderProgram::bind()
{
	glUseProgram(m_programName);
}


void ShaderProgram::unbind()
{
	glUseProgram(0);
}

bool ShaderProgram::uniformExists(std::string uniName)
{
	uniforms_type::iterator it = m_uniforms.find(uniName);
	if (it == m_uniforms.end())
		return false;
	if ((it->second).id == -1)
		return false;

	return true;
}