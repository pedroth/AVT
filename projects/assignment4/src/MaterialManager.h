#pragma once

#include <string>
#include <map>
#include <vector>
#include "ColorMaterial.h"

class MaterialManager 
{
	static MaterialManager *_instance;

	typedef std::map<std::string, ColorMaterial*> materialMapType;
	materialMapType _materials;

	MaterialManager();
public:
	static MaterialManager *instance();
	ColorMaterial *get(std::string matName);
	void put(std::string matName, ColorMaterial *mat);
	void loadFileList(std::string folderPath, std::vector<std::string> fileNameList);
private:
	void loadMaterialFile(std::string folderPath, std::string fileName);
	struct MaterialLoader {
		glm::vec3 emit, amb, diff, spec;
		float shine;
		void parseMaterialString(std::string matStr);
		void parseMaterialLine(std::string line);
		glm::vec3 parseVec3(std::istringstream &stream);
		ColorMaterial *createMaterial();
	};
};