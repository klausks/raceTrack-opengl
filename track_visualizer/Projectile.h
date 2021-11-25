#pragma once
#include "Obj3D.h"

const float DEFAULT_VELOCITY = 0.00000001f;


class Projectile : public Obj3D {
public:

	float velocity;
	Projectile(Mesh* mesh) { this->mesh = mesh; }
	Projectile(Mesh* mesh, glm::vec3 pos, glm::vec3 direction, float velocity = DEFAULT_VELOCITY)
	{
		this->mesh = mesh;
		this->pos = pos;
		this->direction = direction;
		this->velocity = velocity;
	}

	void deflect(glm::vec3 targetObjCenterPos)
	{
		this->direction = normalize(this->pos - targetObjCenterPos);
	}

	void update()
	{
		transform = glm::translate(transform, direction*velocity);
		GLint transformLoc = glGetUniformLocation(this->shader->program, "model");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
	}
private:
	glm::vec3 direction;
};