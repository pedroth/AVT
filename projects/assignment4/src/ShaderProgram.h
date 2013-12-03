#ifndef __SHADER_PROGRAM_H__
#define __SHADER_PROGRAM_H__

#include <GL\glew.h>

#include <glm.hpp>
#include <gtc/type_ptr.hpp>

#include <string>
#include <map>
#include "Shader.h"

struct AttribData {
	std::string name;
	GLuint index;
};

struct UniformData {
	std::string name;
	GLint id;
};

struct UniformBlockData {
	std::string name;
	GLuint bindPoint;
	GLuint bufferID;
};

class ShaderProgram
{
	GLuint m_programName;
	std::map<GLenum, Shader *> m_shaders;
	std::map<std::string, AttribData> m_attribs;
	std::map<std::string, UniformData> m_uniforms;
	std::map<std::string, UniformBlockData> m_uniformBlocks;
public:
	// Create a new OpenGL shader program.
	ShaderProgram();
	// Detach shaders, destroy shaders.
	~ShaderProgram();
	void addShader(GLenum shaderType, const std::string &shaderSource);
	void addAttrib(const std::string &name, GLuint index);
	void addUniform(const std::string &uniformName);
	void addUniformBlock(const std::string &blockName, GLuint bindPoint, GLuint bufferID);
	// Create, compile and link program.
	// After calling this mehtod the program is/should be ready for execution.
	// TODO uniform ids are not being gathered.
	void createCompileLink();
	// Returns this program's OpenGL name.
	GLuint programName();
	// Sets the current OpenGL shader program to this.
	void use();
	// Clear OpenGL program binding.
	void removeFromUse();
	// Get a uniform id by uniform name.
	GLint getUniformId(const std::string &uniformName);
	void sendUniformMat4(const std::string &uniformName, const glm::mat4x4 &mat);
	void sendUniformVec3(const std::string &uniformName, const glm::vec3 &vec);
private:
	void displayShaderCompileLog(const std::string &message, const Shader &shader);
	void displayProgramLinkingLog(const std::string &message);
	std::string linkingInfoLog();
	GLint linkProgram();
};


#endif