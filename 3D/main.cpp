#define GLAD_GL_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include "GLCommon.h"

#include <map>

#include <assimp/Importer.hpp>
#include "rapidxml.hpp"
#include "rapidxml_iterators.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"

#include "Camera.h"
#include "Mesh.h"
#include "Model.h"
#include"Skybox.h"

int samples = 8;

//caricamento modelli 3d
void modelsLoading(Shader shaderProgram, std::vector<Model> uniqueModels, std::vector<std::pair<int, Mat4>> instances);

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

	//Bounding box da XML
	float xmax = stof(root_node->first_node("BoundingBox")->first_node("xmax")->value());
	float xmin = stof(root_node->first_node("BoundingBox")->first_node("xmin")->value());
	float zmax = stof(root_node->first_node("BoundingBox")->first_node("zmax")->value());
	float zmin = stof(root_node->first_node("BoundingBox")->first_node("zmin")->value());


	Camera camera(SCR_WIDTH, SCR_HEIGHT, Vec3(1.0f, 0.0f, 0.0f));

	//-----------------------------AVVIO-----------------------------
	glfwInit(); //avvia glfw

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //versione max che può usare
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);//versione min che può usare
	glfwWindowHint(GLFW_SAMPLES, samples); //attiva multisampling
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //profilo da utilizzare, pacchetto di funzioni composto da core (funzioni moderne), compatibility(outdated)

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "TEST", NULL, NULL);
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

	glEnable(GL_MULTISAMPLE);

	// Strutture per contenere i modelli e le istanze
	std::vector<Model> uniqueModels; //Contiene solo i modelli unici
	std::map<std::string, int> modelPathMap; // Dizionario per mappare i path dei modelli con i loro indici (nel vettore uniqueModels)
	std::vector<std::pair<int, Mat4>> instances; // Stora le istanze dei modelli in un paio ([indice in uniqueModels, Mat trasform dell'istanza])

	for (rapidxml::xml_node<>* model_node = root_node->first_node("model"); model_node; model_node = model_node->next_sibling("model"))
	{
		std::string path = model_node->first_node("path")->value();
		auto it = modelPathMap.find(path); //iteratore per la mappa
		int modelIndex;

		if (it != modelPathMap.end()) {
			//Se iteratore si ferma prima della fine, allora modello già caricato usa indice esistente
			modelIndex = it->second;
		}
		else {
			// Carica nuovo modello e storalo in uniqueModels
			Model newModel(path, model_node->first_node("flipUVs")->value());
			uniqueModels.push_back(newModel);
			modelIndex = uniqueModels.size() - 1;
			modelPathMap[path] = modelIndex; // Aggiungi il path del modello alla mappa e associa l'indice
		}
		// Crea la model matrix
		Mat4 model = Mat4();

		float tx = stof(model_node->first_node("translatex")->value());
		float ty = stof(model_node->first_node("translatey")->value());
		float tz = stof(model_node->first_node("translatez")->value());

		float sx = stof(model_node->first_node("scalex")->value());
		float sy = stof(model_node->first_node("scaley")->value());
		float sz = stof(model_node->first_node("scalez")->value());

		float gradi = stof(model_node->first_node("gradi")->value());
		float rx = stof(model_node->first_node("rotatex")->value());
		float ry = stof(model_node->first_node("rotatey")->value());
		float rz = stof(model_node->first_node("rotatez")->value());

		model = model.translation(Vec3(tx, ty, tz));
		model = model.scale(Vec3(sx, sy, sz));
		model = model.rotation(gradi, Vec3(rx, ry, rz));

		// Stora la coppia indice modello e matrice trasformazione
		instances.emplace_back(modelIndex, model);
	}


	//Creiamo uno shader program
	Shader program1 = Shader("sbus.vert", "sbus.frag");

	Shader skyboxprogram = Shader("skybox.vert", "skybox.frag");


	Vec4 lightColor =Vec4(1.0f, 1.0f, 1.0f, 1.0f);
	Vec3 lightPos = Vec3(0.1f, 0.5f, 0.1f);

	program1.UseProgram();
	int shaderUniformLoc5 = glGetUniformLocation(program1.ID, "lightPos");
	glUniform3f(shaderUniformLoc5, lightPos.x, lightPos.y, lightPos.z);
	int shaderUniformLoc6 = glGetUniformLocation(program1.ID, "lightColor");
	glUniform4f(shaderUniformLoc6, lightColor.x, lightColor.y, lightColor.z, lightColor.w);

	skyboxprogram.UseProgram();
	glUniform1i(glGetUniformLocation(skyboxprogram.ID, "skybox"), 0);

	std::string faces[6]{
		"skybox/right.bmp",
		"skybox/left.bmp",
		"skybox/top.bmp",
		"skybox/bottom.bmp",
		"skybox/front.bmp",
		"skybox/back.bmp"
	};

	Skybox DaySky = Skybox(faces);

	 //---------------------------SHADOWMAPS-------------------------------------//

	//Creiamo un Framebuffer Object per la depth map
	GLuint depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);

	//creiamo una texture che diventerà la depthmap:
	unsigned int shadowWidth = 4096, shadowHeight = 4096;

	GLuint depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	float clampColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clampColor);

	//Bindiamo la texture al framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	

	Shader shadowMapProgram = Shader("shadowMap.vert", "shadowMap.frag");
	Mat4 ortho = Mat4().ortho(-4.0f, 4.0f, -4.0, 4.0, 1.5f, 5.5f);
	std::cout << ortho << std::endl;

	//-----------------------------QUAD per DEPTHMAP----------------------------------//
	
	// Vertici e indici per il quadrato per debugging
	float quadVertices[] = {
		// Positions    // TexCoords
		-1.0f, -1.0f,  0.0f, 0.0f,
		 0.0f, -1.0f,  1.0f, 0.0f,
		 0.0f,  0.0f,  1.0f, 1.0f,
		-1.0f,  0.0f,  0.0f, 1.0f
	};
	unsigned int quadIndices[] = { 0, 1, 2, 0, 2, 3 };

	GLuint quadVAO, quadVBO, quadEBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glGenBuffers(1, &quadEBO);

	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

	// Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	// Texture coordinate attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glBindVertexArray(0);

	// Create quad shader
	Shader quadShader("debug.vert", "debug.frag");

	//-------------------------------------------------------------------------//



	// Variabili per il calcolo del tempo
	double prevTime = 0.0;
	double crntTime = 0.0;
	double currentTick = 0.0;
	double timeDiff;
	// Keeps track of the amount of frames in timeDiff
	unsigned int counter = 0;
	float angle = 0.0f;


	glfwSwapInterval(1);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); wireframe mode
	

	//----------CICLO DI RENDERING----------
	while (!glfwWindowShouldClose(window))
	{
		// Updates counter and times
		crntTime = glfwGetTime();
		timeDiff = crntTime - prevTime;

		currentTick += timeDiff * 240.0f; //Accumula il tempo trascorso con un fattore di moltiplicazione (240)
		float timeOfDay = fmod(currentTick, 24000.0f) / 24000.0f; //restituisce il resto della divisione tra currentTick e 24000 (rappresentante un ciclo giorno/notte completo)
		angle = timeOfDay * 360.0f;
		angle -= 70.0f;

		//stampa angolo
		//std::cout << angle << std::endl;

		counter++;
		
		if (timeDiff >= 1.0 / 60.0)
		{
			// Nuovo titolo
			std::string FPS = std::to_string((1.0 / timeDiff) * counter);
			std::string ms = std::to_string((timeDiff / counter) * 1000);
			std::string newTitle = "Sponza - " + FPS + "FPS / " + ms + "ms";
			glfwSetWindowTitle(window, newTitle.c_str());

			// Resets times and counter
			prevTime = crntTime;
			counter = 0;
		}

		Mat3 rotation = Mat3().rotation(angle, Vec3(0.0f, 1.0f, 0.0f));
		Vec3 lightDirection = rotation * (lightPos * -7.0f);
		Mat4 lightView = Mat4().lookAt(lightDirection, Vec3(0.0f), Vec3(0.0f, 1.0f, 0.0f));
		Mat4 lightProjection = ortho.tras() * lightView;


		glClearColor(0.25f, 0.25f, 0.50f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.inputs(window, xmin, xmax, zmin, zmax);
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		std::cout << camera.Position << std::endl;

		//SKYBOX
		glDepthFunc(GL_LEQUAL);

		skyboxprogram.UseProgram();
		Mat4 view = Mat4();
		Mat4 projection = Mat4();
		Mat4 rotationM = Mat4();
		view = view.lookAtCubemap(camera.Position, camera.Position + camera.Orientation, camera.Up);
		projection = projection.perspective(120.0, (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);

		glUniformMatrix4fv(glGetUniformLocation(skyboxprogram.ID, "view"), 1, GL_TRUE, view.value_ptr());
		glUniformMatrix4fv(glGetUniformLocation(skyboxprogram.ID, "projection"), 1, GL_FALSE, projection.value_ptr());
		glUniformMatrix4fv(glGetUniformLocation(skyboxprogram.ID, "rotation"), 1, GL_FALSE, rotationM.value_ptr());
		glUniform1f(glGetUniformLocation(skyboxprogram.ID, "angle"), angle);

		DaySky.BindVAO();

		glDepthFunc(GL_LESS);
		//-----------------------------

		glCullFace(GL_FRONT);
		shadowMapProgram.UseProgram();

		GLuint lightProjectionLoc = glGetUniformLocation(shadowMapProgram.ID, "lightProjection");
		glUniformMatrix4fv(lightProjectionLoc, 1, GL_FALSE, lightProjection.value_ptr());
		// Setta la lightProjection uniform che cambia in base all'angolo

		glViewport(0, 0, shadowWidth, shadowHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		// Renderizza tutti i modellli per creare la shadow map
		for (const auto& instance : instances) {
			int modelIndex = instance.first;
			const Mat4& modelMatrix = instance.second;
			glUniformMatrix4fv(glGetUniformLocation(shadowMapProgram.ID, "model"), 1, GL_TRUE, modelMatrix.value_ptr());
			uniqueModels[modelIndex].Draw(shadowMapProgram);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glCullFace(GL_BACK);
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT); // Resetta il viewport per il rendering normale


		// Render modelli
		program1.UseProgram();
		glUniform3f(glGetUniformLocation(program1.ID, "lightDirection"), lightDirection.x, lightDirection.y, lightDirection.z);
		glUniformMatrix4fv(glGetUniformLocation(program1.ID, "lightProjection"), 1, GL_FALSE, lightProjection.value_ptr());

		// Passiamo l'angolo allo shader
		glUniform1f(glGetUniformLocation(program1.ID, "angle"), angle);

		glActiveTexture(GL_TEXTURE0 + 3);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glUniform1i(glGetUniformLocation(program1.ID, "shadowMap"), 3);

		int shaderUniformLoc7 = glGetUniformLocation(program1.ID, "lightColor");
		glUniform4f(shaderUniformLoc7, lightColor.x, lightColor.y, lightColor.z, lightColor.w);
		int shaderUniformLoc4 = glGetUniformLocation(program1.ID, "camPos");
		glUniform3f(shaderUniformLoc4, camera.Position.x, camera.Position.y, camera.Position.z);

		camera.Matrix(program1, "camMatrix");

		modelsLoading(program1, uniqueModels, instances);

		glfwSwapBuffers(window);

		glfwPollEvents(); //processare tutti gli eventi raggruppati, come visualizzazione finestra, ridimensionamento ecc., se non si elaborano la finestra non risponde

	}

	// Canvella i programmi
	program1.Delete();
	shadowMapProgram.Delete();
	skyboxprogram.Delete();

	// Cancella la finestra e termina GLFW prima di chiudere il programma
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void modelsLoading(Shader shaderProgram, std::vector<Model> uniqueModels, std::vector<std::pair<int, Mat4>> instances)
{
	for (const auto& instance : instances) {
		int modelIndex = instance.first;
		const Mat4& modelMatrix = instance.second;

		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_TRUE, modelMatrix.value_ptr());
		uniqueModels[modelIndex].Draw(shaderProgram);
	}
}