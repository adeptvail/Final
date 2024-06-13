#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "shaderClass.h"
#include "Texture.h"
#include "Camera.h"

// Vertices coordinates
GLfloat vertices[] =
{ //     COORDINATES     /    TexCoord    /       NORMALS     //
	-1.0f, 0.0f,  1.0f,		0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
	-1.0f, 0.0f, -1.0f,		0.0f, 1.0f,		0.0f, 1.0f, 0.0f,
	 1.0f, 0.0f, -1.0f,		1.0f, 1.0f,		0.0f, 1.0f, 0.0f,
	 1.0f, 0.0f,  1.0f,		1.0f, 0.0f,		0.0f, 1.0f, 0.0f
};

// Indices for vertices order
GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3
};

GLfloat lightVertices[] =
{ //     COORDINATES     //
	-0.1f, -0.1f,  0.1f,
	-0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f,  0.1f,
	-0.1f,  0.1f,  0.1f,
	-0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f,  0.1f
};

GLuint lightIndices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
};


unsigned int width = 800;
unsigned int height = 800;

int main() {

	//initiate glfw
	glfwInit();

	//tell glfw version it should be using and what profile to use
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//create the window object
	GLFWwindow* window = glfwCreateWindow(width, height, "Final Project :3 uwu >w< teehee yipppe", NULL, NULL);
	if (window == NULL) {
		std::cout << "window failed to initialize :p";
		glfwTerminate();
		return -1;
	}

	//set the context to the window I created
	glfwMakeContextCurrent(window);

	//load glad
	gladLoadGL();

	//set viewport sizing from bottom left of the screen to top right
	glViewport(0, 0, width, height);

	//create shader program with the default shaders
	Shader shaderProgram("default.vert", "default.frag");

	//create VAO and bind it
	VAO VAO1;
	VAO1.Bind();

	//Create Vertex Buffer Array linked to verticies
	VBO VBO1(vertices, sizeof(vertices));
	//Create Element Bufffer Array linked to indicies
	EBO EBO1(indices, sizeof(indices));

	//Link VAO to the VBO
	//specify the new 
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 3, GL_FLOAT, 8 * sizeof(float), (void*)(5 * sizeof(float)));

	//Unbind all to prevent accidental modification
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	//Lighting code

	//create new shader for the lighting shaders
	Shader lightShader("light.vert", "light.frag");
	//create new vertex array object for light source
	VAO lightVAO;
	lightVAO.Bind();
	//create VBO and EBO
	VBO lightVBO(lightVertices, sizeof(lightVertices));
	EBO lightEBO(lightIndices, sizeof(lightIndices));
	//set the way the VAO reads the given information
	lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	//unbind all so I dont misuse it somewhere
	lightVAO.Unbind();
	lightVBO.Unbind();
	lightEBO.Unbind();

	//set all light identification
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPOS = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	//move the light object to the correct position
	lightModel = glm::translate(lightModel,lightPOS);

	//create all pyramid attributes
	glm::vec3 pyramidPOS = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 pyramidModel = glm::mat4(1.0f);
	//move it to correct position
	pyramidModel = glm::translate(pyramidModel, pyramidPOS);

	//send all of the necessary data to the shaders
	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPOS.x,lightPOS.y,lightPOS.z);

	//Texture
	Texture planksTex("planks.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE);
	planksTex.texUnit(shaderProgram, "tex0", 0);
	Texture planksSpec("planksSpec.png", GL_TEXTURE_2D, 1, GL_RED, GL_UNSIGNED_BYTE);
	planksSpec.texUnit(shaderProgram, "tex1", 1);

	//enable depth buffer
	glEnable(GL_DEPTH_TEST);

	//create a new camera witht the desired attributes
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	//process all events during the windows lifespan
	while (!glfwWindowShouldClose(window)) {

		//specify clear color
		glClearColor(0.07f, 0.13f, 0.32f, 1.0f);
		//clear the color buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//register the inputs 
		camera.Inputs(window);
		//give the camera object the desired matrixs
		camera.UpdateMatrix(45.0f, 0.1f, 100.0f);
		//set the camera posititon in the shader and set the camera matrix
		shaderProgram.Activate();
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		camera.Matrix(shaderProgram, "camMatrix");
		//bind the planksTex texture
		planksTex.Bind();
		planksSpec.Bind();
		//bind the VAO for openGL
		VAO1.Bind();
		//draw the triangles using the triangles primative
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);

		//activate lightshader object and set the uniform
		lightShader.Activate();
		camera.Matrix(lightShader, "camMatrix");
		//bind and draw triangles to the screen
		lightVAO.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);
		//swap the front/back buffers
		glfwSwapBuffers(window);

		//make sure all events are put to bed and taken care of for the night
		glfwPollEvents();
	}

	//Delete all Objects created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();
	planksTex.Delete();

	//make sure nothing shitty goes on when the application ends
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}