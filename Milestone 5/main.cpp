//---------------------------------------------------
//
// Jeff Phillips | Derek Bamford
// CS-330 Computer Graphics and Visualization
// Milestone 5
// Texturing Objects in a 3D Scene
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
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Milestone 5 Bamford", NULL, NULL);

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

	//Tells GLFW we would like to use the window we just created, because it is stupid
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Places shaders into a single callable variable
	Shader ourShader("3.3.shader.vs", "3.3.shader.fs");

		//  ++++++++++++++++++++++++++++++
		//  +                            +
		//  +    Ribik's Cube Mesh       +
		//	+                            +
		//  ++++++++++++++++++++++++++++++
	GLfloat squareVertices[] = {
		// positions              colors              texture

		//Left and Right Side
		-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,		0.0f, 0.0f, // 0 upper left
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,		1.0f, 0.0f, //1 upper right
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,		1.0f, 1.0f, //2 bottom left
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,		0.0f, 1.0f, //3 bottom right
		-0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,		0.0f, 0.0f, //4 upper left
		 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,		1.0f, 0.0f, //5 upper right
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,		1.0f, 1.0f, //6 bottom left
		-0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,		0.0f, 1.0f, //7 bottom right

		//Front and Back
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,		0.0f, 1.0f, //8 upper left
		-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,		1.0f, 1.0f, //9 upper right
		-0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,		1.0f, 0.0f, //10 bottom left
		-0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,		0.0f, 0.0f, //11 bottom right
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,		0.0f, 1.0f, //12 upper left
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,		1.0f, 1.0f, //13 upper right
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,		1.0f, 0.0f, //14 bottom left
		 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,		0.0f, 0.0f, //15 bottom right

		 //Top and Bottom
		-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,		0.0f, 0.0f, //upper left 16
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,		0.0f, 1.0f, //bottom right 17
		 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,		1.0f, 1.0f, //upper right 18
		-0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,		0.0f, 1.0f, //bottom right 19
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,		0.0f, 0.0f, //upper left 20
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,		1.0f, 0.0f, //upper right 21
		-0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,		1.0f, 1.0f, //bottom left 22
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,		0.0f, 1.0f, //bottom right 23
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

		//  ++++++++++++++++++++++++++++++
		//  +                            +
		//  +      Countertop Mesh       +
		//	+                            +
		//  ++++++++++++++++++++++++++++++
	GLfloat tableVertices[] = {
		// positions			// colors			texture
		-2.0f,  -1.0f, -2.0f,	1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
		 2.0f,  -1.0f, -2.0f,	1.0f, 1.0f, 1.0f,	1.0f, 0.0f,
		 2.0f,  -1.0f,  2.0f,	1.0f, 1.0f, 1.0f,	1.0f, 1.0f,
		 2.0f,  -1.0f,  2.0f,	1.0f, 1.0f, 1.0f,	1.0f, 0.0f,
		-2.0f,  -1.0f,  2.0f,	1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
		-2.0f,  -1.0f, -2.0f,	1.0f, 1.0f, 1.0f,	0.0f, 1.0f,
	};

	unsigned int tableIndices[] = {
	   0, 1, 2,   // first triangle
	   3, 4, 5,
	   6
	};

		//  ++++++++++++++++++++++++++++++
		//  +                            +
		//  +       Computer Mesh        +
		//	+                            +
		//  ++++++++++++++++++++++++++++++
	GLfloat computerVertices[] = {
		// positions               colors            texture
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};

	unsigned int computerIndices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};


	//The viewpoint goes from x = 0, y = 0, to x = 800, y = 600
	glViewport(0, 0, WIDTH, HEIGHT);

	// Vertex Array Object, Vertex Buffer Object, Element Array Object
	// VAO must be generated befor the VBO
	//In this example I am making two objects, one for each triangle
	GLuint VAOs[3], VBOs[3], EBOs[3];
	glGenVertexArrays(3, VAOs);
	glGenBuffers(3, VBOs);
	glGenBuffers(3, EBOs);



		//  ++++++++++++++++++++++++++++++
		//  +                            +
		//  +    Rubik's Cube Setup      +
		//	+                            +
		//  ++++++++++++++++++++++++++++++
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);

	//Stores vertices in VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(squareIndices), squareIndices, GL_STATIC_DRAW);

	//configures so the OpenGl knows how to use the VBO, position attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);



		//  ++++++++++++++++++++++++++++++
		//  +                            +
		//  +     Countertop Setup       +
		//	+                            +
		//  ++++++++++++++++++++++++++++++
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);

	//Stores vertices in VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(tableVertices), tableVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tableIndices), tableIndices, GL_STATIC_DRAW);

	//configures so the OpenGl knows how to use the VBO, position attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	
	    //  ++++++++++++++++++++++++++++++
		//  +                            +
		//  +       Computer Setup       +
		//	+                            +
		//  ++++++++++++++++++++++++++++++
	glBindVertexArray(VAOs[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);

	//Stores vertices in VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(computerVertices), computerVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(computerIndices), computerIndices, GL_STATIC_DRAW);

	//configures so the OpenGl knows how to use the VBO, position attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//______________________________________________________________________________________________________________________________________________________

	//allows OpenGl to account for the depth of the container
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_FRAMEBUFFER_SRGB);

	unsigned int texture1, texture2, texture3, texture4, texture5, texture6;



	//  ++++++++++++++++++++++++++++++
	//  +                            +
	//  +    Rubik's Cube Texture    +
	//	+                            +
	//  ++++++++++++++++++++++++++++++
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
	unsigned char* data = stbi_load("Cube2.jpg", &width, &height, &nrChannels, 0);
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



	//  ++++++++++++++++++++++++++++++
	//  +                            +
	//  +     Countertop Texture     +
	//	+                            +
	//  ++++++++++++++++++++++++++++++
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps

	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data2 = stbi_load("Granite.jpg", &width, &height, &nrChannels, 0);
	if (data2)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data2);



	//  ++++++++++++++++++++++++++++++
	//  +                            +
	//  +     Keyboard Texture       +
	//	+                            +
	//  ++++++++++++++++++++++++++++++
	glGenTextures(1, &texture3);
	glBindTexture(GL_TEXTURE_2D, texture3);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps

	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data3 = stbi_load("Keyboard.jpg", &width, &height, &nrChannels, 0);
	if (data3)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data3);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data3);



	//  ++++++++++++++++++++++++++++++
	//  +                            +
	//  +  Computer Screen Texture   +
	//	+                            +
	//  ++++++++++++++++++++++++++++++
	glGenTextures(1, &texture4);
	glBindTexture(GL_TEXTURE_2D, texture4);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps

	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data4 = stbi_load("Screen2.jpg", &width, &height, &nrChannels, 0);
	if (data4)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data4);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data4);

	    //  ++++++++++++++++++++++++++++++
		//  +                            +
		//  +       Cover Texture        +
		//	+                            +
		//  ++++++++++++++++++++++++++++++
	glGenTextures(1, &texture5);
	glBindTexture(GL_TEXTURE_2D, texture5);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps

	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data5 = stbi_load("computerBack.jpg", &width, &height, &nrChannels, 0);
	if (data5)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data5);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data5);

	//  ++++++++++++++++++++++++++++++
	//  +                            +
	//  +    Caution Sign Texture    +
	//	+                            +
	//  ++++++++++++++++++++++++++++++
	glGenTextures(1, &texture6);
	glBindTexture(GL_TEXTURE_2D, texture6);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps

	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data6 = stbi_load("Caution.png", &width, &height, &nrChannels, 0);
	if (data6)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data6);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data6);

	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	ourShader.use();
	ourShader.setInt("texture4", 0);
	ourShader.setInt("texture6", 1);
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
			//perspective projection
			projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);
		}
		//ortho projection
		else
			projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);


		ourShader.setMat4("projection", projection);

		// camera/view transformation
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("view", view);


		//  ++++++++++++++++++++++++++++++
		//  +                            +
		//  +     Ribik's Cube Draw      +
		//	+                            +
		//  ++++++++++++++++++++++++++++++

		glBindVertexArray(VAOs[0]);

		for (unsigned int i = 0; i < 1; i++)

		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture1);

			//initializes matrix to identity matrix
			glm::mat4 model = glm::mat4(1.0f);

			//moves the 3D object around the world
			model = glm::translate(model, glm::vec3(-3.5f, -4.7f, -13.0f));

			//Rotates the objects over the degees and x, y, z axis
			model = glm::rotate(model, glm::radians(100.0f), glm::vec3(0.0, 1.0f, 0.0f));

			//changes the size of the object
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

			ourShader.setMat4("model", model);
			//draws the triangles
			glDrawElements(GL_TRIANGLES, sizeof(squareIndices) / sizeof(int), GL_UNSIGNED_INT, 0);
		}
		std::cout << glGetError() << std::endl;

		//  ++++++++++++++++++++++++++++++
		//  +                            +
		//  +      Countertop Draw       +
		//	+                            +
		//  ++++++++++++++++++++++++++++++

		glBindVertexArray(VAOs[1]);

		for (unsigned int i = 0; i < 1; i++)

		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture2);

			//initializes matrix to identity matrix
			glm::mat4 model = glm::mat4(1.0f);

			//moves the 3D object around the world
			model = glm::translate(model, glm::vec3(0.0f, -0.3f, -13.0f));

			//Rotates the objects over the degees and x, y, z axis
			model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0, 0.0f, 0.0f));

			//changes the size of the object
			model = glm::scale(model, glm::vec3(6.0f, 5.0f, 3.0f));

			ourShader.setMat4("model", model);
			//draws the triangles
			glDrawElements(GL_TRIANGLES, sizeof(tableIndices) / sizeof(int), GL_UNSIGNED_INT, 0);
		}
		std::cout << glGetError() << std::endl;

		/// ++++++++++++++++++++++++++++++
		//  +                            +
		//  +        Keyboard Draw       +
		//	+                            +
		//  ++++++++++++++++++++++++++++++

		glBindVertexArray(VAOs[2]);

		for (unsigned int i = 0; i < 1; i++)

		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture3); //keyboard

			//initializes matrix to identity matrix
			glm::mat4 model = glm::mat4(1.0f);

			//moves the 3D object around the world
			model = glm::translate(model, glm::vec3(1.0f, -5.25f, -13.5f));

			//Rotates the objects over the degees and x, y, z axis
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0, 0.0f, 0.0f));

			//changes the size of the object
			model = glm::scale(model, glm::vec3(6.0f, 5.0f, 5.0f));

			ourShader.setMat4("model", model);
			//draws the triangles
			glDrawElements(GL_TRIANGLES, sizeof(computerIndices) / sizeof(int), GL_UNSIGNED_INT, 0);
		}
		std::cout << glGetError() << std::endl;

		//  ++++++++++++++++++++++++++++++
		//  +                            +
		//  +        Screen Draw         +
		//	+                            +
		//  ++++++++++++++++++++++++++++++

		for (unsigned int i = 0; i < 1; i++)

		{
			//Overlays two textures on a single surface
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture6); //caution picture
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texture4); //screen picture
			bool multipleTextures = false;

			//initializes matrix to identity matrix
			glm::mat4 model = glm::mat4(1.0f);

			//moves the 3D object around the world
			model = glm::translate(model, glm::vec3(1.0f, -3.5f, -17.75f));

			//Rotates the objects over the degees and x, y, z axis
			model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0, 0.0f, 0.0f));

			//changes the size of the object
			model = glm::scale(model, glm::vec3(6.0f, 5.0f, 5.0f));

			ourShader.setMat4("model", model);
			//draws the triangles
			glDrawElements(GL_TRIANGLES, sizeof(computerIndices) / sizeof(int), GL_UNSIGNED_INT, 0);
		}
		std::cout << glGetError() << std::endl;

		//  ++++++++++++++++++++++++++++++
		//  +                            +
		//  +         Cover Draw         +
		//	+                            +
		//  ++++++++++++++++++++++++++++++

			for (unsigned int i = 0; i < 1; i++)

			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture5); //Computer Screen back

				//initializes matrix to identity matrix
				glm::mat4 model = glm::mat4(1.0f);

				//moves the 3D object around the world
				model = glm::translate(model, glm::vec3(1.0f, -3.5f, -17.76f));

				//Rotates and flips the object over the degees and x, y, z axis
				model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0, 0.0f, 0.0f));
				model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0, 1.0f, 0.0f));

				//changes the size of the object
				model = glm::scale(model, glm::vec3(6.0f, 5.0f, 5.0f));

				ourShader.setMat4("model", model);
				//draws the triangles
				glDrawElements(GL_TRIANGLES, sizeof(computerIndices) / sizeof(int), GL_UNSIGNED_INT, 0);
			}
		std::cout << glGetError() << std::endl;

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
	//Control ability to escape the program
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	//controls the movement of the camera
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

	//Controls the perspective of the camera
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
