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
	int i = 0;
	for (objList_type::iterator it = list.begin(); it != list.end(); ++it) {
		WOInfo info = it->second;
		if (info.shader) {
			glStencilFunc(GL_ALWAYS, i + 1, -1);
			info.object->draw(info.shader);
		}
		else {
			glStencilFunc(GL_ALWAYS, i + 1, -1);
			info.object->draw(shader);
		}
		i++;
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