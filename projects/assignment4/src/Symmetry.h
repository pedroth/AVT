#pragma once
#include<iostream>
#include"engine.h"
#include "WorldObject.h"
typedef std::map<std::string, WorldObject*> Map;

class Symmetry {
private:
	Map * originalObjects;
public:
	Symmetry(Map * original);
	void draw(ShaderProgram * shader);
	virtual glm::mat4 transform() = 0;
	
};