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
	setRotation(direction);
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

void CarAnimation::setRotation(glm::vec3 direction)
{
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
		float senX = this->direction.x - direction.x;
		if (senX != 0)
		{
			float angleX = glm::asin(senX);
			car->setRotation(angleX, glm::vec3(0.0f, 0.0f, 1.0f));
		}
	}
	this->direction = direction;
}