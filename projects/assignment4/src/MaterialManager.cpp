#include "MaterialManager.h"
#include "utils.h"
#include <sstream>
#include <iostream>

MaterialManager *MaterialManager::_instance=nullptr;
MaterialManager *MaterialManager::instance()
{
	if (_instance == nullptr)
		_instance = new MaterialManager();
	return _instance;
}
MaterialManager::MaterialManager()
: _materials()
{}
ColorMaterial *MaterialManager::get(std::string matName)
{
	materialMapType::iterator it = _materials.find(matName);
	if (it == _materials.end())
		return nullptr;
	return it->second;
}
void MaterialManager::put(std::string matName, ColorMaterial *mat)
{
	assert(_materials.find(matName) == _materials.end());
	_materials[matName] = mat;
}
void MaterialManager::loadFileList(std::string folderPath, std::vector<std::string> fileNameList)
{
	for (std::vector<std::string>::iterator it = fileNameList.begin();
		it != fileNameList.end();
		++it)
	{
		std::cout << "Loading material \"" << *it << "\"" << std::endl;
		loadMaterialFile(folderPath, *it);
	}
}
void MaterialManager::loadMaterialFile(std::string folderPath, std::string fileName)
{
	std::string fileContent = readFromFile(folderPath + fileName);
	MaterialLoader loader;
	loader.parseMaterialString(fileContent);
	ColorMaterial *mat = loader.createMaterial();
	put(fileName, mat);
}
void MaterialManager::MaterialLoader::parseMaterialString(std::string matStr)
{
	std::istringstream lineSource(matStr);
	std::string currLine;
	while (getline(lineSource, currLine))
	{
		parseMaterialLine(currLine);
	}
}
void MaterialManager::MaterialLoader::parseMaterialLine(std::string line)
{
	std::istringstream lineStream(line);
	std::string lineType;
	lineStream >> lineType;

	if (lineType.compare("emitCol:") == 0) emit = parseVec3(lineStream);
	if (lineType.compare("ambientCol:") == 0) amb = parseVec3(lineStream);
	if (lineType.compare("diffCol:") == 0) diff = parseVec3(lineStream);
	if (lineType.compare("specCol:") == 0) spec = parseVec3(lineStream);
	if (lineType.compare("shininess:") == 0) lineStream >> shine;
}
glm::vec3 MaterialManager::MaterialLoader::parseVec3(std::istringstream &stream)
{
	glm::vec3 res;
	stream >> res.r >> res.g >> res.b;
	return res;
}
ColorMaterial *MaterialManager::MaterialLoader::createMaterial()
{
	return new ColorMaterial(emit, amb, diff, spec, shine);
}