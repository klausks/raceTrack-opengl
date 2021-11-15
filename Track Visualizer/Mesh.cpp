#include "Mesh.h"

Mesh::Mesh(vector <MeshGroup*> meshGroups)
{
	this->groups = meshGroups;
}

void Mesh::setVertices(vector<glm::vec3> vertices) {
	this->vertices = vertices;
}
