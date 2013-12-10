#include "ColorMaterial.h"

ColorMaterial::ColorMaterial() 
: ColorMaterial(glm::vec3(0.5f)) 
{}
ColorMaterial::ColorMaterial(glm::vec3 color) 
: ColorMaterial(glm::vec3(0.0f), glm::vec3(1.0f), color, glm::vec3(0.0f),0.0f)
{}
ColorMaterial::ColorMaterial(glm::vec3 emit, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shine)
: emitCol(emit), ambCol(amb), diffCol(diff), specCol(spec), shininess(shine)
{}
glm::vec3 ColorMaterial::getDiffColor() {
	return diffCol;
}
void ColorMaterial::setDiffColor(glm::vec3 color) {
	this->diffCol = color;
}
void ColorMaterial::sendToShader(ShaderProgram *program)
{
	program->sendUniformVec3("MaterialEmit", emitCol);
	program->sendUniformVec3("MaterialAmbient", ambCol);
	program->sendUniformVec3("MaterialDiffuse", diffCol);
	program->sendUniformVec3("MaterialSpecular", specCol);
	program->sendUniformFloat("MaterialShininess", shininess);
}

void ColorMaterial::sendToShaderMultiplied(ShaderProgram *program, float factor)
{
	program->sendUniformVec3("MaterialEmit", emitCol * factor);
	program->sendUniformVec3("MaterialAmbient", ambCol * factor);
	program->sendUniformVec3("MaterialDiffuse", diffCol * factor);
	program->sendUniformVec3("MaterialSpecular", specCol * factor);
	program->sendUniformFloat("MaterialShininess", shininess);
}