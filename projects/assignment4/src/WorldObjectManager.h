#ifndef _WORLDMANAGER_H
#define _WORLDMANAGER_H

#include "engine.h"
#include "WorldObject.h"
#include<iostream>

class WorldObjectManager {
private:
	std::vector<WorldObject*> list;
public:
	WorldObjectManager();
	void add(WorldObject* object);
	WorldObject* getObject(int index);
	void draw(ShaderProgram* shader);
};

#endif // !_WORLDMANAGER_H
