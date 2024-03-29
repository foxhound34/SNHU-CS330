//---------------------------------------------------
//
// Jeff Phillips | Derek Bamford
// CS-330 Computer Graphics and Visualization
// Assignment 2-3
// 2D Triangles
//
//---------------------------------------------------


#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;


// Shaders saved in a C-string to be compiled later
const char *vertexShaderSource = "#version 440 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec4 colorFromVBO;\n"
    "out vec4 colorFromVS;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "   colorFromVS = colorFromVBO;\n"
    "}\n\0";
 
 
// Fragment Shader Program Source Code
const char *fragmentShaderSource = "#version 440 core\n"
    "in vec4 colorFromVS;\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = colorFromVS;\n"
    "}\n\0";

int main()
{
	glfwInit(); //initializes GLFW

	//Tells GLFW what version of OpenGL we are using 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//Tells GLFW which OpenGL profile to use (package of functions)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLfloat vertices[] =
	{
		//Triangle One
		// x     y    z				  Colors
		//Index 0 (Bottom Left)
		-1.0f,  0.0f, 0.0f,    0.0f, 0.0f, 1.0f, //Blue

		//Index 1 will repeat for both triangles Bottom Right
		-0.5f,  0.0f, 0.0f,    0.0f, 1.0f, 0.0f, //Green

		//Index 2 (Top Left)
		-1.0f,  1.0f, 0.0f,    1.0f, 0.0f, 0.0f, //Red

		//Index 3 (Bottom Left, Triangle 2)
		 0.0f, -1.0f, 0.0f,    0.0f, 1.0f, 0.0f, //Green

		 //Index 4 (Bottom Right, Triangle 2)
		 0.0f,  0.0f, 0.0f,    1.0f, 0.0f, 0.0f, //Red
	};

	unsigned int indices[] =
	{
		0, 1, 2,
		3, 1, 4
	};

	//Builds a window (Width, Height, Title, Full screen, <reasons>)
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Assignment 2_Bamford", NULL, NULL);

	//error checking for window
	if (window == NULL)
	{
		std::cout << "Failed to creat GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	//Tells GLFW we would like to use the window we just created, because it is stupid
	glfwMakeContextCurrent(window);

	//Loads GLAD so it configures OpenGL
	gladLoadGL();

	//The viewpoint goes from x = 0, y = 0, to x = 600, y = 480
	glViewport(0, 0, WIDTH, HEIGHT);

	//Creates a reference to store a vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

	//must be complied immediately to turn source code into machine code
	glCompileShader(vertexShader);

	//Creates a reference to store a fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

	//must be complied immediately to turn source code into machine code
	glCompileShader(fragmentShader);

	//shaders need to be "wrapped" into a shader program in order to use
	GLuint shaderProgram = glCreateProgram();

	//attaches shader to the shader program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	//Wraps the shader program
	glLinkProgram(shaderProgram);

	//shaders are attached to the program and can now be deleted.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	// Vertex Array Object, Vertex Buffer object,Element Buffer Object 
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

	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//swaps back buffer with the front buffer
	glfwSwapBuffers(window);


	//A loop so that the window won't be terminated immediately
	while (!glfwWindowShouldClose(window))
	{

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);

		//render the first triangle
		glBindVertexArray(VAO);

		//Drawing function start index is 0 and the number of vertices is 6, because its two triangle
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);

		//processes all GLFW events other than closing
		glfwPollEvents();
	}

	//De-allocates resources
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window); //Terminates the window
	glfwTerminate(); //Terminates GLFW
	return 0;
}
