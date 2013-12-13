#pragma once

#include "../SymmetryExt.h"

void testSymmetrys();

class TestChildSymm : public RealSymmetry {
public:
	TestChildSymm(std::string name);
	void draw(ShaderProgram *program);
};