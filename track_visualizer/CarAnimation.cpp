#include "CarAnimation.h"

CarAnimation::CarAnimation(Obj3D* car, vector<glm::vec3> trajectory, float speed)
{
	this->trajectory = trajectory;
	this->speed = speed;
	this->car = car;
	this->car->setTranslation(trajectory[0]);
	this->trajectoryIndex = 0;
	this->trajectoryLen = trajectory.size();
}

void CarAnimation::move()
{
	glm::vec3 displacement = getNextPoint() - getCurrentPoint();
	glm::vec3 direction = glm::normalize(displacement);
	car->setTranslation(displacement);
	trajectoryIndex++;
}

glm::vec3 CarAnimation::getCurrentPoint()
{
	if (trajectoryIndex == trajectory.size())
	{
		resetTrajectoryIndex();
	}
	glm::vec3 currentPoint = trajectory[trajectoryIndex];
	return currentPoint;
}
glm::vec3 CarAnimation::getNextPoint()
{
	return trajectory[(trajectoryIndex + 1) % trajectoryLen];
}

void CarAnimation::resetTrajectoryIndex()
{
	trajectoryIndex = 0;
}