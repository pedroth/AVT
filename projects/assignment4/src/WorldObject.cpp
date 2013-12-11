#include "WorldObject.h"

WorldObject::WorldObject(std::string name, RenderModel* mesh) 
: name(name), mesh(mesh), quaternion(glm::quat(0.0f, 0.0f, 0.0f, 0.0f)), 
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
std::string WorldObject::getName() {
	return name;
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
	glm::vec3 pos = position + translate;

	if (pos[0] > 0){
		pos[0] = 0;
	}
	if (pos[0] < -8){
		pos[0] = -8;
	}
	if (pos[1] < 0){
		pos[1] = 0;
	}
	if (pos[1] > 8){
		pos[1] = 8;
	}
	position = pos;
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

void WorldObject::draw(ShaderProgram* shader) {
	shader->bind();
	ModelMatrixStack.push();
	glm::mat4 transform = getTransformationMatrix();
	glm::mat4 modelStackTop = ModelMatrixStack.current();
	shader->sendUniformMat4("ModelMatrix",modelStackTop * transform);
	shader->sendUniformVec3("Color", this->color.getDiffColor());
	mesh->drawModel();


	glm::vec3 newColor = this->color.getDiffColor();
	newColor[0] -= 0.1f;
	newColor[1] -= 0.1f;
	newColor[2] -= 0.1f;

	if (symmetryAxis >= 1){
		glFrontFace(GL_CW);
		glm::mat4 symmetryTransform = glm::scale(glm::mat4(), glm::vec3(-1, 1, 1));
		shader->sendUniformMat4("ModelMatrix", modelStackTop * symmetryTransform * transform);
		shader->sendUniformVec3("Color", newColor);
		mesh->drawModel();
		glFrontFace(GL_CCW);
	}
	if (symmetryAxis >= 2){
		{
			glFrontFace(GL_CW);
			glm::mat4 symmetryTransform = glm::scale(glm::mat4(), glm::vec3(1, -1, 1));
			shader->sendUniformMat4("ModelMatrix", modelStackTop * symmetryTransform * transform);
			shader->sendUniformVec3("Color", newColor);
			mesh->drawModel();
			glFrontFace(GL_CCW);
		}
		{
			glm::mat4 symmetryTransform = glm::scale(glm::mat4(), glm::vec3(-1, -1, 1));
			shader->sendUniformMat4("ModelMatrix", modelStackTop * symmetryTransform * transform);
			shader->sendUniformVec3("Color", newColor);
			mesh->drawModel();
		}
	}
	ModelMatrixStack.pop();

	shader->unbind();
}

void WorldObject::setSymmetryAxis(int axis){
	symmetryAxis = axis;
}