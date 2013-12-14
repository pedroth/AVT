#include "WorldObjectManager.h"


WorldObjectManager::WorldObjectManager(){}

void WorldObjectManager::add(WorldObject *object) {
	list[object->getName()] = WOInfo(object);
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
	glStencilFunc(GL_ALWAYS, 0, -1);
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

void WorldObjectManager::save(int axis) {
	glm::vec3 pos;
	glm::quat quat;

	std::ofstream file("../resources/worldFile.txt", std::ios::trunc);
	if (file.is_open()){
		file << axis << std::endl;
		
		for (objList_type::iterator it = list.begin(); it != list.end(); ++it) {
			pos = it->second.object->getPosition();
			file << pos[0] << " " << pos[1] << " " << pos[2] << " ";
			quat = it->second.object->getQuaternion();
			file << quat.w << " " << quat.x << " " << quat.y << " " << quat.z << std::endl;
		}

		file.close();
		std::cout << "World Saved" << std::endl;
	}
	else{
		std::cerr << "WorldObjectManager::save: Error in file open" << std::endl;
	}
}

void WorldObjectManager::load(int *axis) {
	glm::vec3 pos;
	glm::quat quat;
	std::string line;
	std::ifstream file("../resources/worldFile.txt");
	if (file.is_open()){
		getline(file, line);
		std::istringstream stream(line);
		stream >> *axis;

		for (objList_type::iterator it = list.begin(); it != list.end(); ++it) {
			getline(file, line);
			std::istringstream stream(line);
			
			stream >> pos[0] >> pos[1] >> pos[2] >> quat.w >> quat.x >> quat.y >> quat.z;

			it->second.object->setPosition(pos);
			it->second.object->setQuaternion(quat);
			it->second.object->setSymmetryAxis(*axis);
		}
		file.close();

		std::cout << "World Loaded" << std::endl;
	}

	else{
		std::cerr << "WorldObjectManager::load: Error in file open" << std::endl;
	}
}