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
void drawFrustum(Mat4 projectionViewMatrix, const Vec4& color, Shader& shader);
Vec4 calculateLightColor(float angle, Vec4 lightColor);

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

	//Collider da XML
	float xmax = stof(root_node->first_node("collider")->first_node("xmax")->value());
	float xmin = stof(root_node->first_node("collider")->first_node("xmin")->value());
	float zmax = stof(root_node->first_node("collider")->first_node("zmax")->value());
	float zmin = stof(root_node->first_node("collider")->first_node("zmin")->value());


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

	// Structures to hold unique models and instances
	std::vector<Model> uniqueModels; //Contiene solo i modelli unici
	std::map<std::string, int> modelPathMap; // Dizionario per mappare i path dei modelli con i loro indici (nel vettore uniqueModels)
	std::vector<std::pair<int, Mat4>> instances; // Stora le istanze dei modelli in un paio ([indice in uniqueModels, Mat trasform dell'istanza])

	for (rapidxml::xml_node<>* model_node = root_node->first_node("model"); model_node; model_node = model_node->next_sibling("model"))
	{
		std::string path = model_node->first_node("path")->value();
		auto it = modelPathMap.find(path);
		int modelIndex;

		if (it != modelPathMap.end()) {
			// Modello già caricato usa indice esistente
			modelIndex = it->second;
		}
		else {
			// Carica nuovo modello e storalo in uniqueModels
			Model newModel(path, model_node->first_node("flipUVs")->value());
			uniqueModels.push_back(newModel);
			modelIndex = uniqueModels.size() - 1;
			modelPathMap[path] = modelIndex;
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


	Shader frustumShader = Shader("frustum.vert", "frustum.frag");

	//---------------------------------------------------------------//
	
	// Vertici e indici per il quadrato
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

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Variables to create periodic event for FPS displaying
	double prevTime = 0.0;
	double crntTime = 0.0;
	double currentTick = 0.0;
	double timeDiff;
	// Keeps track of the amount of frames in timeDiff
	unsigned int counter = 0;
	float angle = -30.0f;
	glfwSwapInterval(1);

	

	//----------CICLO DI RENDERING----------
	while (!glfwWindowShouldClose(window))
	{
		// Updates counter and times
		crntTime = glfwGetTime();
		timeDiff = crntTime - prevTime;

		currentTick += timeDiff * 240.0f;
		float timeOfDay = fmod(currentTick, 24000.0f) / 24000.0f;
		angle = timeOfDay * 360.0f;

		std::cout << angle << std::endl;

		counter++;
		
		if (timeDiff >= 1.0 / 30.0)
		{
			// Creates new title
			std::string FPS = std::to_string((1.0 / timeDiff) * counter);
			std::string ms = std::to_string((timeDiff / counter) * 1000);
			std::string newTitle = "POG - " + FPS + "FPS / " + ms + "ms";
			glfwSetWindowTitle(window, newTitle.c_str());

			// Resets times and counter
			prevTime = crntTime;
			counter = 0;
			// Use this if you have disabled VSync
			//camera.inputs(window);
		}


		//std::cout << ortoProjection << std::endl;

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
		//rotationM = rotationM.rotation(angle, Vec3(0.0f, 0.0f, 1.0f));
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
		// Set lightProjection matrix each frame (in case it changes)

		glViewport(0, 0, shadowWidth, shadowHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		// Render all instances with their model matrices
		for (const auto& instance : instances) {
			int modelIndex = instance.first;
			const Mat4& modelMatrix = instance.second;
			glUniformMatrix4fv(glGetUniformLocation(shadowMapProgram.ID, "model"), 1, GL_TRUE, modelMatrix.value_ptr());
			uniqueModels[modelIndex].Draw(shadowMapProgram);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glCullFace(GL_BACK);
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT); // Reset viewport for main rendering


		program1.UseProgram(); //quale shaderprogram usare
		glUniform3f(glGetUniformLocation(program1.ID, "lightDirection"), lightDirection.x, lightDirection.y, lightDirection.z);
		glUniformMatrix4fv(glGetUniformLocation(program1.ID, "lightProjection"), 1, GL_FALSE, lightProjection.value_ptr());

		// Passiamo the angle directly to the shader instead of calculating light color in C++
		glUniform1f(glGetUniformLocation(program1.ID, "angle"), angle);


		// Bind the Shadow Map
		glActiveTexture(GL_TEXTURE0 + 3);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glUniform1i(glGetUniformLocation(program1.ID, "shadowMap"), 3);

		int shaderUniformLoc7 = glGetUniformLocation(program1.ID, "lightColor");
		glUniform4f(shaderUniformLoc7, lightColor.x, lightColor.y, lightColor.z, lightColor.w);
		int shaderUniformLoc4 = glGetUniformLocation(program1.ID, "camPos");
		glUniform3f(shaderUniformLoc4, camera.Position.x, camera.Position.y, camera.Position.z);

		camera.Matrix(program1, "camMatrix");
	
		////glStencilMask(0xFF); 

		modelsLoading(program1, uniqueModels, instances);

		

		// Passa i parametri della point light allo shader
		////// Draw the light's view frustum (shadow map)
		//frustumShader.UseProgram();
		//camera.Matrix(frustumShader, "camMatrix"); // Set the view-projection matrix
		//Vec4 lightFrustumColor = Vec4(1.0f, 1.0f, 0.0f, 1.0f); // Yellow color for light frustum
		//drawFrustum(lightProjection, lightFrustumColor, frustumShader);

		////// Render shadow map quad
		//glDisable(GL_DEPTH_TEST); // Disable depth test so quad draws on top
		//quadShader.UseProgram();

		//// Bind the depth map texture to texture unit 0
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, depthMap);
		//glUniform1i(glGetUniformLocation(quadShader.ID, "depthMap"), 0);

		//// Draw quad
		//glBindVertexArray(quadVAO);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glBindVertexArray(0);

		//glEnable(GL_DEPTH_TEST); // Re-enable depth test
		////glStencilFunc(GL_EQUAL, 1, 0x00);

		glfwSwapBuffers(window);

		glfwPollEvents(); //processare tutti gli eventi raggruppati, come visualizzazione finestra, ridimensionamento ecc., se non si elaborano la finestra non risponde

	}

	// Delete all the objects we've created
	program1.Delete();

	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
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

// Function to update light color based on angle
Vec4 calculateLightColor(float angle, Vec4 lightColor)
{
	

	if (angle >= 0.0f && angle <= 45.0f) {
		float t = (angle - 0.0f) / 45.0f; // Normalize angle to range [0, 1]
		lightColor = Vec4(1.0f, 1.0f - 0.6 * t , 1.0f - t, 1.0f); // Da bianco a rosso
	}

	if (angle >= 230.0f && angle <= 275.0f) {
		float t = (angle - 230.0f) / 45.0f; // Normalize angle to range [0, 1]
		lightColor = Vec4(1.0f, 0.8 + 0.2 * t, 0.0f + t, 1.0f); //Da rosso a bianco
	}


	return lightColor;
}

struct FrustumVertex {
	Vec3 position;
};

// Function to draw a frustum outline based on a projection-view matrix
void drawFrustum(Mat4 projectionViewMatrix, const Vec4& color, Shader& shader) {
	// Define the 8 corners in normalized device coordinates (-1 to 1 cube)
	// For OpenGL, the NDC has z from -1 (near) to 1 (far)
	Vec4 frustumCornersNDC[8] = {
		// Near plane (z = -1)
		Vec4(-1.0f, -1.0f, -1.0f, 1.0f), // bottom-left-near
		Vec4(1.0f, -1.0f, -1.0f, 1.0f), // bottom-right-near
		Vec4(1.0f,  1.0f, -1.0f, 1.0f), // top-right-near
		Vec4(-1.0f,  1.0f, -1.0f, 1.0f), // top-left-near

		// Far plane (z = 1)
		Vec4(-1.0f, -1.0f,  1.0f, 1.0f), // bottom-left-far
		Vec4(1.0f, -1.0f,  1.0f, 1.0f), // bottom-right-far
		Vec4(1.0f,  1.0f,  1.0f, 1.0f), // top-right-far
		Vec4(-1.0f,  1.0f,  1.0f, 1.0f)  // top-left-far
	};

	// Calculate the inverse of the projection-view matrix
	Mat4 inverseMatrix = projectionViewMatrix.inversa();

	// Transform corners from NDC to world space
	Vec3 frustumCornersWorld[8];
	for (int i = 0; i < 8; i++) {
		// Transform to world space
		Vec4 worldHomogeneous = inverseMatrix * frustumCornersNDC[i];

		// Perform perspective divide
		frustumCornersWorld[i] = Vec3(
			worldHomogeneous.x / worldHomogeneous.w,
			worldHomogeneous.y / worldHomogeneous.w,
			worldHomogeneous.z / worldHomogeneous.w
		);
	}

	// Define the 12 edges of the frustum (indices of frustumCornersWorld)
	unsigned int edges[12][2] = {
		{0, 1}, {1, 2}, {2, 3}, {3, 0},  // Near plane edges
		{4, 5}, {5, 6}, {6, 7}, {7, 4},  // Far plane edges
		{0, 4}, {1, 5}, {2, 6}, {3, 7}   // Connecting edges
	};

	// Create vertices for the lines
	std::vector<FrustumVertex> vertices;
	for (int i = 0; i < 12; i++) {
		FrustumVertex v1 = { frustumCornersWorld[edges[i][0]] };
		FrustumVertex v2 = { frustumCornersWorld[edges[i][1]] };
		vertices.push_back(v1);
		vertices.push_back(v2);
	}

	// Create VAO, VBO
	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(FrustumVertex), vertices.data(), GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(FrustumVertex), (void*)0);
	glEnableVertexAttribArray(0);

	// Use the provided shader
	shader.UseProgram();

	// Set the color uniform
	GLint colorLoc = glGetUniformLocation(shader.ID, "lineColor");
	glUniform4f(colorLoc, color.x, color.y, color.z, color.w);

	// Set model matrix to identity (already in world space)
	Mat4 identity = Mat4();
	GLint modelLoc = glGetUniformLocation(shader.ID, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_TRUE, identity.value_ptr());

	// Draw the frustum as lines
	glLineWidth(2.0f);
	glDrawArrays(GL_LINES, 0, vertices.size());
	glLineWidth(1.0f);

	// Clean up
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}