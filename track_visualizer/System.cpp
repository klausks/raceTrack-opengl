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

	window = glfwCreateWindow(WIDTH, HEIGHT, "Track", nullptr, nullptr);
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
	glEnable(GL_CULL_FACE);
	glDisable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	return EXIT_SUCCESS;
}

int System::SystemSetup()
{
	coreShader = new Shader("Shaders/Core/core.vert", "Shaders/Core/core.frag");
	coreShader->Use();
	loadScene(SCENE_FILE_PATH, coreShader);

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(window, (float)WIDTH / 2, (float)HEIGHT / 2);

	return EXIT_SUCCESS;
}

void System::Run()
{
	// Shader uniform locations relted to Camera
	int viewLoc = glGetUniformLocation(coreShader->program, "view");	
	int viewPosLoc = glGetUniformLocation(coreShader->program, "viewPos");
	int projectionLoc = glGetUniformLocation(coreShader->program, "projection");
	
	// Shader uniform locations related to light source
	const glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
	int lightColorLoc = glGetUniformLocation(coreShader->program, "lightColor");
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));
	
	int lightPosLoc = glGetUniformLocation(coreShader->program, "lightPos");
	int lightAmbientLoc = glGetUniformLocation(coreShader->program, "light.ambient");
	int lightDiffuseLoc = glGetUniformLocation(coreShader->program, "light.diffuse");
	int lightSpecularLoc = glGetUniformLocation(coreShader->program, "light.specular");

	glUniform3f(lightAmbientLoc, 0.3f, 0.3f, 0.3f);
	glUniform3f(lightDiffuseLoc, 1.0f, 1.0f, 1.0f);
	glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);

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

		glClearColor(0.5f, 0.2f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniform3fv(viewPosLoc, 1, glm::value_ptr(cam->position));
		glUniform3fv(lightPosLoc, 1, glm::value_ptr(cam->position));

		carAnimation->move();

		this->car->update();
		this->track->update();
		glEnable(GL_CULL_FACE);
		drawObj(car);
		glDisable(GL_CULL_FACE);
		drawObj(track);
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
	cam = new Camera(carAnimation->getCurrentPoint() + glm::vec3(0.0f, 15.0f, 15.0f), -90.0, -45.0);
}

vector<glm::vec3> System::loadCarTrajectory(string curveFilePath)
{
	vector<glm::vec3> carTrajectory;
	ifstream curveFile(curveFilePath);
	while (!curveFile.eof()) {
		string line;
		getline(curveFile, line);
		if (line == "") { break; }
		stringstream lineStream;
		lineStream << line;
		float x, y, z;
		lineStream >> x >> y >> z;
		carTrajectory.push_back(glm::vec3(x, y, z));
	}
	return carTrajectory;
}

void System::drawObj(Obj3D* obj)
{
	GLint transformLoc = glGetUniformLocation(obj->shader->program, "model");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(obj->transform));

	GLint mtlAmbientLoc = glGetUniformLocation(obj->shader->program, "material.ambient");
	GLint mtlDiffuseLoc = glGetUniformLocation(obj->shader->program, "material.diffuse");
	GLint mtlSpecularLoc = glGetUniformLocation(obj->shader->program, "material.specular");
	GLint mtlShininessLoc = glGetUniformLocation(obj->shader->program, "material.shininess");

	for (MeshGroup* group : obj->mesh->groups) {
		// Set lighting uniforms based on group material
		glUniform3fv(mtlAmbientLoc, 1, glm::value_ptr(group->material->ka));
		glUniform3fv(mtlDiffuseLoc, 1, glm::value_ptr(group->material->kd));
		glUniform3fv(mtlSpecularLoc, 1, glm::value_ptr(group->material->ks));
		glUniform1f(mtlShininessLoc, group->material->ns);

		group->bindTexture();
		glUniform1i(glGetUniformLocation(obj->shader->program, "texture1"), 0);
		glBindVertexArray(group->vao);
		glDrawArrays(GL_TRIANGLES, 0, group->numOfVertices);
		glBindVertexArray(0);
	}
}



void System::Finish()
{
	coreShader->Delete();
	glfwTerminate();
}