#pragma once

#include <vector>
#include "Obj3D.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

class CarAnimation
{
public:
	CarAnimation(Obj3D* car, vector<glm::vec3> trajectory, float speed);

	void move();
	void resetTrajectoryIndex();
	glm::vec3 getCurrentPoint();
	glm::vec3 getNextPoint();
	void setRotation(glm::vec3 direction);
	// getDirection
	// setDirection
	// setRotation
	// getRotation
	// getTranslation
	// setTranslation
	// getTransformation

private:
	const glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);
	void updateVectors();
	vector<glm::vec3> trajectory;
	int trajectoryIndex;
	int trajectoryLen;
	float speed;
	Obj3D* car;
	glm::vec3 worldForward, carForward, worldUp, carUp, carRight, worldRight;
};