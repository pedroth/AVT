#pragma once

#include "engine.h"
#include "SymmetryExt.h"

class Mirror3D : public RealSymmetry {
	glm::vec3 _planePoint;
	glm::vec3 _planeNormal;
private:
	glm::mat4 computeTransform();
protected:
	std::vector<TransformedWO> getGhostTransfWO();
public:
	//Create a Mirror symmetry with the default reflection plane(z=0).
	Mirror3D(std::string name);
	//Create a Mirror symmetry with a plane orthogonal to z=0
	//that contains both p1 and p2.
	Mirror3D(std::string name, glm::vec2 p1, glm::vec2 p2);
	//Create a Mirror symmetry with a plane orthogonal to z=0
	//that contains p1 and that angles from x=0 degrees.
	Mirror3D(std::string name, glm::vec2 p1, float degrees);
};