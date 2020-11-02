#include "Window.h"


// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "GLFW Starter Project";

// Objects to Render
Mesh* Window::bear;
Mesh* Window::bunny;
Mesh* Window::sandal;
Mesh* currObj;

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
	bear = new Mesh("bear.obj");
	bunny = new Mesh("bunny.obj");
	sandal = new Mesh("SandalF20.obj");

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
	currObj->draw(view, projection, shaderProgram);

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
		case GLFW_KEY_1:
			currObj = bear;
			break;

		case GLFW_KEY_2:
			currObj = bunny;
			break;

		case GLFW_KEY_3:
			currObj = sandal;
			break;

		default:
			break;
		}
	}
}

void Window::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	currObj->scale(yoffset);
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
		double velocity = glm::length(currPoint - prevPoint);
		// only move if velocity exceeds some threshold
		if (velocity > 0.0001) {
			// get rot Axis, rot angle and call rotate on currObj
			glm::vec3 rotAxis = glm::cross(prevPoint, currPoint);
			double rot_angle = 1.8 * velocity;
			currObj->rotate(rotAxis, rot_angle);
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
