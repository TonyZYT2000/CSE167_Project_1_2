#include "Window.h"


// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "CSE167 Project 1 & 2";

// Objects to Render
Mesh* Window::bear;
Mesh* Window::bunny;
Mesh* Window::sandal;
Mesh* Window::lightSphere;
Mesh* currObj;

// Light source properties
glm::vec3 Window::lightPos(3, 3, 10);
glm::vec3 Window::lightColor(0.7, 0.98, 1);

// Rotation mode
int Window::rotateScaleMode = 1;

// Render mode
int Window::renderMode = 0;

// Camera Matrices 
// Projection matrix:
glm::mat4 Window::projection; 

// View Matrix:
glm::vec3 Window::eyePos(0, 0, 20);			// Camera position.
glm::vec3 Window::lookAtPoint(0, 0, 0);		// The point we are looking at.
glm::vec3 Window::upVector(0, 1, 0);		// The up direction of the camera.
glm::mat4 Window::view = glm::lookAt(Window::eyePos, Window::lookAtPoint, Window::upVector);

// pressed flag
bool Window::pressed = false;
glm::vec3 Window::prevPoint;

// Shader Program ID
GLuint Window::shaderProgram; 

bool Window::initializeProgram() {
	// Create a shader program with a vertex shader and a fragment shader.
	shaderProgram = LoadShaders("shaders/shader.vert", "shaders/shader.frag");

	// Check the shader program.
	if (!shaderProgram)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}

	return true;
}

bool Window::initializeObjects()
{
	// Load 3 models as point clouds
	// gold bear
	bear = new Mesh("bear.obj", glm::vec3(0.1, 0.08, 0.06), glm::vec3(0, 0, 0), glm::vec3(0.98, 0.99, 1));
	// purple rubber bunny
	bunny = new Mesh("bunny.obj", glm::vec3(0.12, 0.09, 0.13), glm::vec3(0.7, 0.5, 0.8), glm::vec3(0, 0, 0));
	// jade sandal
	sandal = new Mesh("SandalF20.obj", glm::vec3(0.05, 0.1, 0.08), glm::vec3(0.54, 0.89, 0.63), glm::vec3(0.8, 0.8, 0.6));

	// light source sphere
	lightSphere = new Mesh("sphere.obj", lightColor, glm::vec3(0), glm::vec3(0));
	lightSphere->scale(0.02);
	lightSphere->translate(lightPos);

	// Set bear to be the first to display
	currObj = bear;

	return true;
}

void Window::cleanUp()
{
	// Deallcoate the objects.
	delete bear;
	delete bunny;
	delete sandal;

	// Delete the shader program.
	glDeleteProgram(shaderProgram);
}

GLFWwindow* Window::createWindow(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	// 4x antialiasing.
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ 
	// Apple implements its own version of OpenGL and requires special treatments
	// to make it uses modern OpenGL.

	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window.
	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

	// Check if the window could not be created.
	if (!window)
	{
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window.
	glfwMakeContextCurrent(window);

#ifndef __APPLE__
	// On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

	// Initialize GLEW.
	if (glewInit())
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}
#endif

	// Set swap interval to 1.
	glfwSwapInterval(0);

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);

	return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	// In case your Mac has a retina display.
	glfwGetFramebufferSize(window, &width, &height); 
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size.
	glViewport(0, 0, width, height);

	// Set the projection matrix.
	Window::projection = glm::perspective(glm::radians(60.0), 
								double(width) / (double)height, 1.0, 1000.0);
}

void Window::idleCallback()
{
	// Perform any necessary updates here 
	// currObj->update();
}

