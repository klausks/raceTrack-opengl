#include "CarAnimation.h"

CarAnimation::CarAnimation(Obj3D* car, vector<glm::vec3> trajectory, float speed)
{
	this->trajectory = trajectory;
	this->speed = speed;
	this->car = car;
	this->car->setTranslation(trajectory[0]);
	this->trajectoryIndex = 0;
	this->trajectoryLen = trajectory.size();
	this->direction = glm::normalize(trajectory[1] - trajectory[0]);
}

void CarAnimation::move()
{
	glm::vec3 displacement = getNextPoint() - getCurrentPoint();
	glm::vec3 direction = glm::normalize(displacement);
	car->setTranslation(displacement);
	setRotation(direction);
	trajectoryIndex++;
}

glm::vec3 CarAnimation::getCurrentPoint()
{
	if (trajectoryIndex > trajectoryLen - 1)
	{
		resetTrajectoryIndex();
	}
	glm::vec3 currentPoint = trajectory[trajectoryIndex];
	return currentPoint;
}
glm::vec3 CarAnimation::getNextPoint()
{
	return trajectory[(trajectoryIndex + 1) %  trajectoryLen];
}

void CarAnimation::resetTrajectoryIndex()
{
	trajectoryIndex = 0;
}

void CarAnimation::setRotation(glm::vec3 direction)
{
	car->setRotation(1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	/*
	glm::vec3 axis = glm::cross(direction, this->direction);
	float sin = glm::length(axis);
	// float cos = glm::clamp(glm::dot(direction, this->direction), -1.0f, 1.0f);

	float angle = glm::degrees(glm::asin(sin));
	car->setRotation(angle, axis);
	this->direction = direction;
	*/

	/*
	float cos = glm::dot(direction, this->direction);
	if (cos == 1)
	{
		return;
	}
	else
	{
		
		float senY = direction.y - this->direction.y;
		if (senY != 0)
		{
			float angleY = glm::asin(senY);
			car->setRotation(angleY, glm::vec3(1.0f, 0.0f, 0.0f));
		}
		
		float zDiff = direction.z - this->direction.z;
		float xDiff = direction.x - this->direction.x;
		if (zDiff > 0) {
			float angleZ = glm::asin(zDiff);
			car->setRotation(angleZ, glm::vec3(0.0f, 0.0f, 1.0f));
		}
		else
		{
			float angleZ = glm::asin(xDiff);
			car->setRotation(angleZ, glm::vec3(0.0f, 0.0f, 1.0f));
		}

	}
	this->direction = direction;
	*/
}