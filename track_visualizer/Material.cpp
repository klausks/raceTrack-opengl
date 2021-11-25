#include "Material.h"

Material::Material(string id) {
	this->id = id;
	this->ka = { 0.0f, 0.0f, 0.0f };
	this->kd = { 0.0f, 0.0f, 0.0f };
	this->ks = { 0.0f, 0.0f, 0.0f };
	this->ns = 0.0f;
	this->mapKd = "";
}