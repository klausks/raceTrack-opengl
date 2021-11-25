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
	//coreShader->LoadTexture("objs/trout/trout.bmp", "texture1", "troutTexture");
	//coreShader->LoadTexture("objs/LibertStatue/Liberty-GreenBronze-1.png", "texture1", "statueTexture");
	//coreShader->LoadTexture("objs/mesa/mesa01.bmp", "texture1", "mesaTexture");
	coreShader->Use();

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	return EXIT_SUCCESS;
}

void System::Run()
{
	/*
	Mesh* troutMesh = objReader->loadToMesh("objs/trout/", "trout.obj");
	// coreShader->UseTexture("troutTexture");
	Obj3D* trout = new Obj3D(troutMesh, true);
	trout->initialize();
	//trout->setTexture(coreShader->textures["troutTexture"].GetTextureId());
	trout->setShader(coreShader);
	trout->setPosition(glm::vec3(0.5f, 0.5f, -2.0f));
	this->objects.push_back(trout);


	Mesh* statueMesh = objReader->loadToMesh("objs/LibertStatue/", "LibertStatue.obj");
	Obj3D* statue = new Obj3D(statueMesh, true);
	statue->initialize();
	//statue->setTexture(coreShader->textures["statueTexture"].GetTextureId());
	statue->setShader(coreShader);
	statue->setPosition(glm::vec3(0.5f, 0.5f, 5.0f));
	this->objects.push_back(statue);

	Mesh* mesaMesh = objReader->loadToMesh("objs/mesa/", "mesa01.obj");
	Obj3D* mesa = new Obj3D(mesaMesh, true);
	mesa->initialize();
	//mesa->setTexture(coreShader->textures["mesaTexture"].GetTextureId());
	mesa->setShader(coreShader);
	statue->setPosition(glm::vec3(-1.0f, 0.5f, 3.0f));

	this->objects.push_back(mesa);

	Mesh* trackMesh = objReader->loadToMesh("objs/track/", "track.obj");
	Obj3D* track = new Obj3D(trackMesh, true);
	track->initialize();
	track->setShader(coreShader);
	this->objects.push_back(track);
	*/

	loadScene(SCENE_FILE_PATH, coreShader);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(window, (float)WIDTH / 2, (float)HEIGHT / 2);

	int viewLoc = glGetUniformLocation(coreShader->program, "view");
	int projectionLoc = glGetUniformLocation(coreShader->program, "projection");
	glm::mat4 view = glm::mat4();
	glm::mat4 projection = glm::mat4();


	while (!glfwWindowShouldClose(window)) {

		float currentTime = glfwGetTime();
		long currentTimeInMs = currentTime * 1000;
		float elapsedTime = currentTime - previousFrameTime;
		long elapsedTimeInMs = elapsedTime * 1000;

		previousFrameTimeInMs = currentTimeInMs;
		previousFrameTime = currentTime;

		glfwPollEvents();

#pragma region Input Handling

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			cam->processKeyboard(FORWARD, elapsedTime);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			cam->processKeyboard(LEFT, elapsedTime);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			cam->processKeyboard(BACKWARD, elapsedTime);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			cam->processKeyboard(RIGHT, elapsedTime);
		}

		view = cam->getViewMatrix();
		projection = glm::perspective(glm::radians(cam->zoom), aspectRatio, 0.1f, 100.0f);
#pragma endregion

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		/*
		for (Obj3D* obj : this->objects) {
			obj->update();
			obj->draw();
		}
		*/
		this->car->update();
		this->track->update();
		this->car->draw();
		this->track->draw();

		glfwSwapBuffers(window);
	}
}

void System::loadScene(string sceneFilePath, Shader* targetShader)
{
	ifstream arq(sceneFilePath);
	while (!arq.eof()) {
		string line;
		getline(arq, line);
		stringstream lineStream;
		lineStream << line;
		string temp;
		lineStream >> temp;
		if (temp == "obj") {
			string objType;
			string objFilePath;
			lineStream >> objType >> objFilePath;
			size_t fileFolderDelimiterPos = objFilePath.find_last_of("/\\");
			string objFolderPath = objFilePath.substr(0, fileFolderDelimiterPos + 1);
			string objFileName = objFilePath.substr(fileFolderDelimiterPos + 1);

			if (objType == "Car")
			{
				Mesh* carMesh = objReader->loadToMesh(objFolderPath, objFileName);
				Obj3D* car = new Obj3D(carMesh, true);
				car->initialize();
				car->setScale(glm::vec3(0.5f, 0.5f, 0.5f));
				car->setRotation(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				car->setRotation(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				car->setShader(targetShader);
				this->car = car;
			}
			else if (objType == "Track")
			{
				Mesh* trackMesh = objReader->loadToMesh(objFolderPath, objFileName);
				Obj3D* track = new Obj3D(trackMesh, true);
				track->initialize();
				track->setShader(targetShader);
				this->track = track;
			}
		}
		else if (temp == "curve") {
			string curveFilePath;
			lineStream >> curveFilePath;
			vector<glm::vec3> carTrajectory = loadCarTrajectory(curveFilePath);
			this->carAnimation = new CarAnimation(this->car, carTrajectory, CAR_SPEED);
		}
	}
	arq.close();
}

vector<glm::vec3> System::loadCarTrajectory(string curveFilePath)
{
	vector<glm::vec3> carTrajectory;
	ifstream curveFile(curveFilePath);
	while (!curveFile.eof()) {
		string line;
		getline(curveFile, line);
		stringstream lineStream;
		lineStream << line;
		float x, y, z;
		lineStream >> x >> y >> z;
		carTrajectory.push_back(glm::vec3(x, y, z));
	}
	return carTrajectory;
}

void System::Finish()
{
	coreShader->Delete();

	glfwTerminate();
}