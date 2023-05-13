//---------------------------------------------------
//
// Jeff Phillips | Derek Bamford
// CS-330 Computer Graphics and Visualization
// Milestone 3-4
// Pyramid//---------------------------------------------------
//
// Jeff Phillips | Derek Bamford
// CS-330 Computer Graphics and Visualization
// Milestone 3-4
// Pyramid & Square
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

	GLfloat triangleVertices[] = {
		// positions         // colors
		-0.5f,  0.0f,  0.5f,  0.5f, 0.0f, 1.0f,
		-0.5f,  0.0f, -0.5f,  1.0f, 5.0f, 0.0f,    
		 0.5f,  0.0f, -0.5f,  0.0f, 1.0f, 0.5f,   
		 0.5f,  0.0f,  0.5f,  0.5f, 0.0f, 1.0f,  
		 0.0f,  0.8f,  0.0f,  1.0f, 0.5f, 0.0f,  
	};

	unsigned int triangleIndices[] = {
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
	//In this example I am making two objects, one for each triangle
	GLuint VAOs[2], VBOs[2], EBOs[2];
	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);
	glGenBuffers(2, EBOs);

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

	//second container setup
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);

	//Stores vertices in VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangleIndices), triangleIndices, GL_STATIC_DRAW);

	//configures so the OpenGl knows how to use the VBO, position attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//allows OpenGl to account for the depth of the container
	glEnable(GL_DEPTH_TEST);

	//A loop so that the window won't be terminated immediately
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourShader.use();

		// create transformations
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view  = glm::mat4(1.0f);
		glm::mat4 proj  = glm::mat4(1.0f);

		//changes the size of the object
		//model = glm::scale(view, glm::vec3(3.0f, 3.0f, 3.0f));

		//moves the world
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));

		// field of view in radians, aspect ratio of our screen, and the closes and furthest view point
		proj = glm::perspective(glm::radians(45.0f), (float)(WIDTH / HEIGHT), 0.1f, 100.0f);

		int modelLoc = glGetUniformLocation(ourShader.ID, "model");
		int viewLoc = glGetUniformLocation(ourShader.ID, "view");
		int projLoc = glGetUniformLocation(ourShader.ID, "proj");

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

		//render the container
		glBindVertexArray(VAOs[0]);
		
		for (unsigned int i = 0; i < 1; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			//moves the 3D object around the world
			model = glm::translate(model, glm::vec3(-1.5f, 0.0f, -3.0f));
			float angle = 20.0f * i;
			//rotates object in real time (replace glfwGetTime() with int for static rotation
			model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
			ourShader.setMat4("model", model);
			//draws the triangles
			glDrawElements(GL_TRIANGLES, sizeof(squareIndices) / sizeof(int), GL_UNSIGNED_INT, 0);
		}

		//render the second container
		glBindVertexArray(VAOs[1]);

		for (unsigned int i = 0; i < 1; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			//moves the 3D object around the world
			model = glm::translate(model, glm::vec3(1.0f, 0.0f, -1.0f));
			float angle = 20.0f * i;
			//rotates object in real time (replace glfwGetTime() with int for static rotation
			model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
			ourShader.setMat4("model", model);
			//draws the triangles
			glDrawElements(GL_TRIANGLES, sizeof(triangleIndices) / sizeof(int), GL_UNSIGNED_INT, 0);
		}
	
		//Drawing elements
		
		
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

	
	GLfloat triangleVertices[] = {
		// positions         // colors
		-0.5f,  0.0f,  0.5f,  0.5f, 0.0f, 1.0f,
		-0.5f,  0.0f, -0.5f,  1.0f, 5.0f, 0.0f,    
		 0.5f,  0.0f, -0.5f,  0.0f, 1.0f, 0.5f,   
		 0.5f,  0.0f,  0.5f,  0.5f, 0.0f, 1.0f,  
		 0.0f,  0.8f,  0.0f,  1.0f, 0.5f, 0.0f,  
	};

	unsigned int triangleIndices[] = {
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
	//In this example I am making two objects, one for each triangle
	GLuint VAOs[2], VBOs[2], EBOs[2];
	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);
	glGenBuffers(2, EBOs);


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



	//second container setup
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);

	//Stores vertices in VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangleIndices), triangleIndices, GL_STATIC_DRAW);

	//configures so the OpenGl knows how to use the VBO, position attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	//allows OpenGl to account for the depth of the container
	glEnable(GL_DEPTH_TEST);

	//A loop so that the window won't be terminated immediately
	while (!glfwWindowShouldClose(window))
	{

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourShader.use();

		// create transformations
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);

		//moves the 3D object around the world
		model = glm::translate(view, glm::vec3(-1.5f, 0.0f, -3.0f));
		//model = glm::translate(view, glm::vec3( 1.5f, 0.0f, -3.0f));

		//changes the size of the object
		//model = glm::scale(view, glm::vec3(3.0f, 3.0f, 3.0f));

		//rotates the objects can; replace (float)glfwGetTime(), with a float value like 2.0f for a non continual rotation of object
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));

		//moves the world
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));


		// field of view in radians, aspect ratio of our screen, and the closes and furthest view point
		proj = glm::perspective(glm::radians(45.0f), (float)(WIDTH / HEIGHT), 0.1f, 100.0f);

		int modelLoc = glGetUniformLocation(ourShader.ID, "model");
		int viewLoc = glGetUniformLocation(ourShader.ID, "view");
		int projLoc = glGetUniformLocation(ourShader.ID, "proj");

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

		//render the container
		glBindVertexArray(VAOs[0]);
		//Drawing function start index is 0 and the number of vertices is 6, because its two triangle
		glDrawElements(GL_TRIANGLES, sizeof(squareIndices) / sizeof(int), GL_UNSIGNED_INT, 0);


		//render the second container
		glBindVertexArray(VAOs[1]);

		//Drawing function start index is 0 and the number of vertices is 6, because its two triangle
		glDrawElements(GL_TRIANGLES, sizeof(triangleIndices) / sizeof(int), GL_UNSIGNED_INT, 0);

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
