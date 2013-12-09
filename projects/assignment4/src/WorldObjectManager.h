#ifndef _WORLDMANAGER_H
#define _WORLDMANAGER_H

#include "engine.h"
#include "WorldObject.h"
#include<iostream>
#include <map>

class WorldObjectManager {
private:
	struct WOInfo {
		WorldObject *object;
		ShaderProgram *shader;
		WOInfo();
		WOInfo(WorldObject *obj);
	};
	typedef std::map<std::string, WOInfo> objList_type;
	objList_type list;
public:
	WorldObjectManager();
	void add(std::string name, WorldObject* object);
	WorldObject* getObject(std::string name);
	void setObjectShader(std::string name, ShaderProgram *shader);
	void draw(ShaderProgram* shader);
	void setSymmetryAxis(int axis);
};

#endif // !_WORLDMANAGER_H
