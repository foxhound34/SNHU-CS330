//---------------------------------------------------
//
// Jeff Phillips | Derek Bamford
// CS-330 Computer Graphics and Visualization
// Milestone 6
// Lighting a 3D Scene
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

const unsigned int WIDTH = 800; // Sets the window witdth
const unsigned int HEIGHT = 600; // Sets the window height
float MovementSpeed = 5.0f;

// camera
Camera camera(glm::vec3(0.0f, 1.5f, 4.0f)); //sets the camera location
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

//sets the perspective
bool perspective = false; // Sets perspective bool variable

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

//Sets the location of the cubes at point of the light source
glm::vec3 pointLightPositions[] = {

	glm::vec3( 2.0f, 2.0f, -14.0f), // Key light location [0]
	glm::vec3(-2.0f, 2.0f, -14.0f), // Key light location [1]
	glm::vec3(-8.0f, 2.0f, -14.0f), // Key light location [2]
	glm::vec3( 8.0f, 2.0f, -14.0f), // Key light location [3]
};

//Changes the color of the light source
//Color used is called "Wheat" it allows for a softer white light that better replicates the intial photo.
//I found an all white source was too harsh and washed out the scene
glm::vec3 pointLightColors[] = {
	glm::vec3(0.961f, 0.871f, 0.702f), //white  set intensity to 10%
	glm::vec3(0.961f, 0.871f, 0.702f), //white  set intensity to 10%
	glm::vec3(0.961f, 0.871f, 0.702f), //white  set intensity to 100%
	glm::vec3(0.961f, 0.871f, 0.702f), //white  set intensity to 100%

};

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
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Milestone 6_Bamford", NULL, NULL);

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

	//allows OpenGl to account for the depth of the container
	glEnable(GL_DEPTH_TEST);

	Shader lightingShader("2.2.basic_lighting.vs", "2.2.basic_lighting.fs");
	Shader lightCubeShader("2.2.light_cube.vs", "2.2.light_cube.fs");
	std::cout << glGetError() << std::endl; // returns 0 (no error)

	//Vertex and Normals from learnopengl.com

		//  ++++++++++++++++++++++++++++++
		//  +                            +
		//  +    Ribik's Cube Mesh       +
		//	+                            +
		//  ++++++++++++++++++++++++++++++
	GLfloat cubeVertices[] = {

		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};

	//  ++++++++++++++++++++++++++++++
	//  +                            +
	//  +      Countertop Mesh       +
	//	+                            +
	//  ++++++++++++++++++++++++++++++
	GLfloat tableVertices[] = {
		// positions			// Normals			texture
		-2.0f,  -1.0f, -2.0f,	0.0f, -1.0f, 0.0f,	0.0f, 0.0f,
		 2.0f,  -1.0f, -2.0f,	0.0f, -1.0f, 0.0f,	1.0f, 0.0f,
		 2.0f,  -1.0f,  2.0f,	0.0f, -1.0f, 0.0f,	1.0f, 1.0f,
		 2.0f,  -1.0f,  2.0f,	0.0f, -1.0f, 0.0f,	1.0f, 0.0f,
		-2.0f,  -1.0f,  2.0f,	0.0f, -1.0f, 0.0f,	0.0f, 0.0f,
		-2.0f,  -1.0f, -2.0f,	0.0f, -1.0f, 0.0f,	0.0f, 1.0f,
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
		// positions               Normals           texture
		 0.5f,  0.5f, 0.0f,   0.0f, 0.0f, -1.0f,   1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 0.0f, -1.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, -1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   0.0f, 0.0f, -1.0f,   0.0f, 1.0f  // top left 
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

	//______________________________________________________________________________________________________________________________
	//Rubik's Cube container setup
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);

	//Stores vertices in VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	//configures so the OpenGl knows how to use the VBO, position attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	//________________________________________________________________________________________________________________________________
	//Countertop container setup
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);

	//Stores vertices in VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(tableVertices), tableVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tableIndices), tableIndices, GL_STATIC_DRAW);

	//configures so the OpenGl knows how to use the VBO, position attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

