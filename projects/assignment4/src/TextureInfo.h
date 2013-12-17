#pragma once

#include <string>
#include <GL/glew.h>

#include "ShaderProgram.h"
#include "Texture2D.h"

class TextureInfo {
	//The texture in the GPU.
	Texture2D *_tex;
	//The name of the sampler in the shader.
	std::string _texName;
	//The Texture Unit to be used.
	unsigned int _texUnit;
public:
	TextureInfo(Texture2D *tex, std::string name, unsigned int texUnit);
	void setInShaderProgram(ShaderProgram *program);
	GLenum texUnit();
	void bind();
};