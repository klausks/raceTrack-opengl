#pragma once
#include <vector>
using namespace std;

class Face
{
	public:
		Face() {};
		Face(vector<int> vertexIndices, vector<int> textureIndices, vector<int> normalIndices);
		Face(vector<int> vertexIndices, vector<int> textureIndices);
		vector<int> vertexIndices;
		vector<int> normalIndices;
		vector<int> textureIndices;

		int getNumOfVertices()
		{
			return this->vertexIndices.size();
		}

		int getNumOfTextureCoords()
		{
			return this->textureIndices.size();
		}

		int getNumOfNormals()
		{
			return this->normalIndices.size();
		}

		void addVertexIndex(int i)
		{
			this->vertexIndices.push_back(i);
		}

		void addNormalIndex(int i)
		{
			this->normalIndices.push_back(i);
		}

		void addTextureIndex(int i)
		{
			this->textureIndices.push_back(i);
		}

		Face* split()
		{
			Face* newFace = new Face();
			if (this->textureIndices.size() > 0)
			{
				newFace->addTextureIndex(this->textureIndices[0]);
				newFace->addTextureIndex(this->textureIndices[2]);
				newFace->addTextureIndex(this->textureIndices[3]);
				this->textureIndices.erase(textureIndices.begin() + 3);
			}
			if (this->vertexIndices.size() > 0)
			{
				newFace->addVertexIndex(this->vertexIndices[0]);
				newFace->addVertexIndex(this->vertexIndices[2]);
				newFace->addVertexIndex(this->vertexIndices[3]);
				this->vertexIndices.erase(vertexIndices.begin() + 3);
			}
			if (this->normalIndices.size() > 0)
			{
				newFace->addNormalIndex(this->normalIndices[0]);
				newFace->addNormalIndex(this->normalIndices[2]);
				newFace->addNormalIndex(this->normalIndices[3]);
				this->normalIndices.erase(normalIndices.begin() + 3);
			}
			return newFace;
		}
	private:
};