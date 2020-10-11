#include <iostream>
#include <ft2build.h>
#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include <GLFW/glfw3.h>
#include <gllabel/gllabel.hpp>
using namespace std;
using namespace glm;

int main()
{
	if (!glfwInit()) {
		const char* error;
		glfwGetError(&error);
		cerr << "Failed to initialize GLFW: " << error <<"\n";
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(800, 600, "Vector-Based GPU Text Rendering", NULL, NULL);
	if (!window) {
		const char* error;
		glfwGetError(&error);
		cerr << "Failed to create GLFW window: " << error << "\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);
	if (glewExperimental = true; glewInit() != GLEW_OK) {
		cerr << "Failed to initialize GLEW.\n";
		glfwTerminate();
		return -1;
	}

	cout << "GL Version: " << glGetString(GL_VERSION) << "\n";

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glClearColor(160/255.0, 169/255.0, 175/255.0, 1.0);
	
	GLLabel label("Hi");

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) break;
		float time = glfwGetTime();
			
		
		glClear(GL_COLOR_BUFFER_BIT);
		label.render(mat4(1));

		glfwSwapBuffers(window);
	}

	glfwTerminate();
}

