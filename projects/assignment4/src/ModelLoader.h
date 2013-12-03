#include "RenderModel.h"

class ModelLoader {
	struct Index {
		unsigned int v, vn, vt;
	};
	std::vector<Vertex> m_vertices, m_vertexData;
	std::vector<Normal> m_normals, m_normalData;
	std::vector<TexCoord> m_texCoords, m_texCoordData;
	std::vector<Index> m_indexes;
	
	RenderModel *m_loadedModel;
private:
	void parseString(const std::string &objString);
	void parseLine(const std::string &line);
	void prepareModelData();
	void createRenderModel();

	void parseVertex(std::istringstream &stream);
	void parseNormal(std::istringstream &stream);
	void parseTexCoord(std::istringstream &stream);
	void parseFace(std::istringstream &stream);
	void parseVertexIndexes(const std::string &vertexString);
public:
	RenderModel *loadModel(const std::string &pathString);
	void cleanup();
};