#include "WorldObjectManager.h"


WorldObjectManager::WorldObjectManager(){}

void WorldObjectManager::add(std::string name, WorldObject *object) {
	list[name] = WOInfo(object);
}
WorldObject* WorldObjectManager::getObject(std::string name) {
	assert(list.find(name) != list.end());
	return (list.find(name))->second.object;
}
void WorldObjectManager::draw(ShaderProgram* shader) {
	for (objList_type::iterator it = list.begin(); it != list.end(); ++it) {
		WOInfo info = it->second;
		if (info.shader) {
			info.object->draw(info.shader);
		}
		else {
			info.object->draw(shader);
		}
	}
}

void WorldObjectManager::setObjectShader(std::string name, ShaderProgram *shader)
{
	assert(list.find(name) != list.end());
	list[name].shader = shader;
}

WorldObjectManager::WOInfo::WOInfo(WorldObject *obj)
: object(obj), shader(0)
{}
WorldObjectManager::WOInfo::WOInfo()
: object(), shader(0)
{}

void WorldObjectManager::setSymmetryAxis(int axis) {
	for (objList_type::iterator it = list.begin(); it != list.end(); ++it) {
		it->second.object->setSymmetryAxis(axis);
	}
}