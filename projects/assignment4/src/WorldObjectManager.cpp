#include "WorldObjectManger.h"


WorldObjectManager::WorldObjectManager() : list() {}
void WorldObjectManager::add(WorldObject* object) {
	list.push_back(object);
}
WorldObject* WorldObjectManager::getObject(int index) {
	return list.at(index);
}
void WorldObjectManager::draw(ShaderProgram* shader) {
	for (std::vector<WorldObject*>::iterator it = list.begin(); it != list.end(); ++it) {
		(*it)->draw(shader);
	}
}
