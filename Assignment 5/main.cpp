//---------------------------------------------------
//
// Jeff Phillips | Derek Bamford
// CS-330 Computer Graphics and Visualization
// Assignment 5
// Textures
//
//---------------------------------------------------

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include "shader.h"
#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include<iostream>

void framebuffer_size_callback(GLFWwindow* window, int WIDTH, int HEIGHT);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void processInput(GLFWwindow* window);

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;
float MovementSpeed = 5.0f;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

//sets the perspective
bool perspective = false;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
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
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Assignment 5_Bamford", NULL, NULL);

	//error checking for window
	if (window == NULL)
	{
		std::cout << "Failed to creat GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	//Tells GLFW we would like to use the window we just created, because it is stupid
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	Shader ourShader("3.3.shader.vs", "3.3.shader.fs");
	std::cout << glGetError() << std::endl; // returns 0 (no error)


	GLfloat pyramidVertices[] = {
			// positions			 colors				texture
		-0.5f,  0.0f,  0.5f,	0.83f, 0.70f, 0.44f,		0.0f, 0.0f,
		-0.5f,  0.0f, -0.5f,	0.83f, 0.70f, 0.44f,		1.0f, 0.0f, //left face
		 0.5f,  0.0f, -0.5f,	0.83f, 0.70f, 0.44f,		0.0f, 0.0f, //bottom face
		 0.5f,  0.0f,  0.5f,	0.83f, 0.70f, 0.44f,		1.0f, 0.0f, //front face
		 0.0f,  0.8f,  0.0f,	0.92f, 0.86f, 0.76f,		0.5f, 1.0f
	};

	unsigned int pyramidIndices[] = {
	   0, 1, 4, //left face
	   1, 2, 4, //back face
	   2, 3, 4, //right face
	   3, 0, 4 //front face
	};

	GLfloat pyramidBaseVertices[] = {
		// positions			 colors				texture
	-0.5f,  0.0f,  0.5f,	1.0f, 1.0f, 1.0f,		0.5f, -0.5f,
	-0.5f,  0.0f, -0.5f,	1.0f, 1.0f, 1.0f,	   -0.5f, -0.5f,
	 0.5f,  0.0f, -0.5f,	1.0f, 1.0f, 1.0f,	   -0.5f,  0.5f,
	 0.5f,  0.0f,  0.5f,	1.0f, 1.0f, 1.0f,	    0.5f,  0.5f, 
	};

	unsigned int baseIndices[] = {
	   0, 1, 2, // left half bottom
	   0, 2, 3, // right half bottom
	};


	//The viewpoint goes from x = 0, y = 0, to x = 800, y = 600
	glViewport(0, 0, WIDTH, HEIGHT);

	// Vertex Array Object, Vertex Buffer Object, Element Array Object
	// VAO must be generated befor the VBO
	//In this example I am making two objects, one for each triangle
	GLuint VAOs[2], VBOs[2], EBOs[2];
	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);
	glGenBuffers(2, EBOs);

	//______________________________________________________________________________________________________________________________
		//first container setup
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);

	//Stores vertices in VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVertices), pyramidVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pyramidIndices), pyramidIndices, GL_STATIC_DRAW);

	//configures so the OpenGl knows how to use the VBO, position attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	//________________________________________________________________________________________________________________________________
		//Second container setup
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);

	//Stores vertices in VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidBaseVertices), pyramidBaseVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(baseIndices), baseIndices, GL_STATIC_DRAW);

	//configures so the OpenGl knows how to use the VBO, position attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
		//allows OpenGl to account for the depth of the container
	glEnable(GL_DEPTH_TEST);

	unsigned int texture1;
	// texture 1
	// ---------
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char *data = stbi_load("Brick.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	
	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	ourShader.use();
	std::cout << glGetError() << std::endl; // returns 0 (no error)

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
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		//initializes a projection matrix (needed to be added after moving projectiosn to if statement
		glm::mat4 projection;

		// pass projection matrix to shader (note that in this case it could change every frame)
		//Referenced from https://stackoverflow.com/questions/44710262/opengl-switching-between-ortho-and-perspecive-problems
		if (!perspective)
		{
			//orth perspective
			projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);
		}
			//projection perspective
		else
			projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);


		ourShader.setMat4("projection", projection);

		// camera/view transformation
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("view", view);

		//render the Pyramid faces
		glBindVertexArray(VAOs[0]);

		for (unsigned int i = 0; i < 1; i++)

		{   
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture1);

			//initializes matrix to identity matrix
			glm::mat4 model = glm::mat4(1.0f);

			//moves the 3D object around the world
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));

			//Rotates the objects over the degees and x, y, z axis
			model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0, 1.0f, 0.0f));

			//changes the size of the object
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

			ourShader.setMat4("model", model);
			//draws the triangles
			glDrawElements(GL_TRIANGLES, sizeof(pyramidIndices) / sizeof(int), GL_UNSIGNED_INT, 0);
		}
		glBindVertexArray(VAOs[1]);
		for (unsigned int i = 0; i < 1; i++)

		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture1);

			//initializes matrix to identity matrix
			glm::mat4 model = glm::mat4(1.0f);

			//moves the 3D object around the world
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));

			//Rotates the objects over the degees and x, y, z axis
			model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0, 1.0f, 0.0f));

			//changes the size of the object
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

			ourShader.setMat4("model", model);
			//draws the triangles
			glDrawElements(GL_TRIANGLES, sizeof(baseIndices) / sizeof(int), GL_UNSIGNED_INT, 0);
		}

		glfwSwapBuffers(window);

		//processes all GLFW events other than closing
		glfwPollEvents();
	}

	//De-allocates resources
	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);
	glDeleteBuffers(2, EBOs);

	glfwDestroyWindow(window); //Terminates the window
	glfwTerminate(); //Terminates GLFW
	return 0;
}

void processInput(GLFWwindow* window)
{
	//Controls ability to ecape the window
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

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

	//changes camera perspective
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		perspective = false;

	else
		perspective = true;
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
