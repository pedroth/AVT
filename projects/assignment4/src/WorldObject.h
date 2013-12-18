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
	std::string name;
	int symmetryAxis;
	ShaderProgram *  myShader;

public:
	WorldObject(std::string name, RenderModel* mesh);
	glm::quat getQuaternion();
	glm::vec3 getPosition();
	glm::vec3 getScale();
	ColorMaterial getColor();
	RenderModel* getMesh();
	std::string getName();
	ShaderProgram* getMyShader();
	void setMyShader(ShaderProgram* shader);
	void setQuaternion(glm::quat quaternion);
	void setPosition(glm::vec3 position);
	void setScale(glm::vec3 scale);
	void setMesh(RenderModel* mesh);
	void setColor(ColorMaterial color);
	void translate(glm::vec3 translate);
	void rotate(glm::quat rotate);
	glm::mat4x4 getTransformationMatrix();
	void draw(ShaderProgram* shader);
	void setSymmetryAxis(int axis);
	bool hasMyShader();
};

#endif
