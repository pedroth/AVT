#ifndef __RENDERMODEL_H__
#define __RENDERMODEL_H__

#include <GL/glew.h>
#include <glm.hpp>
#include <vector>

//Class representing a triangle mesh.
//Triangles are composed by three consecutive vertices.
class RenderModel {
public:
	struct Triangle {
		unsigned int vertIndex[3];
	};
private:
	std::string m_name;
	
	std::vector<Triangle> m_triangles;

	std::vector<glm::vec3> m_vertices;
	std::vector<glm::vec3> m_normals;
	std::vector<glm::vec2> m_texCoords;

	std::vector<glm::vec3> m_tangents;

	GLuint m_vaoName, m_vertboName, m_normboName, m_texboName, m_tanName;
public:
	//Create a RenderModel with a name and its vertex data.
	//Also loads the object into OpenGL/GPU.
	RenderModel(const std::string& name, const std::vector<Triangle> &triangles, 
		const std::vector<glm::vec3> &verts, const std::vector<glm::vec3> &norm, 
		const std::vector<glm::vec2> &tex);
	//Destroy a RenderModel.
	//Clear the object from openGL/GPU.
	~RenderModel();

	//OpenGL setup of the model.
	void setupModel();
	//OpenGL cleanup.
	void cleanupModel();

	void computeTangentSpaceBases();
	
	void drawModel() const;
	
	size_t vertexNum();
	unsigned int faceNum();
	GLuint vaoName();
	const std::string& name() const;
public:
	static const GLuint POSITION=0;
	static const GLuint NORMAL=1;
	static const GLuint TEX=2;
	static const GLuint TANGENT=3;

private:
	std::vector<glm::vec3> getSequencialVerts();
	std::vector<glm::vec2> getSequencialTexCoords();
	std::vector<glm::vec3> getSequencialNormals();
	std::vector<glm::vec3> getSequencialTangents();

private:
	static const size_t VEC3_SIZE = 3 * sizeof(float);
	static const size_t VEC2_SIZE = 2 * sizeof(float);

private:
	bool checkTriangleIndexes(const std::vector<Triangle> &triangles, unsigned int vertNum);
};

#endif