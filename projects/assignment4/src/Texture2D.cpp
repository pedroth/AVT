#include "Texture2D.h"

#include <iostream>
#include <FreeImagePlus.h>

Texture2D::Texture2D()
: _texId(0)
{
	glGenTextures(1, &_texId);
	bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	unbind();
}

Texture2D::~Texture2D()
{
	glDeleteTextures(1, &_texId);
}

bool Texture2D::loadTextureRGB(std::string texPath)
{
	//load and convert to 24bit RGB unsigned byte
	fipImage image;
	BOOL loaded = image.load(texPath.c_str());
	if (loaded==FALSE) {
		//TODO move this to the caller or to logger or something...
		std::cerr << "Texture could not be loaded from file: '" << texPath << "'." << std::endl;
		return false;
	}
	image.convertTo24Bits();

	bind();
	//load to GPU
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.getWidth(), image.getHeight(), 0, 
		GL_BGR, GL_UNSIGNED_BYTE, image.accessPixels());
	unbind();

	//unload application-side image
	image.clear();

	return true;
}

void Texture2D::bind()
{
	glBindTexture(GL_TEXTURE_2D, _texId);
}
void Texture2D::unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}