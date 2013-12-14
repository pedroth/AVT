
#include "Mirror.h"

#include "utils.h"

Mirror3D::Mirror3D(std::string name)
: RealSymmetry(name), _planePoint(0.0f), _planeNormal(glm::vec3(0.0f,0.0f,1.0f))
{}
Mirror3D::Mirror3D(std::string name, glm::vec2 p1, glm::vec2 p2)
: Mirror3D(name)
{
	_planePoint = glm::vec3(p1, 0.0f);
	glm::vec3 p1Top2 = glm::vec3(p2 - p1, 0.0f);
	_planeNormal = glm::cross(p1Top2, glm::vec3(0.0f, 0.0f, 1.0f));
}
Mirror3D::Mirror3D(std::string name, glm::vec2 p1, float degrees)
: Mirror3D(name)
{
	_planePoint = glm::vec3(p1, 0.0f);
	
	const glm::vec3 yAxis(0.0f, 1.0f, 0.0f);
	const glm::vec3 zAxis(0.0f, 0.0f, 1.0f);
	glm::quat rotation = glm::rotate(glm::quat(), degrees, zAxis);
	_planeNormal = rotation * yAxis;
}
glm::mat4 Mirror3D::computeTransform()
{
	float x2 = _planeNormal.x * _planeNormal.x;
	float xy = _planeNormal.x * _planeNormal.y;
	float xz = _planeNormal.x * _planeNormal.z;
	float y2 = _planeNormal.y * _planeNormal.y;
	float yz = _planeNormal.y * _planeNormal.z;
	float z2 = _planeNormal.z * _planeNormal.z;
	glm::mat3 nSquared(x2, xy, xz, xy, y2, yz, xz, yz, z2);
	glm::mat3 I(1.0f);
	glm::mat4 reflection = glm::mat4(I - 2 * nSquared);

	glm::mat4 translate = glm::translate(glm::mat4(1.0f), _planePoint);
	glm::mat4 invTranslate = glm::translate(glm::mat4(1.0f), -_planePoint);

	glm::mat4 transform = invTranslate * reflection * translate;
	return transform;
}
std::vector<TransformedWO> Mirror3D::getGhostTransfWO()
{
	glm::mat4 transform = computeTransform();

	std::vector<TransformedWO> originals = getOriginalTransfWO();
	std::vector<TransformedWO> ghosts(originals);
	for (unsigned int i = 0; i < ghosts.size(); ++i){
		ghosts[i].transform = transform * ghosts[i].transform;
		ghosts[i].frontFaceCCW = !ghosts[i].frontFaceCCW;
	}
	return ghosts;
}