#include "ShaderManager.h"

ShaderManager* ShaderManager::shaderManager = nullptr;
ShaderManager::ShaderManager() : shaders() { }
ShaderManager* ShaderManager::getInstance() {
	if (shaderManager == nullptr)
		shaderManager = new ShaderManager();
	return shaderManager;
}
void ShaderManager::add(std::string name, ShaderProgram* shader) {
	shaders[name] =  shader;
}
ShaderProgram*  ShaderManager::get(std::string name) {
	shaderMapType::iterator it = shaders.find(name);
	if (it == shaders.end())
		return nullptr;
	else
		return it->second;
}