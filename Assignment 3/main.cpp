//---------------------------------------------------
//
// Jeff Phillips | Derek Bamford
// CS-330 Computer Graphics and Visualization
// Assignment 3-3
// Pyramid
//
//---------------------------------------------------



#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include<iostream>

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;


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
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Assignment 3_Bamford", NULL, NULL);

	//error checking for window
	if (window == NULL)
	{
		std::cout << "Failed to creat GLFW window" << std::endl;
		glfwTerminate();
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

	GLfloat vertices[] = {
		// positions         // colors
		-0.5f,  0.0f,  0.5f,  0.5f, 0.0f, 1.0f,
		-0.5f,  0.0f, -0.5f,  1.0f, 5.0f, 0.0f,    
		 0.5f,  0.0f, -0.5f,  0.0f, 1.0f, 0.5f,   
		 0.5f,  0.0f,  0.5f,  0.5f, 0.0f, 1.0f,  
		 0.0f,  0.8f,  0.0f,  1.0f, 0.5f, 0.0f,  
	};

	unsigned int indices[] = {
	   0, 1, 2, 
	   0, 2, 3, 
	   0, 1, 4,
	   1, 2, 4,
	   2, 3, 4,
	   3, 0, 4
	};

	//The viewpoint goes from x = 0, y = 0, to x = 800, y = 600
	glViewport(0, 0, WIDTH, HEIGHT);


	// Vertex Array Object, Vertex Buffer object
	// VAO must be generated befor the VBO
	//In this example I am making two objects, one for each triangle
	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);


	//first triangle setup
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Stores vertices in VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//configures so the OpenGl knows how to use the VBO, position attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//color attribute (3 represents the  RGB)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//allows OpenGl to account for the depth of the container
	glEnable(GL_DEPTH_TEST);

	//A loop so that the window won't be terminated immediately
	while (!glfwWindowShouldClose(window))
	{
		//Sets background color to black
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		//clears the color and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//activates our shader program
		ourShader.use();

		// create transformations
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);

		//glm::mat4 proj = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 100.0f);

		//positions the object within the space
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));

		//Rotates the object in real time over the x, y, z axis
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.5f, 0.0f));
		
		//moves the world not the "viewpoint"
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -4.0f));

		// field of view in radians, aspect ratio of our screen, and the closes and furthest view point
		proj = glm::perspective(glm::radians(45.0f), (float)(WIDTH / HEIGHT), 0.1f, 100.0f);

		int modelLoc = glGetUniformLocation(ourShader.ID, "model");
		int viewLoc = glGetUniformLocation(ourShader.ID, "view");
		int projLoc = glGetUniformLocation(ourShader.ID, "proj");

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
	
		//render the container
		glBindVertexArray(VAO);

		//Drawing function start index is 0 and the number of vertices is 6, because its two triangle
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);

		//processes all GLFW events other than closing
		glfwPollEvents();
	}

	//De-allocates resources
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwDestroyWindow(window); //Terminates the window
	glfwTerminate(); //Terminates GLFW
	return 0;
}
