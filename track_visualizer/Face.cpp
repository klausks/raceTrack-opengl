#include "Face.h"

Face::Face(vector<int> vertexIndices, vector<int> textureIndices, vector<int> normalIndices)
{
	this->vertexIndices = vertexIndices;
	this->textureIndices = textureIndices;
	this->normalIndices = normalIndices;
}
Face::Face(vector<int> vertexIndices, vector<int> textureIndices)
{
	this->vertexIndices = vertexIndices;
	this->textureIndices = textureIndices;
}