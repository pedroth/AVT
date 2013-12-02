#pragma once
#include<map>
#include"engine.h"


class ShaderManager {
private :
	typedef std::map<std::string, ShaderProgram *> shaderMapType;
	shaderMapType  shaders;
	static ShaderManager* shaderManager;
	ShaderManager();
public:
	static ShaderManager* getInstance();
	void add(std::string name, ShaderProgram* shader);
	ShaderProgram*  get(std::string name);
};