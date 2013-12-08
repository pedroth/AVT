#include "WorldObject.h"

WorldObject::WorldObject(RenderModel* mesh) : mesh(mesh), quaternion(glm::quat(0.0f, 0.0f, 0.0f, 0.0f)), 
				position(glm::vec3(0.0f,0.0f,0.0f)), scale(glm::vec3(1.0f)), color(), symmetryAxis(0) { 
}

glm::quat WorldObject::getQuaternion() {
	return quaternion;
}
glm::vec3 WorldObject::getPosition() {
	return position;
}
glm::vec3 WorldObject::getScale() {
	return scale;
}
ColorMaterial WorldObject::getColor() {
	return color;
}
RenderModel* WorldObject::getMesh() {
	return mesh;
}
void WorldObject::setQuaternion(glm::quat quaternion) {
	quaternion = glm::normalize(quaternion);
	this->quaternion = quaternion;
}
void WorldObject::setPosition(glm::vec3 position) {
	this->position = position;
}
void WorldObject::setScale(glm::vec3 scale) {
	this->scale = scale;
}
void WorldObject::setMesh(RenderModel* mesh) {
	this->mesh = mesh;
}
void WorldObject::setColor(ColorMaterial color) {
	this->color = color;
}
void WorldObject::translate(glm::vec3 translate) {
	position = position + translate;
}
void WorldObject::rotate(glm::quat rotate) {
	rotate = glm::normalize(rotate);
	quaternion = rotate  * quaternion;
}
glm::mat4x4 WorldObject::getTransformationMatrix() {
	glm::mat4 rotation = glm::mat4_cast(quaternion);
	glm::mat4 translate = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), this->scale);
	return translate * rotation * scale;
}

glm::mat4x4 WorldObject::getXAxisTransformationMatrix() {
	glm::vec3 newPos, newScale;
	newPos[0] = -position[0];
	newPos[1] = position[1];
	newPos[2] = position[2];

	newScale[0] = -this->scale[0];
	newScale[1] = this->scale[1];
	newScale[2] = this->scale[2];

	glm::mat4 rotation = glm::mat4_cast(quaternion);
	glm::mat4 translate = glm::translate(glm::mat4(1.0f), newPos);
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), newScale);
	return translate * rotation * scale;
}

glm::mat4x4 WorldObject::getZAxisTransformationMatrix() {
	glm::vec3 newPos, newScale;
	newPos[0] = position[0];
	newPos[1] = -position[1];
	newPos[2] = position[2];

	newScale[0] = this->scale[0];
	newScale[1] = -this->scale[1];
	newScale[2] = this->scale[2];

	glm::mat4 rotation = glm::mat4_cast(quaternion);
	glm::mat4 translate = glm::translate(glm::mat4(1.0f), newPos);
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), newScale);
	return translate * rotation * scale;
}

glm::mat4x4 WorldObject::getXZAxisTransformationMatrix() {
	glm::vec3 newPos, newScale;
	newPos[0] = -position[0];
	newPos[1] = -position[1];
	newPos[2] = position[2];

	newScale[0] = -this->scale[0];
	newScale[1] = -this->scale[1];
	newScale[2] = this->scale[2];

	glm::mat4 rotation = glm::mat4_cast(quaternion);
	glm::mat4 translate = glm::translate(glm::mat4(1.0f), newPos);
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), newScale);
	return translate * rotation * scale;
}

void WorldObject::draw(ShaderProgram* shader) {
	shader->bind();
	glm::mat4 transform = getTransformationMatrix();
	shader->sendUniformMat4("ModelMatrix",transform);
	shader->sendUniformVec3("Color", this->color.getColor());
	mesh->drawModel();

	if (symmetryAxis >= 1){
		transform = getXAxisTransformationMatrix();
		shader->sendUniformMat4("ModelMatrix", transform);
		shader->sendUniformVec3("Color", this->color.getColor());
		mesh->drawModel();
	}
	if (symmetryAxis >= 2){
		transform = getZAxisTransformationMatrix();
		shader->sendUniformMat4("ModelMatrix", transform);
		shader->sendUniformVec3("Color", this->color.getColor());
		mesh->drawModel();

		transform = getXZAxisTransformationMatrix();
		shader->sendUniformMat4("ModelMatrix", transform);
		shader->sendUniformVec3("Color", this->color.getColor());
		mesh->drawModel();
	}
	shader->unbind();
}

void WorldObject::setSymmetryAxis(int axis){
	symmetryAxis = axis;
}