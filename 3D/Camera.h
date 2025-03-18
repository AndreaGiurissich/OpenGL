#pragma once
#include "Textures.h"
#include "Algebra_Lineare.h"


class Camera
{
public:
	// Immagazzina i principali vettore della camera
	Vec3 Position;
	Vec3 Up = Vec3(0.0f, 1.0f, 0.0f);
	Vec3 Orientation = Vec3(0.0f, 0.0f, -1.0f);
	Mat4 cameraMatrix = Mat4(1.0f);

	// Impedisce alla camera di spostarsi quando viene effettuato il primo click con il tasto destro del mouse
	bool firstClick = true;

	// Memorizza la larghezza ed altezza della finestra
	float width;
	float height;

	// Aggiusta la velocità della camera e la sua sensibilità quando ci si gira intorno
	float speed = 0.1f;
	float sensitivity = 100.0f;

	// Il costruttore della camera per impostare i valori iniziali
	Camera(float width, float height, Vec3 position);

	// Aggiornare la matrice della camera al Vertex Shader
	void updateMatrix(float FOVdeg, float nearPlane, float farPlane);
	// Esportare la matrice della camera allo shader
	void Matrix(Shader& shader, const char* uniform);
	// Gestisce gli input della camera
	void inputs(GLFWwindow* window);
};
