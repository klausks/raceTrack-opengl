#include "System.h"
#include "Projectile.h"

void System::mouse_callback(GLFWwindow* windows, double xPos, double yPos)
{
	float xOffset = xPos - lastMouseXPos;
	float yOffset = lastMouseYPos - yPos; // reversed since y-coordinates go from bottom to top

	lastMouseXPos = xPos;
	lastMouseYPos = yPos;

	cam->processMouseMovement(xOffset, yOffset);
}

void System::scroll_callback(GLFWwindow* windows, double xOffset, double yOffset)
{
	cam->processMouseScroll(yOffset);
}

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

	window = glfwCreateWindow(WIDTH, HEIGHT, "3DShooter", nullptr, nullptr);

	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
	aspectRatio = (float)screenWidth / (float)screenHeight;
	fov = 45.0f;

	if (window == nullptr) {
		std::cout << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		std::cout << "Failed no init GLEW." << std::endl;
		return EXIT_FAILURE;
	}

	glViewport(0, 0, screenWidth, screenHeight);


	return EXIT_SUCCESS;

}

int System::OpenGLSetup()
{

	glEnable(GL_BLEND);	// Enables blending ( glBlendFunc )
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CW);

	return EXIT_SUCCESS;
}

int System::SystemSetup()
{
	coreShader = new Shader("Shaders/Core/core.vert", "Shaders/Core/core.frag");
	coreShader->LoadTexture("objs/trout/trout.bmp", "texture1", "troutTexture");
	coreShader->LoadTexture("objs/LibertStatue/Liberty-GreenBronze-1.png", "texture1", "statueTexture");
	coreShader->LoadTexture("objs/mesa/mesa01.bmp", "texture1", "mesaTexture");
	coreShader->Use();

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	return EXIT_SUCCESS;
}

void System::Run()
{

	Mesh* troutMesh = objReader->read("objs/trout/", "trout.obj");
	coreShader->UseTexture("troutTexture");
	Obj3D* trout = new Obj3D(troutMesh, true);
	trout->initialize();
	//trout->setTexture(coreShader->textures["troutTexture"].GetTextureId());
	trout->setShader(coreShader);
	trout->setPosition(glm::vec3(0.5f, 0.5f, -2.0f));
	this->objects.push_back(trout);

	Mesh* statueMesh = objReader->read("objs/LibertStatue/", "LibertStatue.obj");
	Obj3D* statue = new Obj3D(statueMesh, true);
	statue->initialize();
	//statue->setTexture(coreShader->textures["statueTexture"].GetTextureId());
	statue->setShader(coreShader);
	statue->setPosition(glm::vec3(0.5f, 0.5f, 5.0f));
	this->objects.push_back(statue);

	Mesh* mesaMesh = objReader->read("objs/mesa/", "mesa01.obj");
	Obj3D* mesa = new Obj3D(mesaMesh, true);
	mesa->initialize();
	//mesa->setTexture(coreShader->textures["mesaTexture"].GetTextureId());
	mesa->setShader(coreShader);
	statue->setPosition(glm::vec3(-1.0f, 0.5f, 3.0f));

	this->objects.push_back(mesa);


	glm::mat4 projection(1.0f);
	glm::mat4 view = glm::mat4(1.0f);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(window, (float)WIDTH / 2, (float)HEIGHT / 2);


	while (!glfwWindowShouldClose(window)) {

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();

		projection = glm::perspective(glm::radians(cam->zoom), aspectRatio, 0.1f, 100.0f);
		view = cam->getViewMatrix();

#pragma region Input Handling

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			cam->processKeyboard(FORWARD, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			cam->processKeyboard(LEFT, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			cam->processKeyboard(BACKWARD, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			cam->processKeyboard(RIGHT, deltaTime);
		}

#pragma endregion

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		int viewLoc = glGetUniformLocation(coreShader->program, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		int projectionLoc = glGetUniformLocation(coreShader->program, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		for (Obj3D* obj : this->objects) {
			obj->update();
			obj->draw();
		}

		glfwSwapBuffers(window);
	}
}

void System::loadScene(string configFileName)
{

}

void System::Finish()
{
	coreShader->Delete();

	glfwTerminate();
}