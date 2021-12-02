#include "Obj3D.h"

Obj3D::Obj3D(Mesh* mesh, bool isStatic)
{
	this->mesh = mesh;
	this->isStatic = isStatic;
}

void Obj3D::initialize()
{
	transform = glm::mat4(1);
	scale = glm::mat4(1);
	rotation = glm::mat4(1);
	translation = glm::mat4(1);
	pos = glm::vec3(0.0f, 0.0f, 0.0f);
	angle = 0.0f;

	setBoundingSphere();
	passMeshDataToBuffers();
}

void Obj3D::setBoundingSphere()
{
	setP0();
	setP1();
	centerPos = glm::vec3((p1.x + p0.x) / 2, (p1.y + p0.y) / 2, (p1.z + p0.z) / 2);
	float diagonal = getDist(p0, p1);
	radius = diagonal / 2;
}

void Obj3D::setP0()
{
	this->p0 = glm::vec3(mesh->minX, mesh->minY, mesh->minZ);
}

void Obj3D::setP1()
{
	this->p1 = glm::vec3(mesh->maxX, mesh->maxY, mesh->maxZ);
}

float Obj3D::getDist(glm::vec3 p0, glm::vec3 p1)
{
	float width = p1.x - p0.x;
	float height = p1.y - p0.y;
	float depth = p1.z - p0.z;
	return sqrt(pow(width, 2) + pow(height, 2) + pow(depth, 2));
}

void Obj3D::passMeshDataToBuffers()
{
	int groups = this->mesh->getNumOfGroups();
	for (int i = 0; i < groups; i++)
	{
		int verticesCount = 0;
		vector<float> vertices;
		vector<float> textureCoords;
		vector<float> normals;
		MeshGroup* group = this->mesh->getGroupAt(i);
		int numOfFaces = group->getNumOfFaces();
		for (int j = 0; j < numOfFaces; j++)
		{
			Face* face = group->getFaceAt(j);
			int numOfVertices = face->getNumOfVertices();
			for (int k = 0; k < numOfVertices; k++)
			{
				glm::vec3 vertex = mesh->vertices[face->vertexIndices[k]];
				vertices.push_back(vertex.x);
				vertices.push_back(vertex.y);
				vertices.push_back(vertex.z);
				verticesCount++;
			}
			int numOfTexCoords = face->getNumOfTextureCoords();
			for (int k = 0; k < numOfTexCoords; k++)
			{
				glm::vec2 textureCoord = mesh->textureCoords[face->textureIndices[k]];
				textureCoords.push_back(textureCoord.x);
				textureCoords.push_back(textureCoord.y);
			}
			int numOfNormals = face->getNumOfNormals();
			for (int k = 0; k < numOfNormals; k++)
			{
				glm::vec3 normal = mesh->normals[face->normalIndices[k]];
				normals.push_back(normal.x);
				normals.push_back(normal.y);
				normals.push_back(normal.z);
			}
		}
		group->numOfVertices = verticesCount;
		GLuint positionVBO = 0, texVBO = 0, normalVBO = 0, VAO = 0;
		glGenVertexArrays(1, &VAO);
		group->vao = VAO;
		glGenBuffers(1, &positionVBO);
		glGenBuffers(1, &texVBO);
		glGenBuffers(1, &normalVBO);

		glBindVertexArray(VAO);

		// Position attribute
		glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
		glEnableVertexAttribArray(0);

		// Texture attribute
		glBindBuffer(GL_ARRAY_BUFFER, texVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * textureCoords.size(), textureCoords.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
		glEnableVertexAttribArray(1);

		// Normal attribute
		glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * normals.size(), normals.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
		glEnableVertexAttribArray(2);

		glBindVertexArray(0); // Unbind VAO
	}
}

void Obj3D::setRotation(float angle, glm::vec3 axis)
{
	rotation = glm::rotate(rotation, angle, axis);
}

void Obj3D::setTranslation(glm::vec3 displacements)
{
	translation = glm::translate(translation, displacements);
}

void Obj3D::setScale(glm::vec3 scaleFactors)
{
	scale = glm::scale(scale, scaleFactors);
}

void Obj3D::draw()
{
	GLint transformLoc = glGetUniformLocation(this->shader->program, "model");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

	for (MeshGroup* group : this->mesh->groups)
	{
		group->bindTexture();
		glUniform1i(glGetUniformLocation(shader->program, "texture1"), 0);
		glBindVertexArray(group->vao);
		glDrawArrays(GL_TRIANGLES, 0, group->numOfVertices);
		glBindVertexArray(0);
	}
}

void Obj3D::update()
{
	transform = translation * rotation * scale;
}

bool Obj3D::detectProjectileCollision(glm::vec3 projectileCenterPos, float projectileRadius)
{
	float distBetweenCenters = getDist(this->centerPos, projectileCenterPos);
	if (distBetweenCenters < this->radius + projectileRadius) {
		return true;
	}
}