void Window::displayCallback(GLFWwindow* window)
{	
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	// Render the objects
	glUseProgram(shaderProgram);
	glUniform1i(glGetUniformLocation(shaderProgram, "renderMode"), renderMode);
	glUniform3fv(glGetUniformLocation(shaderProgram, "eyePos"), 1, glm::value_ptr(Window::eyePos));
	glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos"), 1, glm::value_ptr(lightPos));
	glUniform3fv(glGetUniformLocation(shaderProgram, "lightColor"), 1, glm::value_ptr(lightColor));
	currObj->draw(view, projection, shaderProgram);
	lightSphere->draw(view, projection, shaderProgram);

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();

	// Swap buffers.
	glfwSwapBuffers(window);
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	/*
	 * TODO: Modify below to add your key callbacks.
	 */
	
	// Check for a key press.
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);				
			break;

		// switch between the cube and the cube pointCloud
		case GLFW_KEY_F1:
			currObj = bear;
			break;

		case GLFW_KEY_F2:
			currObj = bunny;
			break;

		case GLFW_KEY_F3:
			currObj = sandal;
			break;

		case GLFW_KEY_1:
			rotateScaleMode = 1;
			break;

		case GLFW_KEY_2:
			rotateScaleMode = 2;
			break;

		case GLFW_KEY_3:
			rotateScaleMode = 3;
			break;

		case GLFW_KEY_N:
			renderMode = (renderMode == 0) ? 1 : 0;
			break;

		default:
			break;
		}
	}
}

void Window::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	if (rotateScaleMode == 1) {
            currObj->scalebyScroll(yoffset);
	}
	else if (rotateScaleMode == 2) {
		glm::vec3 trans;
		trans[0] = 0.1 * yoffset * lightPos[0];
		trans[1] = 0.1 * yoffset * lightPos[1];
		trans[2] = 0.1 * yoffset * lightPos[2];
		lightSphere->translate(trans);
		lightPos = lightPos + trans;
	}
	else {
            currObj->scalebyScroll(yoffset);
		glm::vec3 trans;
		trans[0] = 0.1 * yoffset * lightPos[0];
		trans[1] = 0.1 * yoffset * lightPos[1];
		trans[2] = 0.1 * yoffset * lightPos[2];
		lightPos = lightPos + trans;
		lightSphere->scalebyScroll(yoffset);
	}
}

void Window::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	// when left button pressed
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		// set flag to true
		pressed = true;
		// calculate pressed position in 3D
		double xpos;
		double ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		prevPoint = trackBallMapping(glm::vec2(xpos, ypos));
	}
	// else, set flag to false
	else {
		pressed = false;
	}
}

void Window::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
	// when flag pressed is true
	if (pressed) {
		// get current screen position and calculate current position in 3D
		glm::vec2 currPos(xpos, ypos);
		glm::vec3 currPoint = trackBallMapping(currPos);

		// get velocity
		float velocity = glm::length(currPoint - prevPoint);
		// only move if velocity exceeds some threshold
		if (velocity > 0.0001) {
			// get rot Axis, rot angle and call rotate on currObj
			glm::vec3 rotAxis = glm::cross(prevPoint, currPoint);
			float rotAngle = 1.8 * velocity;
			if (rotateScaleMode == 1) {
                        currObj->rotate(rotAxis, rotAngle);
			}
			else if (rotateScaleMode == 2) {
				lightSphere->rotate(rotAxis, rotAngle);
				lightPos = glm::vec3(glm::rotate(glm::mat4(1), rotAngle, rotAxis) * glm::vec4(lightPos, 1));
			}
			else {
                        currObj->rotate(rotAxis, rotAngle);
				lightSphere->rotate(rotAxis, rotAngle);
				lightPos = glm::vec3(glm::rotate(glm::mat4(1), rotAngle, rotAxis) * glm::vec4(lightPos, 1));
			}
			prevPoint = currPoint;
		}
	}
}

glm::vec3 Window::trackBallMapping(glm::vec2 point) {
	glm::vec3 v;
	double d;
	v.x = (2 * point.x - width) / width;
	v.y = (height - 2 * point.y) / height;
	v.z = 0;
	d = glm::length(v);
	d = (d < 1) ? d : 1;
	v.z = sqrt(1.001 - d * d);
	v = v / glm::length(v);
	return v;
}
