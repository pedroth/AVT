#ifndef _WORLDOBJECT_H_
#define _WORLDOBJECT_H_

#include "engine.h"

class WorldObject  {
private:
	glm::quat quaternion;
	glm::vec3 position;
	glm::vec3 scale;
	RenderModel* mesh;

public:
	WorldObject(RenderModel* mesh);
	glm::quat getQuaternion();
	glm::vec3 getPosition();
	glm::vec3 getScale();
	RenderModel* getMesh();
	void setQuaternion(glm::quat quaternion);
	void setPosition(glm::vec3 position);
	void setScale(glm::vec3 scale);
	void setMesh(RenderModel* mesh);
	glm::mat4x4 getTransformationMatrix();
	void draw(ShaderProgram* shader);
};

#endif
