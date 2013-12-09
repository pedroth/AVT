#pragma once
#include"Mirror.h"


Mirror::Mirror(Map * original) : Symmetry(original) {}

glm::mat4 Mirror::transform() {
	glm::mat4 ret = glm::mat4(1.0f);
	ret[0][0] = -1.0f;
	return ret;
}