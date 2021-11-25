#include "ObjReader.h"


Mesh* ObjReader::loadToMesh(string folderPath, string fileName) {
	Mesh* mesh = new Mesh();
	MeshGroup* currentGroup = nullptr;
	string mtlFileName;
	ifstream arq(folderPath + fileName);
	while (!arq.eof()) {
		string line;
		getline(arq, line);
		stringstream lineStream;
		lineStream << line;
		string temp;
		lineStream >> temp;
		if (temp == "v") {
			// ler vértice ...
			float x, y, z;
			lineStream >> x >> y >> z;
			if (x > mesh->maxX) { mesh->maxX = x; }
			if (y > mesh->maxY) { mesh->maxY = y; }
			if (z > mesh->maxZ) { mesh->maxZ = z; }
			if (x < mesh->minX) { mesh->minX = x; }
			if (y < mesh->minY) { mesh->minY = y; }
			if (z < mesh->minZ) { mesh->minZ = z; }

			glm::vec3 vertex = buildVec3(x, y, z);
			mesh->addVertex(vertex);
		}
		else if (temp == "vt") {
			float x, y;
			lineStream >> x >> y;
			glm::vec2 textureCoord = buildTextureCoord(x, y);
			mesh->addTextureCoord(textureCoord);
		}
		else if (temp == "vn") {
			float x, y, z;
			lineStream >> x >> y >> z;
			glm::vec3 normal = buildVec3(x, y, z);
			mesh->addNormal(normal);
		}
		else if (temp == "f") {
			Face* face = new Face();
			string token;
			string vertice, texture, normal;
			int nOfVertices = 0;
			while (lineStream >> token) {
				stringstream stoken;
				stoken << token;
				getline(stoken, vertice, '/');
				getline(stoken, texture, '/');
				getline(stoken, normal, '/');

				if (vertice != "") {
					face->addVertexIndex(stoi(vertice) - 1);
				}
				if (texture != "") {
					face->addTextureIndex(stoi(texture) - 1);
				}
				if (normal != "") {
					face->addNormalIndex(stoi(normal) - 1);
				}
				nOfVertices++;
			}
			if (nOfVertices == 4) {
				Face* additionalFace = face->split();
				currentGroup->addFace(additionalFace);
			}
			if (currentGroup != nullptr) { currentGroup->addFace(face); }
		}
		else if (temp == "g") {
			string groupName;
			lineStream >> groupName;
			currentGroup = new MeshGroup(groupName);
			mesh->addGroup(currentGroup);
		}
		else if (temp == "usemtl") {
			string mtlId;
			lineStream >> mtlId;
			currentGroup->material = new Material(mtlId);
		}
		else if (temp == "mtllib") {

			lineStream >> mtlFileName;
		}
		else {
			continue;
		}
	}
	arq.close();
	readMtlFile(folderPath, mtlFileName, mesh);
	loadTextures(folderPath, mesh);

	return mesh;
}

glm::vec3 ObjReader::buildVec3(float x, float y, float z)
{
	return glm::vec3(x, y, z);
}

glm::vec2 ObjReader::buildTextureCoord(float x, float y)
{
	return glm::vec2(x, y);
}

glm::vec3 ObjReader::buildNormal(float x, float y, float z)
{
	return glm::vec3(x, y, z);
}

void ObjReader::readMtlFile(string folderPath, string fileName, Mesh* mesh)
{
	std::map<string, Material*> groupMaterials;
	for (MeshGroup* group : mesh->groups) {
		string groupMaterialId = group->getMaterialId();
		if (groupMaterialId != "")
		{
			groupMaterials[groupMaterialId] = group->material;
		}
	}
	Material* currentMaterial = NULL;
	ifstream arq(folderPath + fileName);
	while (!arq.eof()) {
		string line;
		getline(arq, line);
		stringstream lineStream;
		lineStream << line;
		string temp;
		lineStream >> temp;
		if (temp == "newmtl") {
			string materialId;
			lineStream >> materialId;
			currentMaterial = groupMaterials[materialId];
		}
		else if (currentMaterial != NULL) {
			if (temp == "Ns") {
				float ns;
				lineStream >> ns;
				currentMaterial->ns = ns;
			}
			else if (temp == "Ka") {
				float x, y, z;
				lineStream >> x >> y >> z;
				glm::vec3 ka = buildVec3(x, y, z);
				currentMaterial->ka = ka;
			}
			else if (temp == "Kd") {
				float x, y, z;
				lineStream >> x >> y >> z;
				glm::vec3 kd = buildVec3(x, y, z);
				currentMaterial->kd = kd;
			}
			else if (temp == "Ks") {
				float x, y, z;
				lineStream >> x >> y >> z;
				glm::vec3 ks = buildVec3(x, y, z);
				currentMaterial->ks = ks;
			}
			else if (temp == "map_Kd") {
				string mapKd;
				lineStream >> mapKd;
				currentMaterial->mapKd = mapKd;
			}
		}
	}
}

void ObjReader::loadTextures(string texturesLocationFolder, Mesh* targetMesh)
{
	for (MeshGroup* group : targetMesh->groups)
	{
		Material* material = group->material;
		if (material != NULL)
		{
			string diffuseTextureMap = material->mapKd;
			if (diffuseTextureMap != "")
			{
				string textureFilePath = texturesLocationFolder + material->mapKd;
				group->texture = new Texture(textureFilePath.c_str());
			}
		}
	}
}