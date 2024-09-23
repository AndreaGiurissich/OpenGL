#define GLAD_GL_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include "GLCommon.h"
#include <iostream>

#include "Camera.h"
#include "Texture.h"
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

void processInput(GLFWwindow *window);
int numSquares = 1;

const unsigned int width = 800;
const unsigned int height = 800;

int main()
{
	glfwInit();

	//Fino a che versione di opengl devi essere compatibile?
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//Che profilo di openGl usiamo?
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Crea finestra x*y con titolo
	//Crea il frame buffer, 
	GLFWwindow* window = glfwCreateWindow(width, height, "Texture? Va beneeee....", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Errore creazione finestra" << std::endl;
		glfwTerminate();
		return -1;
	}
	//Tutte le chiamate openGl si rifletteranno sulla finestra window(il contesto corrente)
	glfwMakeContextCurrent(window);

	int version = gladLoadGL(glfwGetProcAddress);
	printf("GL %d %d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

	//Creiamo i vertici del quadrato

	// Vertices coordinates
	GLfloat vertices[] =
	{ //     COORDINATES     /        COLORS      /   TexCoord  //
		-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
		 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
		 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
		 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	2.5f, 5.0f
	};

	// Indices for vertices order
	GLuint indices[] =
	{
		0, 1, 2,
		0, 2, 3,
		0, 1, 4,
		1, 2, 4,
		2, 3, 4,
		3, 0, 4
	};

	//Creiamo uno shader program
	Shader program1 = Shader("sbus.vert", "sbus.frag");

	//Creiamo una texture
	Texture texture1 = Texture("chess.jpg",GL_REPEAT, GL_REPEAT,GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_RGB);

	VAO VAO1 = VAO();
	VAO1.Bind();

	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));

	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float),(void*)( 3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	texture1.texUnit(program1, "texture1", 0);


	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Creates camera object
	Camera camera(width, height, Vec3(0.0f, 0.0f, 2.0f));

	camera.Matrix(90.0f, 0.1f, 100.0f, program1, "camMatrix");
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//Ciclo di rendering
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.0f, 0.1f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		
		program1.UseProgram();

		texture1.Bind();

		// Handles camera inputs
		camera.inputs(window);
		// Updates and exports the camera matrix to the Vertex Shader
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, (sizeof(indices))/sizeof(int), GL_UNSIGNED_INT, 0);

		program1.setInt("numSquares", numSquares);

		glfwSwapBuffers(window); //Fammi vedere il render buffer
		//2 frame buffer render and show, 
		glfwPollEvents();// gestisce gli eventi lato finestra
		//Prende i clic sulla finestra e agisce di conseguenza quando viene chiamata
	}

	glfwTerminate();

	return 0;


}


void processInput(GLFWwindow * window)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
			numSquares = 1;

		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
			numSquares = 2;

		if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
			numSquares = 3;

		if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
			numSquares = 4;

		if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
			numSquares = 5;

		if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
			numSquares = 6;

		if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
			numSquares = 7;

		if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
			numSquares = 8;

		if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
			numSquares = 9;
	}