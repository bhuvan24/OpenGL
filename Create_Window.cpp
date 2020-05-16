/* Lesson 1 - Create An Empty Window
External Dependent Libs = GLFW
*/
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
using namespace std;

//*********Global Variables****************
const GLint WIDTH = 1000, HEIGHT = 800;
//*****************************************

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{
	//Check 1 - GLFW Initization
	if (!glfwInit())
	{
		cout << "Error - GLFW Init Failed" << endl;
		glfwTerminate();
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE, GL_TRUE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);

	//Check 2 - Window Initization
	if (!window)
	{
		cout << "GLFW window creation failed" << endl;
		glfwTerminate();
		return -1;
	}

	int buff_width, buff_height;
	glfwGetFramebufferSize(window, &buff_width, &buff_height);

	glfwMakeContextCurrent(window);

	//Resize window
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//Check 2 - GLAD Initization
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//Render loop
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		//To Colour window - RED,GREEN,BLUE,ALPHA
		glClearColor(1.0f, 0.2f, 1.0f, 0.5f);
		glClear(GL_COLOR_BUFFER_BIT);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;
}