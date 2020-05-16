/*Lesson 3 - Create triangle - colour by interpolation*/
#include <iostream>
#include <string.h>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


using namespace std;
const GLint WIDTH = 1000, HEIGHT = 800;

const float radians = 3.14f / 180.0f;

GLuint vao, vbo, shader, uniformModel;

bool direction = true;

float foffset = 0.0f;
float fmaxoffset = 0.7f;
float fincrement = 0.0005f;

float current_angle = 0.0f;

float scaler = 0.01f;

static const char* vertshader = "						\n\
#version 450											\n\
														\n\
layout (location = 0) in vec3 pos;						\n\
														\n\
out vec4 vCol;											\n\
														\n\
uniform mat4 model;										\n\
														\n\
void main()												\n\
{														\n\
	gl_Position = model * vec4(pos, 1.0);				 \n\
	vCol = vec4(clamp(pos, 0.0f ,1.0f), 1.0f);          \n\
}";

static const char* fragshader = "						\n\
#version 450											\n\
														\n\
in vec4 vCol;											\n\
														\n\
out vec4 colour;										\n\
														\n\
void main()												\n\
{														\n\
	colour = vCol;					    \n\
}";
void CreateTriangle()
{
	GLfloat vert[] = {
		-1.0f,-1.0f,0.0f,
		1.0f,-1.0f,0.0f,
		0.0f,1.0f,0.0f
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void Addshader(GLint prog, const char* shadercode, GLenum shadertype)
{
	GLuint theshader = glCreateShader(shadertype);
	const GLchar* thecode[1];
	thecode[0] = shadercode;

	GLint codelength[1];
	codelength[0] = strlen(shadercode);

	glShaderSource(theshader, 1, thecode, codelength);
	glCompileShader(theshader);

	GLint result = 0;
	GLchar logger[1024] = { 0 };

	glLinkProgram(theshader);
	glGetShaderiv(theshader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(theshader, sizeof(logger), NULL, logger);
		cout << "Error :" << logger << endl;
		return;
	}

	glAttachShader(prog, theshader);

	return;
}

void CompileShader()
{
	shader = glCreateProgram();

	if (!shader)
	{
		cout << "error in shader program" << endl;
		return;
	}

	Addshader(shader, vertshader, GL_VERTEX_SHADER);
	Addshader(shader, fragshader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar logger[1024] = { 0 };

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(logger), NULL, logger);
		cout << "Error :" << logger << endl;
		return;
	}
	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(logger), NULL, logger);
		cout << "Error :" << logger << endl;
		return;
	}

	uniformModel = glGetUniformLocation(shader, "model");

}
int main()
{
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

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);

	if (!window)
	{
		cout << "GLFW window creation failed" << endl;
		glfwTerminate();
		return -1;
	}

	int buff_width, buff_height;
	glfwGetFramebufferSize(window, &buff_width, &buff_height);

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		cout << "GLEW INIT failed" << endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		return -1;

	}

	glViewport(0, 0, buff_width, buff_height);

	CreateTriangle();
	CompileShader();

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		if (direction) {
			foffset += fincrement;
		}
		else
		{
			foffset -= fincrement;
		}

		if (abs(foffset) >= fmaxoffset)
		{
			direction = !direction;
		}

		if (current_angle < 360)
		{
			current_angle += 0.1f;
		}
		else
		{
			current_angle -= 360.0f;
		}

		if (scaler < 1.0f)
		{
			scaler += 0.00005f;
		}
		else
		{
			scaler = 0.00f;
		}
		glClearColor(0.0f, 0.1f, 0.15f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);

		glm::mat4 model(1.0f);
		/*translate */
		model = glm::translate(model,glm::vec3(0.0f,foffset,0.0f));
		/*Rotate*/
		model = glm::rotate(model, current_angle * radians, glm::vec3(0.0f, 0.0f, 1.0f));
		/*scale*/
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(vao);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(0);

		glUseProgram(0);
		glfwSwapBuffers(window);
	}
	return 0;
}