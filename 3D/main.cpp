#define GLAD_GL_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include "GLCommon.h"
#include <iostream>
#include "Algebra_Lineare.h"
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
	{ //     COORDINATES     /        COLORS        /    TexCoord    /       NORMALS     //
		-1.0f, 0.0f,  1.0f,		0.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
		-1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 0.0f,		0.0f, 1.0f,		0.0f, 1.0f, 0.0f,
		 1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 0.0f,		1.0f, 1.0f,		0.0f, 1.0f, 0.0f,
		 1.0f, 0.0f,  1.0f,		0.0f, 0.0f, 0.0f,		1.0f, 0.0f,		0.0f, 1.0f, 0.0f
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


	//Creiamo uno shader program
	Shader program1 = Shader("sbus.vert", "sbus.frag");

	VAO VAO1 = VAO();
	VAO1.Bind();

	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));

	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	//Shader per il cubo luce
	Shader lightShader = Shader("light.vert", "light.frag");

	VAO lightVAO = VAO();
	lightVAO.Bind();

	VBO lightVBO(lightVertices, sizeof(lightVertices));
	EBO lightEBO(lightIndices, sizeof(lightIndices));

	lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	lightVAO.Unbind();
	lightVBO.Unbind();

	Vec4 lightColor =Vec4(1.0f, 1.0f, 1.0f, 1.0f);
	Vec3 lightPos = Vec3(1.0f, 1.0f, 1.0f);
	Mat4 lightModel =Mat4();
	lightModel = lightModel.translation(lightPos);

	Vec3 pyramidPos = Vec3(0.0f, 0.0f, 0.0f);
	Mat4 pyramidModel = Mat4();
	pyramidModel = pyramidModel.translation(pyramidPos);

	lightShader.UseProgram();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_TRUE, lightModel.value_ptr());
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	program1.UseProgram();
	glUniformMatrix4fv(glGetUniformLocation(program1.ID, "model"), 1, GL_TRUE, pyramidModel.value_ptr());
	glUniform4f(glGetUniformLocation(program1.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(program1.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	Texture planksTex("wood_floor_diff_4k.jpg", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_RGB, 0);
	planksTex.texUnit(program1, "tex0", 0);
	Texture planksSpec("wood_floor_ao_4k.jpg", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_RED, 1);
	planksSpec.texUnit(program1, "tex1", 1);
	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);
	// Creates camera object
	Camera camera(width, height, Vec3(0.0f, 0.0f, 2.0f));

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//Ciclo di rendering
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.0f, 0.1f, 0.2f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Handles camera inputs
		camera.inputs(window);
		// Updates and exports the camera matrix to the Vertex Shader
		camera.updateMatrix(90.0f, 0.1f, 100.0f);


		// Tells OpenGL which Shader Program we want to use
		program1.UseProgram();
		// Exports the camera Position to the Fragment Shader for specular lighting
		glUniform3f(glGetUniformLocation(program1.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		// Export the camMatrix to the Vertex Shader of the pyramid
		camera.Matrix(program1, "camMatrix");
		// Binds texture so that is appears in rendering
		planksTex.Bind();
		planksSpec.Bind();
		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);



		// Tells OpenGL which Shader Program we want to use
		lightShader.UseProgram();
		// Export the camMatrix to the Vertex Shader of the light cube
		camera.Matrix(lightShader, "camMatrix");
		// Bind the VAO so OpenGL knows to use it
		lightVAO.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);


		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}



	// Delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	program1.Delete();
	lightVAO.Delete();
	lightVBO.Delete();
	lightEBO.Delete();
	lightShader.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
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