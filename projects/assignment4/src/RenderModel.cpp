
#include "RenderModel.h"
#include <iostream>

#include <map>

RenderModel::RenderModel(const std::string& name, const std::vector<Triangle> &triangles,
	const std::vector<glm::vec3> &verts, const std::vector<glm::vec3> &norm,
	const std::vector<glm::vec2> &tex) :
m_name(name), m_triangles(triangles), m_vertices(verts), 
m_normals(norm), m_texCoords(tex), m_tangents(),
m_vaoName(0), m_vertboName(0), m_normboName(0), m_texboName(0),
m_tanName(0)
{
	assert(verts.size() == norm.size() && verts.size() == tex.size());
	assert(checkTriangleIndexes(triangles,verts.size()));
	computeTangentSpaceBases();
	setupModel();
}
RenderModel::~RenderModel()
{
	cleanupModel();
}
void RenderModel::setupModel()
{
	std::vector<glm::vec3> verts = getSequencialVerts();
	std::vector<glm::vec2> texCoords = getSequencialTexCoords();
	std::vector<glm::vec3> normals = getSequencialNormals();
	std::vector<glm::vec3> tangents = getSequencialTangents();

	//create vertexArray
	glGenVertexArrays(1, &m_vaoName);
	glBindVertexArray(m_vaoName);

	//create buffer (pos,col)
	GLuint vbos[4];
	glGenBuffers(4, vbos);
	m_vertboName = vbos[0]; m_normboName = vbos[1]; m_texboName = vbos[2];
	m_tanName = vbos[3];
	glBindBuffer(GL_ARRAY_BUFFER, m_vertboName);
	glBufferData(GL_ARRAY_BUFFER, VEC3_SIZE*verts.size(), verts.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, m_normboName);
	glBufferData(GL_ARRAY_BUFFER, VEC3_SIZE*normals.size(), normals.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, m_texboName);
	glBufferData(GL_ARRAY_BUFFER, VEC2_SIZE*texCoords.size(), texCoords.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, m_tanName);
	glBufferData(GL_ARRAY_BUFFER, VEC3_SIZE*tangents.size(), tangents.data(), GL_STATIC_DRAW);
	
	//set attrib pointers
	glBindBuffer(GL_ARRAY_BUFFER, m_vertboName);
	glEnableVertexAttribArray(POSITION);
	glVertexAttribPointer(POSITION, 3, GL_FLOAT, GL_FALSE, 
		VEC3_SIZE, 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_normboName);
	glEnableVertexAttribArray(NORMAL);
	glVertexAttribPointer(NORMAL, 3, GL_FLOAT, GL_FALSE,
		VEC3_SIZE, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_texboName);
	glEnableVertexAttribArray(TEX);
	glVertexAttribPointer(TEX, 2, GL_FLOAT, GL_FALSE,
		VEC2_SIZE, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_tanName);
	glEnableVertexAttribArray(TANGENT);
	glVertexAttribPointer(TANGENT, 3, GL_FLOAT, GL_FALSE,
		VEC3_SIZE, 0);

	//unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(POSITION);
	glDisableVertexAttribArray(NORMAL);
	glDisableVertexAttribArray(TEX);
	glDisableVertexAttribArray(TANGENT);
}
void RenderModel::drawModel() const
{
	//prepare
	glBindVertexArray(m_vaoName);

	//draw
	glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());

	//unbind
	glBindVertexArray(0);
}
void RenderModel::cleanupModel()
{
	glDeleteVertexArrays(1, &m_vaoName);
	glDeleteBuffers(1, &m_vertboName);
	glDeleteBuffers(1, &m_normboName);
	glDeleteBuffers(1, &m_texboName);
	glDeleteBuffers(1, &m_tanName);
}
void RenderModel::computeTangentSpaceBases()
{

	m_tangents.clear();
	m_tangents.reserve(m_vertices.size());
	for (unsigned int i = 0; i < m_vertices.size(); ++i)
		m_tangents.push_back(glm::vec3(0.0f));
	//m_bitangents.clear();
	//m_bitangents.reserve(m_vertices.size());
	//for (unsigned int i = 0; i < m_vertices.size(); ++i)
	//	m_bitangents.push_back(glm::vec3(0.0f));

	for (unsigned int triI = 0; triI < m_triangles.size(); ++triI)
	{
		unsigned int v1Index = m_triangles[triI].vertIndex[0];
		unsigned int v2Index = m_triangles[triI].vertIndex[1];
		unsigned int v3Index = m_triangles[triI].vertIndex[2];

		glm::vec3 v1Pos = m_vertices[v1Index];
		glm::vec3 v2Pos = m_vertices[v2Index];
		glm::vec3 v3Pos = m_vertices[v3Index];
		glm::vec2 v1Tex = m_texCoords[v1Index];
		glm::vec2 v2Tex = m_texCoords[v2Index];
		glm::vec2 v3Tex = m_texCoords[v3Index];

		glm::vec3 Q1Pos = v2Pos - v1Pos;
		glm::vec3 Q2Pos = v3Pos - v1Pos;

		glm::vec2 Q1Tex = v2Tex - v1Tex;
		glm::vec2 Q2Tex = v3Tex - v1Tex;

		glm::mat2 TexMat;
		TexMat[0] = Q1Tex; TexMat[1] = Q2Tex;
		TexMat = glm::transpose(TexMat);

		glm::mat2x3 PosMatT;
		PosMatT[0] = Q1Pos; PosMatT[1] = Q2Pos;
		glm::mat3x2 PosMat = glm::transpose(PosMatT);

		glm::mat2 TexMatInv = glm::inverse(TexMat);

		glm::mat3x2 TBMatT = TexMatInv * PosMat;

		glm::mat2x3 TBMat = glm::transpose(TBMatT);

		glm::vec3 Tmodel = TBMat[0]; 
		//glm::vec3 Bmodel = TBMat[1];

		m_tangents[v1Index] += Tmodel;
		m_tangents[v2Index] += Tmodel;
		m_tangents[v3Index] += Tmodel;
		
		//m_bitangents[v1Index] += Bmodel;
		//m_bitangents[v2Index] += Bmodel;
		//m_bitangents[v3Index] += Bmodel;
	}

	for (unsigned int vertI = 0; vertI < m_vertices.size(); ++vertI)
	{
		glm::vec3 N = m_normals[vertI];
		glm::vec3 T = m_tangents[vertI];
		//glm::vec3 B = m_bitangents[vertI];

		//Gram-Schmidt orthogonalize
		glm::vec3 Tortho = T - glm::dot(N, T)*N;
		//glm::vec3 Bortho = B - glm::dot(N, B)*N - glm::dot(Tortho, B)*Tortho;

		m_tangents[vertI] = glm::normalize(Tortho);
		//m_bitangents[vertI] = glm::normalize(Bortho);
	}
}
unsigned int RenderModel::faceNum()
{
	return m_vertices.size() / 3;
}
GLuint RenderModel::vaoName()
{
	return m_vaoName;
}
const std::string& RenderModel::name() const
{
	return m_name;
}
size_t RenderModel::vertexNum()
{
	return m_vertices.size();
}
bool RenderModel::checkTriangleIndexes(const std::vector<Triangle> &triangles, unsigned int vertNum)
{
	bool failed = false;
	for (unsigned int i = 0; i < triangles.size(); ++i){
		if (triangles[i].vertIndex[0] >= vertNum ||
			triangles[i].vertIndex[1] >= vertNum ||
			triangles[i].vertIndex[2] >= vertNum)
			failed = true;
	}
	return !failed;
}
std::vector<glm::vec3> RenderModel::getSequencialVerts()
{
	std::vector<glm::vec3> verts;
	for (unsigned int i = 0; i < m_triangles.size(); ++i) {
		Triangle tri = m_triangles[i];
		verts.push_back(m_vertices[tri.vertIndex[0]]);
		verts.push_back(m_vertices[tri.vertIndex[1]]);
		verts.push_back(m_vertices[tri.vertIndex[2]]);
	}
	return verts;
}
std::vector<glm::vec2> RenderModel::getSequencialTexCoords()
{
	std::vector<glm::vec2> texCoords;
	for (unsigned int i = 0; i < m_triangles.size(); ++i) {
		Triangle tri = m_triangles[i];
		texCoords.push_back(m_texCoords[tri.vertIndex[0]]);
		texCoords.push_back(m_texCoords[tri.vertIndex[1]]);
		texCoords.push_back(m_texCoords[tri.vertIndex[2]]);
	}
	return texCoords;
}
std::vector<glm::vec3> RenderModel::getSequencialNormals()
{
	std::vector<glm::vec3> normals;
	for (unsigned int i = 0; i < m_triangles.size(); ++i) {
		Triangle tri = m_triangles[i];
		normals.push_back(m_normals[tri.vertIndex[0]]);
		normals.push_back(m_normals[tri.vertIndex[1]]);
		normals.push_back(m_normals[tri.vertIndex[2]]);
	}
	return normals;
}
std::vector<glm::vec3> RenderModel::getSequencialTangents()
{
	std::vector<glm::vec3> tangents;
	for (unsigned int i = 0; i < m_triangles.size(); ++i) {
		Triangle tri = m_triangles[i];
		tangents.push_back(m_tangents[tri.vertIndex[0]]);
		tangents.push_back(m_tangents[tri.vertIndex[1]]);
		tangents.push_back(m_tangents[tri.vertIndex[2]]);
	}
	return tangents;
}