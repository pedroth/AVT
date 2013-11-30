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

}