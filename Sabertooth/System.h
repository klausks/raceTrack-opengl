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
#include "Bspline.h"
#include "ObjWriter.h"
#include "SceneWriter.h"

// VBOs and VAOs
static Bspline* bSpline = new Bspline();
static GLuint controlPointsVAO, controlPointsVBO;
static GLuint bSplineVAO, bSplineVBO, bSplineInVAO, bSplineInVBO, bSplineExVAO, bSplineExVBO;
static ObjWriter* objWriter;
static SceneWriter* sceneWriter;

// parameters
static const float WIDTH = 1280, HEIGHT = 840;
static const string TARGET_TRACK_FOLDER = "../track_visualizer/objs/track/";
static const string TARGET_SCENE_FOLDER = "../track_visualizer/scene/";
static const string CAR_OBJ_FOLDER = "../track_visualizer/objs/cars/slot_car_blue/";
static const string CAR_OBJ_FILE = "slot_car_blue_diffuse.obj";
static const int FPS = 60;
static float lastMouseXPos;
static float lastMouseYPos;


class System
{
private:
	// control variables
	int screenWidth, screenHeight;
	long elapsedTimeInMs = 0.0f;
	long previousFrameTimeInMs = 0.0f;


	//ObjWriter* objWriter = new ObjWriter();

	// input handlers
	void process_keyboard_input();
	static void mouse_callback(GLFWwindow* window, int button, int action, int mods);


	// utility functions
	void clearScreen() {
		glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	static void delVAO(GLuint& vao) {
		glDeleteVertexArrays(1, &vao);
	}

	static void delVBO(GLuint& vbo) {
		glDeleteBuffers(1, &vbo);

	}

	static void genVAO(GLuint& vao) {
		glGenVertexArrays(1, &vao);
	}

	static void genVBO(GLuint& vbo) {
		glGenBuffers(1, &vbo);
	}

	static void bindVAO(GLuint& vao) {
		glBindVertexArray(vao);
	}

	static void updateVaoAndVbo(GLuint& vao, GLuint& vbo, vector<glm::vec3> vertices) {
		vector<float> verticesArr;
		for (glm::vec3 vertice : vertices)
		{
			verticesArr.push_back(vertice.x);
			verticesArr.push_back(vertice.y);
			verticesArr.push_back(vertice.z);
		}
		bindVAO(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, verticesArr.size() * sizeof(float), verticesArr.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);
	}

	static void updateVAOsAndVBOs() {
		updateVaoAndVbo(controlPointsVAO, controlPointsVBO, bSpline->controlPoints);
		updateVaoAndVbo(bSplineVAO, bSplineVBO, bSpline->curvePoints);
		updateVaoAndVbo(bSplineExVAO, bSplineExVBO, bSpline->externalCurvePoints);
		updateVaoAndVbo(bSplineInVAO, bSplineInVBO, bSpline->internalCurvePoints);
	}

	void draw();
	void genVBOsAndVAOs();

public:
	GLFWwindow* window;
	Shader* coreShader;
	
	System();
	~System();

	int GLFWInit();
	int OpenGLSetup();
	int SystemSetup();
	void Run();
	void Finish();

};

#endif