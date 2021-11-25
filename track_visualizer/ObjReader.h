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

		Mesh* loadToMesh(string folderPath, string fileName);
		void readMtlFile(string folderPath, string fileName, Mesh* mesh);
		void loadTextures(string texturesLocationFolder, Mesh* targetMesh);

		glm::vec3 buildVec3(float x, float y, float z);
		glm::vec2 buildTextureCoord(float x, float y);
		glm::vec3 buildNormal(float x, float y, float z);


	private:
		Mesh* mesh;
};