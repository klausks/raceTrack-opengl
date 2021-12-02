#include "System.h"

System::System()
{
}

System::~System()
{
}

int System::GLFWInit()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Track Generator", nullptr, nullptr);
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed no init GLEW." << std::endl;
		return EXIT_FAILURE;
	}
	glViewport(0, 0, screenWidth, screenHeight);
	return EXIT_SUCCESS;
}

int System::OpenGLSetup()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	return EXIT_SUCCESS;
}

int System::SystemSetup()
{
	projection = glm::ortho(0.0f, WIDTH, HEIGHT, 0.0f, -255.0f, 255.0f);
	coreShader = new Shader("Shaders/Core/core.vert", "Shaders/Core/core.frag");
	coreShader->Use();

	glfwSetMouseButtonCallback(window, mouse_callback);
	genVBOsAndVAOs();

	return EXIT_SUCCESS;
}

void System::mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		double currentXPos, currentYpos;
		glfwGetCursorPos(window, &currentXPos, &currentYpos);
		if (lastMouseXPos == currentXPos && lastMouseYPos == currentYpos)
		{
			return;
		}
		lastMouseXPos = currentXPos;
		lastMouseYPos = currentYpos;

		bSpline->insertControlPoint(glm::vec3(currentXPos, currentYpos, 0.0f));
		updateVAOsAndVBOs();
	}
}

void System::genVBOsAndVAOs()
{
	genVAO(controlPointsVAO);
	genVAO(bSplineVAO);
	genVAO(bSplineExVAO);
	genVAO(bSplineInVAO);

	genVBO(controlPointsVBO);
	genVBO(bSplineVBO);
	genVBO(bSplineExVBO);
	genVBO(bSplineInVBO);
}

void System::Run()
{
	glfwSetCursorPos(window, (float)WIDTH / 2, (float)HEIGHT / 2);
	int projectionLoc = glGetUniformLocation(coreShader->program, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	while (!glfwWindowShouldClose(window))
	{
		long currentTimeInMs = long(glfwGetTime()*1000);
		elapsedTimeInMs = currentTimeInMs - previousFrameTimeInMs;
		if (elapsedTimeInMs < (1 / FPS) * 1000)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds((1 / FPS) * 1000 - elapsedTimeInMs));
		}
		previousFrameTimeInMs = currentTimeInMs;

		glfwPollEvents();
		process_keyboard_input();
		clearScreen();
		draw();
		glfwSwapBuffers(window);
	}
}

void System::process_keyboard_input()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		bSpline->clear();
		updateVAOsAndVBOs();
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		updateHeight(5.0f);
		updateVAOsAndVBOs();
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		updateHeight(-5.0f);
		updateVAOsAndVBOs();
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		objWriter = new ObjWriter(bSpline, TARGET_TRACK_FOLDER);
		objWriter->write();
		sceneWriter = new SceneWriter(objWriter, TARGET_SCENE_FOLDER, CAR_OBJ_FOLDER + CAR_OBJ_FILE);
		sceneWriter->write();
	}
}

void System::draw()
{
	// Control points
	bindVAO(controlPointsVAO);
	glPointSize(5);
	glDrawArrays(GL_POINTS, 0, bSpline->controlPoints.size());

	// B-Spline curve 
	bindVAO(bSplineVAO);
	glDrawArrays(GL_LINE_STRIP, 0, bSpline->curvePoints.size());

	// B-Spline external curve 
	bindVAO(bSplineExVAO);
	glDrawArrays(GL_LINE_STRIP, 0, bSpline->externalCurvePoints.size());

	// B-Spline internal curve 
	bindVAO(bSplineInVAO);
	glDrawArrays(GL_LINE_STRIP, 0, bSpline->internalCurvePoints.size());
}

void System::Finish()
{
	coreShader->Delete();
	glfwTerminate();
}

float System::euclideanDistance(float x1, float y1, float x2, float y2)
{
	float xDist = x2 - x1;
	float yDist = y2 - y1;
	float distance = sqrt(pow(xDist, 2) + pow(yDist, 2));
	return distance;
}

void System::updateHeight(float value)
{
	double xCursorPos, yCursorPos;
	glfwGetCursorPos(window, &xCursorPos, &yCursorPos);

	// Find control point closest to mouse position
	int closestControlPointIdx = 0;
	float smallestDist = euclideanDistance(bSpline->controlPoints[0].x, bSpline->controlPoints[0].y, xCursorPos, yCursorPos);
	
	for (int i = 1; i < bSpline->controlPoints.size(); i++)
	{
		float currentDist = euclideanDistance(bSpline->controlPoints[i].x, bSpline->controlPoints[i].y, xCursorPos, yCursorPos);
		if (currentDist < smallestDist)
		{
			smallestDist = currentDist;
			closestControlPointIdx = i;
		}
	}

	float newHeight = bSpline->controlPoints[closestControlPointIdx].z + value;
	if (newHeight > 255.0f)
	{
		bSpline->controlPoints[closestControlPointIdx].z = 255.0f;
	}
	else if (newHeight < 0.0f)
	{
		bSpline->controlPoints[closestControlPointIdx].z = 0.0f;
	}
	else
	{
		bSpline->controlPoints[closestControlPointIdx].z = newHeight;
	}
	bSpline->update();
}