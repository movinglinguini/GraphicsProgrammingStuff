#include <iostream>

// GLEW
//#define GLEW_STATIC
#include <glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include <SOIL.h>
// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other includes
#include "Shader.h"
#include "Model.h"
#include "Camera.h"

Camera cam(glm::vec3(0.0f, 1.0f, 3.0f));

//Some utility variables
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

double lastX = 0.0;
double lastY = 0.0;

bool keys[1024];
bool first_mouse = true;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double x_pos, double y_pos);

// Utility functions
void move_cam();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// The MAIN function, from here we start the application and run the game loop
int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);

	// Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();

    // Define the viewport dimensions
    glViewport(0, 0, WIDTH, HEIGHT);
	
	//Setup OpenGL options
	glEnable(GL_DEPTH_TEST);


	Shader shader("vertShader.glsl", "fragShader.glsl");

	Model character("nanosuit/nanosuit.obj");

	// Game loop
    while (!glfwWindowShouldClose(window))
    {
		GLfloat currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;

        // Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
		move_cam();

        // Render
        // Clear the color buffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.Use();

		glm::mat4 view = cam.GetViewMatrix();
		glm::mat4 projection= glm::perspective(cam.Zoom, (GLfloat)WIDTH / HEIGHT, 0.1f, 100.0f);
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));

		GLuint viewLoc = glGetUniformLocation(shader.Program, "view");
		GLuint projectionLoc = glGetUniformLocation(shader.Program, "projection");
		GLuint modelLoc = glGetUniformLocation(shader.Program, "model");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		character.Draw(shader);


        // Swap the screen buffers
        glfwSwapBuffers(window);
    }
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;

}

void mouse_callback(GLFWwindow * window, double x_pos, double y_pos)
{
	if (first_mouse)
	{
		lastX = x_pos;
		lastY = y_pos;
		first_mouse = false;
	}

	GLfloat x_offset = x_pos - lastX;
	GLfloat y_offset = lastY - y_pos;

	lastX = x_pos;
	lastY = y_pos;

	cam.ProcessMouseMovement(x_offset, y_offset);
}

void move_cam()
{
	if (keys[GLFW_KEY_W])
	{
		cam.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (keys[GLFW_KEY_A])
	{
		cam.ProcessKeyboard(LEFT, deltaTime);
	}
	if (keys[GLFW_KEY_S])
	{
		cam.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (keys[GLFW_KEY_D])
	{
		cam.ProcessKeyboard(RIGHT, deltaTime);
	}
	if (keys[GLFW_KEY_E])
	{
		cam.ProcessKeyboard(UP, deltaTime);
	}
	if (keys[GLFW_KEY_Q])
	{
		cam.ProcessKeyboard(DOWN, deltaTime);
	}
}