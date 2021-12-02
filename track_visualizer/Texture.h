#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include "AssetManager.h"

class Texture
{
private:
	GLuint textureID;
	GLint textureLocation;
	GLshort textureNum;

public:
	Texture();
	Texture(const char* path, const char* textureUniformName, GLuint shaderProgram, GLint textureNum);
	Texture(const char* path);
	~Texture();

	GLuint GetTextureId() { return textureID; }
	GLshort GetTextureNum() { return textureNum; }
};

#endif