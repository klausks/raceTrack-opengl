#include "CarAnimation.h"

CarAnimation::CarAnimation(Obj3D* car, vector<glm::vec3> trajectory, float speed)
{
	this->trajectory = trajectory;
	this->speed = speed;
	this->car = car;
	this->car->setTranslation(trajectory[0]);
	this->car->setRotation(-90.0, glm::vec3(1.0f, 0.0f, 0.0f));
	this->car->setRotation(-90.0, glm::vec3(0.0f, 0.0f, 1.0f));
	this->carForward = glm::vec3(0.0f, 1.0f, 0.0f);
	this->trajectoryIndex = 0;
	this->trajectoryLen = trajectory.size();
	this->worldForward = glm::normalize(trajectory[1] - trajectory[0]);
	this->worldRight = glm::vec3(0.0f, 0.0f, 1.0f);
	this->worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	this->carUp = glm::vec3(0.0f, 0.0f, -1.0f);
	this->carRight = glm::vec3(1.0f, 0.0f, 0.0f);
}

void CarAnimation::move()
{
	glm::vec3 displacement = getNextPoint() - getCurrentPoint();
	glm::vec3 forward = glm::normalize(displacement);
	car->setTranslation(displacement);
	setRotation(forward);
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

void CarAnimation::setRotation(glm::vec3 worldForward)
{
	float cos = glm::dot(worldForward, this->worldForward);
	if (cos == 1.0f)
	{
		return;
	}
	else
	{
		/*
		float senY = worldForward.y - this->worldForward.y;
		if (senY != 0)
		{
			float angleY = glm::asin(senY);
			car->setRotation(angleY, glm::vec3(1.0f, 0.0f, 0.0f));
		}
		*/
		float zDiff = worldForward.z - this->worldForward.z;
		float xDiff = worldForward.x - this->worldForward.x;
		float angleZ = glm::asin(zDiff);
		if (zDiff > 0)
		{
			car->setRotation(-angleZ, glm::vec3(0.0f, 0.0f, 1.0f));
		}
		else
		{
			car->setRotation(angleZ, glm::vec3(0.0f, 0.0f, 1.0f));
		}

	}
	this->worldForward = worldForward;
}