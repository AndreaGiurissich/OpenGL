#include "Camera.h"

#include <corecrt_math_defines.h>

Camera::Camera(float width, float height, Vec3 position)
{
	Camera::width = width;
	Camera::height = height;
	Position = position;
}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
	// Inizializza le matrici altrimenti saranno la matrice nulla
	Mat4 view = Mat4();
	Mat4 projection = Mat4();

	// Fa sì che la camera guardi nella giusta direzione dalla giusta posizione
	view = view.lookAt(Position, Position + Orientation, Up);
	// Aggiunge la prospettiva alla scena 
	projection = projection.perspective(FOVdeg, width/height, nearPlane, farPlane);

	// Imposta una nuova matrice della camera
	cameraMatrix = projection * view;
}

void Camera::Matrix(Shader& shader, const char* uniform)
{
	// Esporta la matrice della camera alla Vertex Shader
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_TRUE, &cameraMatrix.m[0][0]);
}


void Camera::inputs(GLFWwindow* window, float xmin, float xmax, float zmin, float zmax)
{
	Vec3 v = (Orientation.cross(Up));
	v.normalize();

	currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	cameraSpeed = speed * deltaTime;

	Position.y = -1.7f;

	if (checkCollision(xmin, xmax, zmin, zmax)){
		PrevPos = Position;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			Position += Orientation * cameraSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			Position -= ( v * cameraSpeed);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			Position -= (Orientation * cameraSpeed);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			Position += v * cameraSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			Position += Up * cameraSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		{
			Position -= Up * cameraSpeed;
		}
	}
	else
	{
		Position = PrevPos;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		cameraSpeed = 0.04f;
	}


	// Gestisce gli input del mouse
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		// Nasconde il cursore del mouse
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Impedisce alla camera di saltare al primo clic
		if (firstClick)
		{
			glfwSetCursorPos(window, (width / 2), (height / 2));
			firstClick = false;
		}

		// Memorizza le coordinate del cursore
		double mouseX;
		double mouseY;
		// Recupera le coordinate del cursore
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// Normalizza e sposta le coordinate del cursore in modo che inizino al centro dello schermo
		// e poi li "trasforma" in gradi
		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

		// Calcola il prossimo cambiamento verticale nell'orientamento
		Vec3 v1 = Orientation.cross(Up);
		v1.normalize();
		Vec3 newOrientation = Orientation.rotate(-rotX, v1);

		// Decide se il prossimo orientamento verticale è valido o meno, evita il gimbal lock
		if (abs(newOrientation.angle(Up) - 90.0f) <= 85.0f)
		{
			Orientation = newOrientation;
		}
		
		// Ruota l'orientamento da sinistra a destra
		Orientation = Orientation.rotate(-rotY, Up);

		// Imposta il cursore del mouse al centro dello schermo in modo che non finisca per vagare
		glfwSetCursorPos(window, (width / 2), (height / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		// Mostra il cursore da quando la camera non si guarda più intorno
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Assicura che la prossima volta che la camera si guarda intorno non si muove
		firstClick = true;
	}
}

bool Camera::checkCollision(float xmin, float xmax, float zmin, float zmax)
{
	if (Position.x >= xmin && Position.x <= xmax && Position.z >= zmin && Position.z <= zmax) {
		return true;
	}
	else return false;
}
