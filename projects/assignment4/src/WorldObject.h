#ifndef _WORLDOBJECT_H_
#define _WORLDOBJECT_H_

#include "engine.h"

class WorldObject  {
private:
	glm::quat quaternion;
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 color;
	RenderModel* mesh;

public:
	WorldObject(RenderModel* mesh);
	glm::quat getQuaternion();
	glm::vec3 getPosition();
	glm::vec3 getScale();
	glm::vec3 getColor();
	RenderModel* getMesh();
	void setQuaternion(glm::quat quaternion);
	void setPosition(glm::vec3 position);
	void setScale(glm::vec3 scale);
	void setMesh(RenderModel* mesh);
	void setColor(glm::vec3 color);
	glm::mat4x4 getTransformationMatrix();
	void draw(ShaderProgram* shader);
};

#endif
