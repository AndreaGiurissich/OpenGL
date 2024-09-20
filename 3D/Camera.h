#pragma once
#include "Texture.h"
#include "Algebra_Lineare.h"

class Camera
{
public:
	// Stores the main vectors of the camera
	Vec3 Position;
	Vec3 Up = Vec3(0.0f, 1.0f, 0.0f);
	Vec3 Orientation = Vec3(0.0f, 0.0f, -1.0f);
	Mat4 cameraMatrix = Mat4(1.0f);

	// Prevents the camera from jumping around when first clicking left click
	bool firstClick = true;

	// Stores the width and height of the window
	float width;
	float height;

	// Adjust the speed of the camera and it's sensitivity when looking around
	float speed = 0.1f;
	float sensitivity = 100.0f;

	// Camera constructor to set up initial values
	Camera(float width, float height, Vec3 position);

	// Updates the camera matrix to the Vertex Shader
	void updateMatrix(float FOVdeg, float nearPlane, float farPlane);
	// Exports the camera matrix to a shader
	void Matrix(Shader& shader, const char* uniform);
	// Handles camera inputs
	void Inputs(GLFWwindow* window);
};
