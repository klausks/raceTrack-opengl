#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include "Mesh.h"
#include "Obj3D.h"
#include "Material.h"
#include <map>


using namespace std;

class ObjReader
{
	public:
		ObjReader() {}

		Mesh* build();
		Mesh* read(string folderPath, string fileName);

		glm::vec3 buildVec3(float x, float y, float z);
		glm::vec2 buildTextureCoord(float x, float y);
		glm::vec3 buildNormal(float x, float y, float z);
		void readMtlFile(string folderPath, string fileName, Mesh* mesh);

	private:
		Mesh* mesh;
};