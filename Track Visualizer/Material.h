#pragma once
#include <glm\glm.hpp>
#include <string>
using namespace std;

class Material {
public:
	Material(string id);
	string id;
	glm::vec3 ka, kd, ks;
	float ns;
	string mapKd;
	unsigned int TID;
};