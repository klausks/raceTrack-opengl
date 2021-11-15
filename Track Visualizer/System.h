#ifndef SYSTEM_H
#define SYSTEM_H

// Internal
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <math.h>

// External Libs
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <SOIL.h>

// GLM Includes
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

// Headers
#include "Shader.h"
#include "AssetManager.h"
#include "Time.h"
#include "Camera.h"
#include "ObjReader.h"
#include "Mesh.h"
#include "Obj3D.h"
#include "Projectile.h"

static Camera* cam = new Camera(glm::vec3(0.0f, 0.0f, 10.0f));
static const GLint WIDTH = 1280, HEIGHT = 840;
static float deltaTime = 0.0f;
static float lastFrame = 0.0f;
static float lastMouseXPos = WIDTH / 2;
static float lastMouseYPos = HEIGHT / 2;

class System
{
private:
	// Screen
	static void mouse_callback(GLFWwindow* window, double xPos, double yPos);
	static void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
	void processCollision(Obj3D* obj);

	int screenWidth, screenHeight;
	ObjReader* objReader;
	vector<Obj3D*> objects;
	Projectile* projectile;

public:
	GLFWwindow* window;
	Shader* coreShader;
	GLfloat aspectRatio;
	GLfloat fov;
	
	System();
	~System();

	//void mouse_callback(GLFWwindow* windows, double xPos, double yPos);
	//void scroll_callback(GLFWwindow* windows, double xOffset, double yOffset);

	int GLFWInit();
	int OpenGLSetup();
	int SystemSetup();

	void loadScene(string configFileName);
	void addObj3D(Obj3D* obj);


	void Run();

	void Finish();

};

#endif