#pragma once
#include "Textures.h"
#include "Algebra_Lineare.h"


class Camera
{
public:
	// Stores the main vectors of the camera
	Vec3 Position, PrevPos;
	Vec3 Up = Vec3(0.0f, 1.0f, 0.0f);
	Vec3 Orientation = Vec3(0.0f, 0.0f, -1.0f);
	Mat4 cameraMatrix = Mat4(1.0f);


	//velocità camera
	float cameraSpeed = 0.0f;
	float speed = 0.5f;
	//sensibilità camera
	float sensitivity = 100.0f;
	//delta time
	float deltaTime = 0.0f;
	//last frame
	float lastFrame = 0.0f;
	//current frame
	float currentFrame = 0.0f;

	// Prevents the camera from jumping around when first clicking left click
	bool firstClick = true;

	// Memorizza la larghezza ed altezza della finestra
	float width;
	float height;




	// Il costruttore della camera per impostare i valori iniziali
	Camera(float width, float height, Vec3 position);

	// Aggiornare la matrice della camera al Vertex Shader
	void updateMatrix(float FOVdeg, float nearPlane, float farPlane);
	// Esportare la matrice della camera allo shader
	void Matrix(Shader& shader, const char* uniform);
	// Handles camera inputs
	void inputs(GLFWwindow* window, float xmin, float xmax, float zmin, float zmax);

	bool checkCollision(float xmin, float xmax, float zmin, float zmax);
};