//______________________________________________________________________________________________________________
	//Computer container setup
	glBindVertexArray(VAOs[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);

	//Stores vertices in VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tableIndices), tableIndices, GL_STATIC_DRAW);

	//configures so the OpenGl knows how to use the VBO, position attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
//___________________________________________________________________________________________________________________

	unsigned int texture1, texture2, texture3, texture4, texture5;

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



	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	// be sure to activate shader when setting uniforms/drawing objects

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

		//swapped to Depth and color to prevent any issues with glClear
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		//activates shader when setting uniforms/drawing objects
		lightingShader.use();
		lightingShader.setVec3("viewPos", camera.Position);
		lightingShader.setFloat("material.shininess", 2.0f);


		// Fixed light 1
		lightingShader.setVec3("pointLights[0].position", pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		lightingShader.setVec3("pointLights[0].ambient", pointLightColors[0].x * 1.0, pointLightColors[0].y * 1.0, pointLightColors[0].z * 1.0); //sets color intensity to 100%
		lightingShader.setVec3("pointLights[0].diffuse", pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
		lightingShader.setVec3("pointLights[0].specular", pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
		lightingShader.setFloat("pointLights[0].constant", 1.0f);
		lightingShader.setFloat("pointLights[0].linear", 0.09f);
		lightingShader.setFloat("pointLights[0].quadratic", 0.032f);

		// Fix light 2
		lightingShader.setVec3("pointLights[1].position", pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		lightingShader.setVec3("pointLights[1].ambient", pointLightColors[1].x * 0.1, pointLightColors[1].y * 0.1, pointLightColors[1].z * 0.1); //sets color intesity to 10%
		lightingShader.setVec3("pointLights[1].diffuse", pointLightColors[1].x, pointLightColors[1].y, pointLightColors[1].z);
		lightingShader.setVec3("pointLights[1].specular", pointLightColors[1].x, pointLightColors[1].y, pointLightColors[1].z);
		lightingShader.setFloat("pointLights[1].constant", 1.0f);
		lightingShader.setFloat("pointLights[1].linear", 0.09f);
		lightingShader.setFloat("pointLights[1].quadratic", 0.032f);
		std::cout << glGetError() << std::endl; // returns 0 (no error)

		// Fixed light 3
		lightingShader.setVec3("pointLights[2].position", pointLightPositions[2].x, pointLightPositions[0].y, pointLightPositions[0].z);
		lightingShader.setVec3("pointLights[2].ambient", pointLightColors[2].x * 0.1, pointLightColors[0].y * 0.1, pointLightColors[0].z * 0.1); //sets color intensity to 10%
		lightingShader.setVec3("pointLights[2].diffuse", pointLightColors[2].x, pointLightColors[0].y, pointLightColors[0].z);
		lightingShader.setVec3("pointLights[2].specular", pointLightColors[2].x, pointLightColors[0].y, pointLightColors[0].z);
		lightingShader.setFloat("pointLights[2].constant", 1.0f);
		lightingShader.setFloat("pointLights[2].linear", 0.09f);
		lightingShader.setFloat("pointLights[2].quadratic", 0.032f);

		// Fixed light 4
		lightingShader.setVec3("pointLights[3].position", pointLightPositions[3].x, pointLightPositions[1].y, pointLightPositions[1].z);
		lightingShader.setVec3("pointLights[3].ambient", pointLightColors[3].x * 1.0, pointLightColors[1].y * 1.0, pointLightColors[1].z * 1.0); //sets color intesity to 100%
		lightingShader.setVec3("pointLights[3].diffuse", pointLightColors[3].x, pointLightColors[1].y, pointLightColors[1].z);
		lightingShader.setVec3("pointLights[3].specular", pointLightColors[3].x, pointLightColors[1].y, pointLightColors[1].z);
		lightingShader.setFloat("pointLights[3].constant", 1.0f);
		lightingShader.setFloat("pointLights[3].linear", 0.09f);
		lightingShader.setFloat("pointLights[3].quadratic", 0.032f);
		std::cout << glGetError() << std::endl; // returns 0 (no error)

		//initializes a projection matrix (needed to be added after moving projectiosn to if statement
		glm::mat4 projection;

		// pass projection matrix to shader (note that in this case it could change every frame)
		//Referenced from https://stackoverflow.com/questions/44710262/opengl-switching-between-ortho-and-perspecive-problems
		if (!perspective)
		{
			//orth perspective
			projection = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, 0.1f, 100.0f);
		}
		//projection perspective
		else
			projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

		// camera/view transformation
		glm::mat4 view = camera.GetViewMatrix();
		lightingShader.setMat4("view", view);
		lightingShader.setMat4("projection", projection);

		std::cout << glGetError() << std::endl; // returns 0 (no error)

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

			//Adds lighting shader to the 3D model
			lightingShader.setMat4("model", model);

			//Draws the cube using the Draw Array
			glDrawArrays(GL_TRIANGLES, 0, 36);
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

			//Adds lighting shader to the 3D model
			lightingShader.setMat4("model", model);

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
			model = glm::translate(model, glm::vec3(1.0f, -2.75f, -13.5f));

			//Rotates the objects over the degees and x, y, z axis
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0, 0.0f, 0.0f));

			//changes the size of the object
			model = glm::scale(model, glm::vec3(6.0f, 5.0f, 5.0f));

			//Adds lighting shader to the 3D model
			lightingShader.setMat4("model", model);

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
			glBindTexture(GL_TEXTURE_2D, texture4); //screen picture
			bool multipleTextures = false;

			//initializes matrix to identity matrix
			glm::mat4 model = glm::mat4(1.0f);

			//moves the 3D object around the world
			model = glm::translate(model, glm::vec3(1.0f, -1.72f, -15.99f));

			//Rotates the objects over the degees and x, y, z axis
			model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0, 0.0f, 0.0f));

			//changes the size of the object
			model = glm::scale(model, glm::vec3(6.0f, 5.0f, 5.0f));

			//Adds lighting shader to the 3D model
			lightingShader.setMat4("model", model);

			//draws the triangles using the Elemental buffer
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
			model = glm::translate(model, glm::vec3(1.0f, -5.3f, -19.53f));

			//Rotates and flips the object over the degees and x, y, z axis
			model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0, 1.0f, 0.0f));

			//changes the size of the object
			model = glm::scale(model, glm::vec3(6.0f, 5.0f, 5.0f));

			//Adds lighting shader to the 3D model
			lightingShader.setMat4("model", model);

			//draws the triangles using the Elemental buffer
			glDrawElements(GL_TRIANGLES, sizeof(computerIndices) / sizeof(int), GL_UNSIGNED_INT, 0);
		}
		std::cout << glGetError() << std::endl;

		// also draw the lamp object
		lightCubeShader.use();
		lightCubeShader.setMat4("projection", projection);
		lightCubeShader.setMat4("view", view);

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
	//Controls ability to ecape the window
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	//Controls camera movement
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
