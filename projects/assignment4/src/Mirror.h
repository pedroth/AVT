#pragma once
#include "Symmetry.h"
#include "engine.h"

class Mirror : Symmetry {
public:
	Mirror(Map * original);
	glm::mat4 transform();
	void draw(ShaderProgram * shader) {
		Symmetry::draw(shader);
	}
};