#pragma once

#include <vector>
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

class Obj3D
{
	public:
		Obj3D() {};
		Obj3D(Mesh* mesh, bool isStatic);

		void initialize();
		void passMeshDataToBuffers();
		void setBoundingSphere();
		float getDist(glm::vec3 p0, glm::vec3 p1);
		bool detectProjectileCollision(glm::vec3 projectileCenterPos, float projectileRadius);
		void setP0();
		void setP1();


		//Para setar os atributos 
		void setShader(Shader* shader) { this->shader = shader; shader->Use(); }
		void setPosition(glm::vec3 pos) { this->pos = pos; }
		void setDimension(glm::vec3 scale) { this->scale = scale; }
		void setAngle(float angle) { this->angle = angle; }


		//Para controle direto da matriz de transformações
		void setRotation(float angle, glm::vec3 axis);
		void setTranslation(glm::vec3 displacements);
		void setScale(glm::vec3 scaleFactors);

		void draw();
		void update();

		glm::vec3 pos, scale, p0, p1, centerPos;
		float angle;
		glm::mat4 transform;
		Mesh* mesh;
		Shader* shader;
		int numOfVertices;
		float radius;
		bool isStatic;
};