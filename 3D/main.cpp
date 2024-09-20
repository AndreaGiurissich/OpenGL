#define GLAD_GL_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include "GLCommon.h"
#include <iostream>
#include "Texture.h"
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

void processInput(GLFWwindow *window);
int numSquares = 1;

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
	GLFWwindow* window = glfwCreateWindow(1080, 1080, "Texture? Va beneeee....", NULL, NULL);
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
	float vertices1[] =
	{ // positions          // colors           // texture coords
		1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};
	unsigned int indices1[] = {  //Indica in che ordine reinderizzare i vertici
	0, 1, 3,   // first triangle
	1, 2, 3,    // second triangle
	};


	//Creiamo uno shader program
	Shader program1 = Shader("Shader.vert", "Shader.frag");

	//Creiamo una texture
	Texture texture1 = Texture("chess.jpg",GL_REPEAT, GL_REPEAT,GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_RGB);

	VAO VAO1 = VAO();
	VAO1.Bind();

	VBO VBO1(vertices1, sizeof(vertices1));
	EBO EBO1(indices1, sizeof(indices1));

	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float),(void*)( 3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	texture1.texUnit(program1, "texture1", 0);
	

	//Ciclo di rendering
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.0f, 0.1f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		texture1.Bind();


		program1.UseProgram();
		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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