#define GLAD_GL_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include "GLCommon.h"

#include <assimp/Importer.hpp>

#include "Mesh.h"

//void processInput(GLFWwindow *window);
const unsigned int width = 1000;
const unsigned int height = 1000;

Vertex vertices[] =
{ //               COORDINATES           /            COLORS          /           NORMALS         /       TEXTURE COORDINATES    //
	Vertex{Vec3(-1.0f, 0.0f,  1.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), Vec2(0.0f, 0.0f)},
	Vertex{Vec3(-1.0f, 0.0f, -1.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), Vec2(0.0f, 1.0f)},
	Vertex{Vec3(1.0f, 0.0f, -1.0f),  Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), Vec2(1.0f, 1.0f)},
	Vertex{Vec3(1.0f, 0.0f,  1.0f),  Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), Vec2(1.0f, 0.0f)}
};

// Indices for vertices order
GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3
};

Vertex lightVertices[] =
{ //     COORDINATES     //
	Vertex{Vec3(-0.1f, -0.1f,  0.1f)},
	Vertex{Vec3(-0.1f, -0.1f, -0.1f)},
	Vertex{Vec3(0.1f, -0.1f, -0.1f)},
	Vertex{Vec3(0.1f, -0.1f,  0.1f)},
	Vertex{Vec3(-0.1f,  0.1f,  0.1f)},
	Vertex{Vec3(-0.1f,  0.1f, -0.1f)},
	Vertex{Vec3(0.1f,  0.1f, -0.1f)},
	Vertex{Vec3(0.1f,  0.1f,  0.1f)}
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

	Texture textures[]
	{
		Texture("wood_floor_diff_4k.jpg", "diffuse", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_RGB, 0),
		Texture("wood_floor_disp_4k.png", "specular", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_RED, 1)
	};

	//Creiamo uno shader program
	Shader program1 = Shader("sbus.vert", "sbus.frag");

	std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
	std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
	// Create floor mesh
	Mesh floor(verts, ind, tex);

	//Shader per il cubo luce
	Shader lightShader = Shader("light.vert", "light.frag");

	std::vector <Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
	std::vector <GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));

	Mesh light(lightVerts, lightInd, tex);

	Vec4 lightColor =Vec4(1.0f, 1.0f, 1.0f, 1.0f);
	Vec3 lightPos = Vec3(0.5f, 0.5f, 0.5f);
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

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Creates camera object
	Camera camera(width, height, Vec3(0.0f, 0.0f, 2.0f));

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//Ciclo di rendering
	while (!glfwWindowShouldClose(window))
	{
		//processInput(window);

		glClearColor(0.0f, 0.1f, 0.2f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Handles camera inputs
		camera.inputs(window);
		// Updates and exports the camera matrix to the Vertex Shader
		camera.updateMatrix(90.0f, 0.1f, 100.0f);

		floor.Draw(program1, camera);
		light.Draw(lightShader, camera);

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}

	// Delete all the objects we've created
	program1.Delete();
	lightShader.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}


//void processInput(GLFWwindow * window)
//	{
//		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//			glfwSetWindowShouldClose(window, true);
//
//		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
//			numSquares = 1;
//
//		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
//			numSquares = 2;
//
//		if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
//			numSquares = 3;
//
//		if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
//			numSquares = 4;
//
//		if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
//			numSquares = 5;
//
//		if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
//			numSquares = 6;
//
//		if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
//			numSquares = 7;
//
//		if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
//			numSquares = 8;
//
//		if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
//			numSquares = 9;
//	}