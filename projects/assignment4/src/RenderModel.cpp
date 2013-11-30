
#include "RenderModel.h"
#include "uniformsAttribs.h"
#include <iostream>

#include <map>

std::map<std::string, size_t> VertAttrOffsets;

RenderModel::RenderModel(const std::vector<Vertex> &verts, const std::vector<Normal> &norm, const std::vector<TexCoord> &tex) :
m_vertices(verts), m_normals(norm), m_texCoords(tex),
m_vaoName(0), m_vertboName(0), m_normboName(0), m_texboName(0)
{}
void RenderModel::setupModel()
{
	//create vertexArray
	glGenVertexArrays(1, &m_vaoName);
	glBindVertexArray(m_vaoName);

	//create buffer (pos,col)
	GLuint vbos[3];
	glGenBuffers(3, vbos);
	m_vertboName = vbos[0]; m_normboName = vbos[1]; m_texboName = vbos[2];
	glBindBuffer(GL_ARRAY_BUFFER, m_vertboName);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*m_vertices.size(), m_vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, m_normboName);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Normal)*m_normals.size(), m_normals.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, m_texboName);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TexCoord)*m_texCoords.size(), m_texCoords.data(), GL_STATIC_DRAW);
	
	//set attrib pointers
	glBindBuffer(GL_ARRAY_BUFFER, m_vertboName);
	glEnableVertexAttribArray(VertexAttribs::Indexes::POSITION);
	glVertexAttribPointer(VertexAttribs::Indexes::POSITION, 3, GL_FLOAT, GL_FALSE, 
		sizeof(Vertex), 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_normboName);
	glEnableVertexAttribArray(VertexAttribs::Indexes::NORMAL);
	glVertexAttribPointer(VertexAttribs::Indexes::NORMAL, 3, GL_FLOAT, GL_FALSE,
		sizeof(Normal), 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_texboName);
	glEnableVertexAttribArray(VertexAttribs::Indexes::TEXCOORD);
	glVertexAttribPointer(VertexAttribs::Indexes::TEXCOORD, 2, GL_FLOAT, GL_FALSE,
		sizeof(TexCoord), 0);


	//unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(VertexAttribs::Indexes::POSITION);
	glDisableVertexAttribArray(VertexAttribs::Indexes::NORMAL);
	glDisableVertexAttribArray(VertexAttribs::Indexes::TEXCOORD);
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
}

void RenderModel::vaoName(GLuint vao)
{
	m_vaoName = vao;
}
GLuint RenderModel::vaoName()
{
	return m_vaoName;
}
size_t RenderModel::vertexNum()
{
	return m_vertices.size();
}