#ifndef __RENDERMODEL_H__
#define __RENDERMODEL_H__

#include <GL/glew.h>
#include <vector>

struct Vertex {
	float x, y, z;
};
struct Normal {
	float x, y, z;
};
struct TexCoord {
	float s, t;
};

class RenderModel {
	std::string m_name;
	GLuint m_vaoName, m_vertboName, m_normboName, m_texboName;
	std::vector<Vertex> m_vertices;
	std::vector<Normal> m_normals;
	std::vector<TexCoord> m_texCoords;
public:
	RenderModel(const std::string& name, const std::vector<Vertex> &verts, 
		const std::vector<Normal> &norm, const std::vector<TexCoord> &tex);
	~RenderModel();
	void setupModel();
	void cleanupModel();
	void drawModel() const;
	size_t vertexNum();
	void vaoName(GLuint vao);
	GLuint vaoName();
	const std::string& name() const;
public:
	static const GLuint POSITION=0;
	static const GLuint NORMAL=1;
	static const GLuint TEX=2;
};

#endif