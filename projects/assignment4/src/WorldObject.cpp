#include "WorldObject.h"

WorldObject::WorldObject(RenderModel* mesh): mesh(mesh) { }
glm::quat WorldObject::getQuaternion() {
	return quaternion;
}
glm::vec3 WorldObject::getPosition() {
	return position;
}
glm::vec3 WorldObject::getScale() {
	return scale;
}
RenderModel* WorldObject::getMesh() {
	return mesh;
}
void WorldObject::setQuaternion(glm::quat quaternion) {
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

glm::mat4x4 WorldObject::getTransformationMatrix() {
	glm::mat4 rotation = glm::mat4_cast(quaternion);
	glm::mat4 translate = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), this->scale);
	return translate * rotation * scale;
}

void WorldObject::draw(ShaderProgram* shader) {
	shader->bind();
	glm::mat4 transform = getTransformationMatrix();
	shader->sendUniformMat4("ModelMatrix",transform);
	mesh->drawModel();
	shader->unbind();
}