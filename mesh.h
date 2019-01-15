#ifndef MESH_INCLUDED_H
#define MESH_INCLUDED_H

#include <../glm/glm.hpp>
#include <string>
#include <vector>
#include "obj_loader.h"

struct Vertex
{
public:
	Vertex(const glm::vec3& pos, const glm::vec2& texCoord, const glm::vec3& normal, const glm::vec3& color)
	{
		this->pos = pos;
		this->texCoord = texCoord;
		this->normal = normal;
		this->color = color;
	}

	glm::vec3* GetPos() { return &pos; }
	glm::vec2* GetTexCoord() { return &texCoord; }
	glm::vec3* GetNormal() { return &normal; }
	glm::vec3* GetColor() { return &color; }

	glm::vec3 pos;
	glm::vec2 texCoord;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec3 weight;
};


class Mesh
{
public:
	IndexedModel model;
	Mesh(int CylParts,int linkPosition);
    Mesh(const std::string& fileName);
	Mesh(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);

	void Draw(int mode);

	virtual ~Mesh();
protected:
private:
	enum MeshBufferPositions
	{
	POSITION_VB,
	TEXCOORD_VB,
	NORMAL_VB,
	INDEX_VB,
	COLOR_VB,
	weights_VB
	};

	static const unsigned int NUM_BUFFERS = 6;
	inline void operator=(const Mesh& mesh) {
		for (int i = 0; i < NUM_BUFFERS; i++) {
			m_vertexArrayBuffers[i] = mesh.m_vertexArrayBuffers[i];
		}
		model = mesh.model;
	}
	Mesh(const Mesh& mesh) {}

    void InitMesh(const IndexedModel& model);

	unsigned int m_vertexArrayObject;
	unsigned int m_vertexArrayBuffers[NUM_BUFFERS];
	unsigned int m_numIndices;
};

#endif
