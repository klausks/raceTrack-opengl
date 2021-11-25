#pragma once

#include <vector>
#include "MeshGroup.h"
#include <glm\glm.hpp>

using namespace std;

class Mesh
{
public:
	Mesh()
	{
		this->minX = 99999.0;
		this->minY = 99999.0;
		this->minZ = 99999.0;
		this->maxX = -99999.0;
		this->maxY = -99999.0;
		this->maxZ = -99999.0;
	};
	Mesh(vector <MeshGroup*> meshGroups);

	void setVertices(vector<glm::vec3> vertices);

	int getNumOfGroups()
	{
		return this->groups.size();
	}

	MeshGroup* getGroupAt(int index)
	{
		return this->groups.at(index);
	}

	void addGroup(MeshGroup* group)
	{
		this->groups.push_back(group);
	}

	void addVertex(glm::vec3 vertex)
	{
		this->vertices.push_back(vertex);
	}

	void addTextureCoord(glm::vec2 textureCoord)
	{
		this->textureCoords.push_back(textureCoord);
	}

	void addNormal(glm::vec3 normal)
	{
		this->normals.push_back(normal);
	}


	vector<MeshGroup*> groups;
	vector<glm::vec3> vertices;
	vector<glm::vec2> textureCoords;
	vector<glm::vec3> normals;
	float minX, minY, minZ, maxX, maxY, maxZ;
	string name;


private:
};