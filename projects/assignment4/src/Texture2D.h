#pragma once

#include <GL\glew.h>
#include <string>

class Texture2D {
	GLuint _texId;
public:
	Texture2D();
	~Texture2D();
	bool loadTextureRGB(std::string texPath);
	void bind();
	void unbind();
};