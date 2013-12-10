#pragma once

#include <glm.hpp>
#include "ShaderProgram.h"

class ColorMaterial {
private:
	glm::vec3 emitCol;
	glm::vec3 ambCol;
	glm::vec3 diffCol;
	glm::vec3 specCol;
	float shininess;
public:
	ColorMaterial();
	ColorMaterial(glm::vec3 diffColor);
	ColorMaterial(glm::vec3 emit, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shine);
	glm::vec3 getDiffColor();
	void setDiffColor(glm::vec3 color);
	//void sendToShader(ShaderProgram *program);
	//void sendToShaderMultiplied(ShaderProgram *program, float factor);
};