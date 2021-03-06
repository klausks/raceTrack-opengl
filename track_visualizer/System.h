#ifndef SYSTEM_H
#define SYSTEM_H

// Internal
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <math.h>
#include <thread>
#include <cstddef>

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
#include "CarAnimation.h"

static const GLint WIDTH = 1600, HEIGHT = 840;
static const int FPS = 60;
static const float CAR_SPEED = 10.0f;
static const string SCENE_FILE_PATH = "scene/scene.txt";


static Camera* cam; // = new Camera(glm::vec3(0.0f, 0.0f, 10.0f));
static float deltaTime = 0.0f;
static float lastFrame = 0.0f;
static float lastMouseXPos = WIDTH / 2;
static float lastMouseYPos = HEIGHT / 2;
static float previousFrameTime = 0.0f;
static long previousFrameTimeInMs = 0.0f;

class System
{
private:
	static void mouse_callback(GLFWwindow* window, double xPos, double yPos);
	static void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
	void processCollision(Obj3D* obj);

	int screenWidth, screenHeight;
	ObjReader* objReader;
	vector<Obj3D*> objects;
	Obj3D* car;
	Obj3D* track;
	Projectile* projectile;
	CarAnimation* carAnimation;

public:
	System();
	~System();

	GLFWwindow* window;
	Shader* coreShader;
	GLfloat aspectRatio;
	GLfloat fov;

	int GLFWInit();
	int OpenGLSetup();
	int SystemSetup();

	// load files generated from Track Generator program
	void loadScene(string sceneFilePath, Shader* targetShader);
	vector<glm::vec3> loadCarTrajectory(string curveFilePath);

	// obj handling
	void addObj3D(Obj3D* obj);
	void setObjLighitingUniforms(Obj3D* obj, Shader* shader);
	void drawObj(Obj3D* obj);

	void Run();
	void Finish();
};

#endif