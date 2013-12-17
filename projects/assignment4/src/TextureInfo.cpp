#include "TextureInfo.h"

TextureInfo::TextureInfo(Texture2D *tex, std::string name, unsigned int texUnit)
: _tex(tex), _texName(name), _texUnit(texUnit)
{}
void TextureInfo::setInShaderProgram(ShaderProgram *program)
{
	bind();
	program->sendUnifomInt(_texName, _texUnit);
}
unsigned int TextureInfo::texUnit()
{
	return _texUnit;
}
void TextureInfo::bind()
{
	glActiveTexture(GL_TEXTURE0 + _texUnit);
	_tex->bind();
}