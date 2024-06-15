#include "Mesh.h"

// Vertices coordinates
// Vertices coordinates
Vertex vertices[] =
{ //               COORDINATES           /            COLORS          /           NORMALS         /       TEXTURE COORDINATES    //
	Vertex{glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
};

// Indices for vertices order
GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3
};

Vertex lightVertices[] =
{ //     COORDINATES     //
	Vertex{glm::vec3(-0.1f, -0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f, -0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f,  0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f,  0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f,  0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f,  0.1f,  0.1f)}
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


unsigned int width = 1000;
unsigned int height = 1000;

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

	Texture textures[] = {
		Texture("planks.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture("planksSpec.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE)
	};

	//create shader program with the default shaders
	Shader shaderProgram("default.vert", "default.frag");

	std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
	std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
	Mesh floor(verts, ind, tex);

	//Lighting code

	//create new shader for the lighting shaders
	Shader lightShader("light.vert", "light.frag");
	// Store mesh data in vectors for the mesh
	std::vector <Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
	std::vector <GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
	// Create light mesh
	Mesh light(lightVerts, lightInd, tex);

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
		
		floor.Draw(shaderProgram, camera);
		light.Draw(lightShader, camera);

		//swap the front/back buffers
		glfwSwapBuffers(window);

		//make sure all events are put to bed and taken care of for the night
		glfwPollEvents();
	}

	//Delete all Objects created
	shaderProgram.Delete();
	lightShader.Delete();

	//make sure nothing shitty goes on when the application ends
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}