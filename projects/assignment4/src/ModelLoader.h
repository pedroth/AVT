#ifndef _RENDERMODEL_H_
#define _RENDERMODEL_H_

#include "RenderModel.h"

class ModelLoader {
	struct Index {
		unsigned int v, vn, vt;
	};
	std::vector<glm::vec3> m_vertices, m_vertexData;
	std::vector<glm::vec3> m_normals, m_normalData;
	std::vector<glm::vec2> m_texCoords, m_texCoordData;
	std::vector<Index> m_indexes;

	std::vector<RenderModel::Triangle> m_triangles;
	
	RenderModel *m_loadedModel;
private:
	void parseString(const std::string &objString);
	void parseLine(const std::string &line);
	void prepareModelData();
	void createRenderModel(std::string modelName);

	void parseVertex(std::istringstream &stream);
	void parseNormal(std::istringstream &stream);
	void parseTexCoord(std::istringstream &stream);
	void parseFace(std::istringstream &stream);
	void parseVertexIndexes(const std::string &vertexString);
public:
	RenderModel *loadModel(std::string modelName, const std::string &pathString);
	void cleanup();
};

#endif