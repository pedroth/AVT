#ifndef _WORLDOBJECT_H_
#define _WORLDOBJECT_H_

#include "engine.h"
#include "ColorMaterial.h"

class WorldObject  {
private:
	glm::quat quaternion;
	glm::vec3 position;
	glm::vec3 scale;
	ColorMaterial color;
	RenderModel* mesh;
	int symmetryAxis;

public:
	WorldObject(RenderModel* mesh);
	glm::quat getQuaternion();
	glm::vec3 getPosition();
	glm::vec3 getScale();
	ColorMaterial getColor();
	RenderModel* getMesh();
	void setQuaternion(glm::quat quaternion);
	void setPosition(glm::vec3 position);
	void setScale(glm::vec3 scale);
	void setMesh(RenderModel* mesh);
	void setColor(ColorMaterial color);
	void translate(glm::vec3 translate);
	void rotate(glm::quat rotate);
	glm::mat4x4 getTransformationMatrix();
	glm::mat4x4 getXAxisTransformationMatrix();
	glm::mat4x4 getZAxisTransformationMatrix();
	glm::mat4x4 getXZAxisTransformationMatrix();
	void draw(ShaderProgram* shader);
	void setSymmetryAxis(int axis);
};

#endif
