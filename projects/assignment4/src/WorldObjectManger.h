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