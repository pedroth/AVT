#include "ColorMaterial.h"

ColorMaterial::ColorMaterial() : color(glm::vec3(0.5f)) { }
glm::vec3 ColorMaterial::getColor() {
	return color;
}
void ColorMaterial::setColor(glm::vec3 color) {
	this->color = color;
}