//---------------------------------------------------
//
// Jeff Phillips | Derek Bamford
// CS-330 Computer Graphics and Visualization
// Assignment 4-3
// Basic Camera Movement
//
//---------------------------------------------------

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include "shader.h"
#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include<iostream>

void framebuffer_size_callback(GLFWwindow* window, int WIDTH, int HEIGHT);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void processInput(GLFWwindow* window);

//Sets the Height and Width of the window
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
	glfwInit(); //initializes GLFW

	//Tells GLFW what version of OpenGL we are using 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//Tells GLFW which OpenGL profile to use (package of functions)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	//Builds a window (Width, Height, Title, Full screen, <reasons>)
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Assignment 4_Bamford", NULL, NULL);

	//error checking for window
	if (window == NULL)
	{
		std::cout << "Failed to creat GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//Tells GLFW we would like to use the window we just created, because it is stupid
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	Shader ourShader("3.3.shader.vs", "3.3.shader.fs");

	GLfloat squareVertices[] = {
		// positions         // colors
		-0.5f, -0.5f, -0.5f,  0.5f, 0.0f, 1.0f,  // A 0
		 0.5f, -0.5f, -0.5f,  0.5f, 0.0f, 1.0f,  // B 1
		 0.5f,  0.5f, -0.5f,  0.5f, 0.0f, 1.0f,  // C 2
		-0.5f,  0.5f, -0.5f,  0.5f, 0.0f, 1.0f, // D 3
		-0.5f, -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,  // E 4
		 0.5f, -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,   // F 5
		 0.5f,  0.5f,  0.5f,  0.5f, 0.0f, 1.0f,  // G 6
		-0.5f,  0.5f,  0.5f,  0.5f, 0.0f, 1.0f,   // H 7

		-0.5f,  0.5f, -0.5f,  1.0f, 0.5f, 0.0f,  // D 8
		-0.5f, -0.5f, -0.5f,  1.0f, 0.5f, 0.0f,  // A 9
		-0.5f, -0.5f,  0.5f,  1.0f, 0.5f, 0.0f,  // E 10
		-0.5f,  0.5f,  0.5f,  1.0f, 0.5f, 0.0f,  // H 11
		 0.5f, -0.5f, -0.5f,  1.0f, 0.5f, 0.0f,   // B 12
		 0.5f,  0.5f, -0.5f,  1.0f, 0.5f, 0.0f,  // C 13
		 0.5f,  0.5f,  0.5f,  1.0f, 0.5f, 0.0f,   // G 14
		 0.5f, -0.5f,  0.5f,  1.0f, 0.5f, 0.0f,  // F 15

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.5f,  // A 16
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.5f,   // B 17
		 0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.5f,   // F 18
		-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.5f,  // E 19
		 0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.5f,  // C 20
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.5f,  // D 21
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.5f,  // H 22
		 0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.5f,  // G 23
	};

	unsigned int squareIndices[] = {
		0, 3, 2,
		2, 1, 0,
		4, 5, 6,
		6, 7 ,4,
		// left and right
		11, 8, 9,
		9, 10, 11,
		12, 13, 14,
		14, 15, 12,
		// bottom and top
		16, 17, 18,
		18, 19, 16,
		20, 21, 22,
		22, 23, 20
	};

	GLfloat tableVertices[] = {
		// positions         // colors
		-2.0f,  -1.0f, -2.0f,  1.0f, 1.0f, 1.0f,
		 2.0f,  -1.0f, -2.0f,  1.0f, 1.0f, 1.0f,
		 2.0f,  -1.0f,  2.0f,  1.0f, 1.0f, 1.0f,
		 2.0f,  -1.0f,  2.0f,  1.0f, 1.0f, 1.0f,
		-2.0f,  -1.0f,  2.0f,  1.0f, 1.0f, 1.0f,
		-2.0f,  -1.0f, -2.0f,  1.0f, 1.0f, 1.0f,
	};

	unsigned int tableIndices[] = {
	   0, 1, 2, 
	   3, 4, 5,
	   6,
	};

	GLfloat pyramidVertices[] = {
			// positions			 colors		
		-0.5f,  0.0f,  0.5f,	0.5f, 0.0f, 1.0f,		
		-0.5f,  0.0f, -0.5f,	1.0f, 5.0f, 0.0f,		
		 0.5f,  0.0f, -0.5f,	0.0f, 1.0f, 0.5f,		
		 0.5f,  0.0f,  0.5f,	0.5f, 0.0f, 1.0f,		
		 0.0f,  0.8f,  0.0f,	1.0f, 0.5f, 0.0f,		
	};

	unsigned int pyramidIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   0, 1, 4,
	   1, 2, 4,
	   2, 3, 4,
	   3, 0, 4
	};

	//The viewpoint goes from x = 0, y = 0, to x = 800, y = 600
	glViewport(0, 0, WIDTH, HEIGHT);

	// Vertex Array Object, Vertex Buffer Object, Element Array Object
	// VAO must be generated befor the VBO
	//In this example I am making two objects, one for each primative
	GLuint VAOs[3], VBOs[3], EBOs[3];
	glGenVertexArrays(3, VAOs);
	glGenBuffers(3, VBOs);
	glGenBuffers(3, EBOs);
