//model_setup.cpp

#include <GL/glew.h>
#include <vector>
#include <sstream>

#include "utils.h"

#include "ModelLoader.h"

RenderModel *ModelLoader::loadModel(const std::string& pathString)
{
	std::string objString = readFromFile(pathString);
	parseString(objString);
	prepareModelData();
	createRenderModel();
	cleanup();
	return m_loadedModel;
}

void ModelLoader::parseString(const std::string &objString)
{
	//create string input stream
	std::istringstream objStream(objString);
	//for each line
	//get line
	std::string line;
	while (std::getline(objStream, line))
	{
		//parse line
		parseLine(line);
	}
}

void ModelLoader::parseLine(const std::string &line) {
	std::istringstream lineStream(line);
	std::string lineType;
	lineStream >> lineType;

	if (lineType.compare("v") == 0) parseVertex(lineStream);
	if (lineType.compare("vn") == 0) parseNormal(lineStream);
	if (lineType.compare("vt") == 0) parseTexCoord(lineStream);
	if (lineType.compare("f") == 0) parseFace(lineStream);
}

void ModelLoader::parseVertex(std::istringstream &stream)
{
	Vertex v;
	stream >> v.x >> v.y >> v.z;
	m_vertices.push_back(v);
}

void ModelLoader::parseNormal(std::istringstream &stream)
{
	Normal n;
	stream >> n.x >> n.y >> n.z;
	m_normals.push_back(n);
}

void ModelLoader::parseTexCoord(std::istringstream &stream)
{
	TexCoord vt;
	stream >> vt.s >> vt.t;
	m_texCoords.push_back(vt);
}

void ModelLoader::parseFace(std::istringstream &stream)
{
	std::string vertexString;
	stream >> vertexString;
	parseVertexIndexes(vertexString);
	stream >> vertexString;
	parseVertexIndexes(vertexString);
	stream >> vertexString;
	parseVertexIndexes(vertexString);
}

void ModelLoader::parseVertexIndexes(const std::string &vertexString)
{
	std::istringstream vertexStream(vertexString);
	std::string vInd, tInd, nInd;

	std::getline(vertexStream, vInd, '/');
	std::getline(vertexStream, tInd, '/');
	std::getline(vertexStream, nInd, '/');

	unsigned int verInd, texInd, norInd;
	verInd = std::stoul(vInd);
	texInd = std::stoul(tInd);
	norInd = std::stoul(nInd);

	Index ind;
	ind.v = verInd;
	ind.vn = norInd;
	ind.vt = texInd;

	m_indexes.push_back(ind);
}

void ModelLoader::prepareModelData()
{
	for (Index &ind : m_indexes) {
		m_vertexData.push_back(m_vertices[ind.v - 1]);
		m_normalData.push_back(m_normals[ind.vn - 1]);
		m_texCoordData.push_back(m_texCoords[ind.vt - 1]);
	}
}

void ModelLoader::createRenderModel()
{
	m_loadedModel = new RenderModel(m_vertexData, m_normalData, m_texCoordData);
}

void ModelLoader::cleanup()
{
	m_vertices.clear();
	m_vertexData.clear();
	m_normals.clear();
	m_normalData.clear();
	m_texCoords.clear();
	m_texCoordData.clear();
	m_indexes.clear();
}