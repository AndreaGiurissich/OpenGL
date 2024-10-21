#define GLAD_GL_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include "GLCommon.h"

#include <assimp/Importer.hpp>
#include "rapidxml.hpp"
#include "rapidxml_iterators.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"

#include "Camera.h"
#include "Mesh.h"
#include "Model.h"

//void processInput(GLFWwindow *window);
//const unsigned int width = 1000;
//const unsigned int height = 1000;

//Vertex vertices[] =
//{ //               COORDINATES           /            COLORS          /           NORMALS         /       TEXTURE COORDINATES    //
//	Vertex{Vec3(-1.0f, 0.0f,  1.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), Vec2(0.0f, 0.0f)},
//	Vertex{Vec3(-1.0f, 0.0f, -1.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), Vec2(0.0f, 1.0f)},
//	Vertex{Vec3(1.0f, 0.0f, -1.0f),  Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), Vec2(1.0f, 1.0f)},
//	Vertex{Vec3(1.0f, 0.0f,  1.0f),  Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), Vec2(1.0f, 0.0f)}
//};
//
//// Indices for vertices order
//GLuint indices[] =
//{
//	0, 1, 2,
//	0, 2, 3
//};

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

//caricamento modelli 3d
void modelsLoading(float modelsDimension, rapidxml::xml_node<>* root_node, /*string stencil,*/ Shader shaderProgram, vector<Mat4>& modelsMat, vector<Model>& models)
{
	int modelCount = 0;
	for (rapidxml::xml_node<>* model_node = root_node->first_node("model"); model_node; model_node = model_node->next_sibling("model"))
	{
		//if (stencil == model_node->first_node("stencil")->value()) // caricamento dei modelli a seconda del parametro stencil tramite xml
		//{
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_TRUE, modelsMat[modelCount].value_ptr());

			models[modelCount].Draw(shaderProgram);

			modelCount++;

	}
}

int main()
	{
	// Crea un documento RapidXML
	rapidxml::xml_document<> doc;
	// legge file xml in un vector
	ifstream file("configurazione.xml");

	// Inseriamo nel vector qui di seguito i char partendo da:
	//				   (iteratore dello stream dal file,    fino all'iteratore che indica l'eof)
	vector<char> buffer((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
	buffer.push_back('\0');
	// Usa l'identificatore per specificare come analizzare il documento
	doc.parse<0>(&buffer[0]);
	rapidxml::xml_node<>* root_node = doc.first_node("Root");

	const unsigned int SCR_WIDTH = atoi(root_node->first_node("window")->first_node("width")->value());
	const unsigned int SCR_HEIGHT = atoi(root_node->first_node("window")->first_node("height")->value());

	Camera camera(SCR_WIDTH, SCR_HEIGHT, Vec3(0.0f, 3.0f, 0.0f));

	//-----------------------------AVVIO-----------------------------
	glfwInit(); //avvia glfw

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //versione max che può usare
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0); //versione min che può usare
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //profilo da utilizzare, pacchetto di funzioni composto da core (funzioni moderne), compatibility(outdated)

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Texture? Va beneeee....", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Errore creazione finestra" << std::endl;
		glfwTerminate();
		return -1;
	}
	//Tutte le chiamate openGl si rifletteranno sulla finestra window(il contesto corrente)
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	int version = gladLoadGL(glfwGetProcAddress);
	printf("GL %d %d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);
	//glEnable(GL_STENCIL_TEST);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CCW);
	


	//Modelli da XML

	int modelsDimension = 0;
	for (rapidxml::xml_node<>* model_node = root_node->first_node("model"); model_node; model_node = model_node->next_sibling("model"))
	{
		modelsDimension++;
	}

	vector<Model> models(modelsDimension);
	vector<Mat4> modelsMat(modelsDimension);

	int modelsCount = 0;

	for (rapidxml::xml_node<>* model_node = root_node->first_node("model"); model_node; model_node = model_node->next_sibling("model"))
	{
		models[modelsCount] = Model((model_node->first_node("path")->value()), false);

		cout << "obj" << modelsCount << endl << model_node->first_node("path")->value() << endl;

		Mat4 model = Mat4(1.0f);

		//translate da xml
		float tx = stof(model_node->first_node("translatex")->value());
		float ty = stof(model_node->first_node("translatey")->value());
		float tz = stof(model_node->first_node("translatez")->value());

		//scale da xml
		float sx = stof(model_node->first_node("scalex")->value());
		float sy = stof(model_node->first_node("scaley")->value());
		float sz = stof(model_node->first_node("scalez")->value());

		//rotation da xml
		float gradi = stof(model_node->first_node("gradi")->value());
		float rx = stof(model_node->first_node("rotatex")->value());
		float ry = stof(model_node->first_node("rotatey")->value());
		float rz = stof(model_node->first_node("rotatez")->value());

		model = model.translation(Vec3(tx, ty, tz));
		model = model.scale(Vec3(sx, sy, sz));
		model = model.rotation(gradi, Vec3(rx, ry, rz));
		model = model.tras();

		modelsMat[modelsCount] = model;
		modelsCount++;
	}

	//Creiamo uno shader program
	Shader program1 = Shader("sbus.vert", "sbus.frag");


	//Shader per il cubo luce
	Shader lightShader = Shader("light.vert", "light.frag");


	Vec4 lightColor =Vec4(1.0f, 1.0f, 1.0f, 1.0f);
	Vec3 lightPos = Vec3(0.5f, 0.5f, 0.5f);
	Mat4 lightModel =Mat4();
	lightModel = lightModel.translation(lightPos);
	lightShader.UseProgram();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_TRUE, lightModel.value_ptr());
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);


	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glfwSwapInterval(1);
	//Ciclo di rendering
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.25f, 0.25f, 0.50f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glStencilFunc(GL_NEVER, 1, 0xFF);
		//glStencilOp(GL_REPLACE, GL_REPLACE, GL_KEEP); // stencil no, stencil ok - depth no, stencil ok - depth ok
		//glStencilMask(0xFF);
		glClear(GL_STENCIL_BUFFER_BIT);

		camera.inputs(window);
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		program1.UseProgram(); //quale shaderprogram usare

		int shaderUniformLoc4 = glGetUniformLocation(program1.ID, "camPos");
		glUniform3f(shaderUniformLoc4, camera.Position.x, camera.Position.y, camera.Position.z);

		camera.Matrix(program1, "camMatrix");

		//glStencilMask(0xFF); 

		modelsLoading(modelsDimension, root_node, program1, modelsMat, models);

		//glStencilFunc(GL_EQUAL, 1, 0x00);

		glfwSwapBuffers(window);

		glfwPollEvents(); //processare tutti gli eventi raggruppati, come visualizzazione finestra, ridimensionamento ecc., se non si elaborano la finestra non risponde

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
