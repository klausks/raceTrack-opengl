#pragma once

#include <vector>
#include "Face.h"
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include "Material.h"
#include "Texture.h"

using namespace std;

class MeshGroup
{
public:

	MeshGroup()
	{
		this->name = "";
		this->material = NULL;
		this->texture = NULL;
	}
	MeshGroup(string name)
	{
		this->name = name;
		this->material = NULL;
		this->texture = NULL;
	}
	MeshGroup(vector <Face*> faces);

	string name;
	GLuint vao = NULL;
	GLuint textureId = NULL;
	int shaderLocation = 0;
	vector<GLuint> vbos;
	vector<Face*> faces;
	Material* material;
	Texture* texture;

	int numOfVertices;
	void bindVAO();
	void bindTexture();
	void setTexture(string filename);

	void setName(string newName)
	{
		this->name = newName;
	}

	int getNumOfFaces()
	{
		return this->faces.size();
	}

	Face* getFaceAt(int index)
	{
		return this->faces.at(index);
	}

	string getMaterialId() {
		if (this->material != NULL) {
			return this->material->id;
		}
		else
		{
			return "";
		}
	}

	void addFace(Face* face)
	{
		faces.push_back(face);
	}

	void addVBO(GLuint vbo) {
		this->vbos.push_back(vbo);
	}

private:
};