//-----------------------------------------------------------------------------------------------------------------
	//first container setup
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);

	//Stores vertices in VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(squareIndices), squareIndices, GL_STATIC_DRAW);

	//configures so the OpenGl knows how to use the VBO, position attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
//-----------------------------------------------------------------------------------------------------------------
	//second container setup
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);

	//Stores vertices in VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(tableVertices), tableVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tableIndices), tableIndices, GL_STATIC_DRAW);

	//configures so the OpenGl knows how to use the VBO, position attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
//-----------------------------------------------------------------------------------------------------------------
	//Third container setup
	glBindVertexArray(VAOs[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);

	//Stores vertices in VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVertices), pyramidVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pyramidIndices), pyramidIndices, GL_STATIC_DRAW);

	//configures so the OpenGl knows how to use the VBO, position attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//-----------------------------------------------------------------------------------------------------------------
	
	//allows OpenGl to account for the depth of the container
	glEnable(GL_DEPTH_TEST);

	//A loop so that the window won't be terminated immediately
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//Calls the keyboard commands below
		processInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourShader.use();

		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
		ourShader.setMat4("projection", projection);

		// camera/view transformation
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("view", view);

		//render the Rubik's cube
		//----------------------------------------------------------------------------------------------------------
		glBindVertexArray(VAOs[0]);

		for (unsigned int i = 0; i < 1; i++)

		{   //initializes matrix to identity matrix
			glm::mat4 model = glm::mat4(1.0f);

			//moves the 3D object around the world
			model = glm::translate(model, glm::vec3(-3.5f, -5.0f, -13.0f));

			//Rotates the objects over the degees and x, y, z axis
			model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0, 1.0f, 0.0f));

			//changes the size of the object
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

			ourShader.setMat4("model", model);
			//draws the triangles
			glDrawElements(GL_TRIANGLES, sizeof(squareIndices) / sizeof(int), GL_UNSIGNED_INT, 0);
		}

		//render the countertop
		//------------------------------------------------------------------------------------------------------------
		glBindVertexArray(VAOs[1]);

		for (unsigned int i = 0; i < 1; i++)
		{
			//initializes matrix to identity matrix
			glm::mat4 model = glm::mat4(1.0f);

			//moves the 3D object around the world
			model = glm::translate(model, glm::vec3(0.0f, -2.5f, -15.0f));
			
			//Rotates the objects over the degees and x, y, z axis
			model = glm::rotate(model, glm::radians(360.0f), glm::vec3(1.0, 0.0f, 0.0f));

			//changes the size of the object
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			ourShader.setMat4("model", model);

			//draws the triangles
			glDrawElements(GL_TRIANGLES, sizeof(tableIndices) / sizeof(int), GL_UNSIGNED_INT, 0);
		}
		//render the Pyramid
		//------------------------------------------------------------------------------------------------------------
		glBindVertexArray(VAOs[2]);

		for (unsigned int i = 0; i < 1; i++)
		{
			//initializes matrix to identity matrix
			glm::mat4 model = glm::mat4(1.0f);

			//moves the 3D object around the world
			model = glm::translate(model, glm::vec3(0.0f, -5.0f, -13.0f));

			//Rotates the objects over the degees and x, y, z axis
			model = glm::rotate(model, glm::radians(.0f), glm::vec3(1.0, 0.0f, 0.0f));

			//changes the size of the object
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			ourShader.setMat4("model", model);

			//draws the triangles
			glDrawElements(GL_TRIANGLES, sizeof(pyramidIndices) / sizeof(int), GL_UNSIGNED_INT, 0);
		}

		glfwSwapBuffers(window);

		//processes all GLFW events other than closing
		glfwPollEvents();
	}

	//De-allocates resources
	glDeleteVertexArrays(3, VAOs);
	glDeleteBuffers(3, VBOs);
	glDeleteBuffers(3, EBOs);

	glfwDestroyWindow(window); //Terminates the window
	glfwTerminate(); //Terminates GLFW
	return 0;
}

void processInput(GLFWwindow* window)
{
	//Allows user to exit by pressing ESC
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	//Controls camera movement with keyboard inputs
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
}

// This method handles the resize of the window
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// Whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
