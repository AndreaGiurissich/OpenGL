#include "Camera.h"

Camera::Camera(float width, float height, Vec3 position)
{
	Camera::width = width;
	Camera::height = height;
	Position = position;
}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
	// Initializes matrices since otherwise they will be the null matrix
	Mat4 view = Mat4();
	Mat4 projection = Mat4();

	// Makes camera look in the right direction from the right position
	view = view.lookAt(Position, Position + Orientation, Up);
	// Adds perspective to the scene
	projection = projection.persp(FOVdeg, width, height, nearPlane, farPlane);

	// Sets new camera matrix
	cameraMatrix = projection * view;
}

void Camera::Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform)
{
	// Initializes matrices since otherwise they will be the null matrix
	Mat4 view = Mat4();
	Mat4 projection = Mat4();
	Vec3 center = Position + Orientation;

	// Makes camera look in the right direction from the right position
	view = view.lookAt(Position, center, Up);
	// Adds perspective to the scene
	projection = projection.persp(FOVdeg, width , height, nearPlane, farPlane);

	Mat4 pv = projection * view;

	// Exports the camera matrix to the Vertex Shader
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_TRUE, &pv.m[0][0]);
}


void Camera::inputs(GLFWwindow* window)
{
	Vec3 v = (Orientation.cross(Up));
	v.normalize();

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		Position += Orientation * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		Position -= ( v * speed);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		Position -= (Orientation * speed);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		Position += v * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		Position += Up * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		Position -= Up * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		speed = 0.4f;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		speed = 0.1f;
	}


	// Handles mouse inputs
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		// Hides mouse cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Prevents camera from jumping on the first click
		if (firstClick)
		{
			glfwSetCursorPos(window, (width / 2), (height / 2));
			firstClick = false;
		}

		// Stores the coordinates of the cursor
		double mouseX;
		double mouseY;
		// Fetches the coordinates of the cursor
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
		// and then "transforms" them into degrees 
		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

		// Calculates upcoming vertical change in the Orientation

		Vec3 newOrientation = Orientation.rotate(-rotX, v);

		// Decides whether or not the next vertical Orientation is legal or not
		if ((abs(newOrientation.angle(Up) - 90.0f))<= 85.0f)
		{
			Orientation = newOrientation;
		}

		// Rotates the Orientation left and right
		Orientation = Orientation.rotate(-rotY, Up);

		// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
		glfwSetCursorPos(window, (width / 2), (height / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		// Unhides cursor since camera is not looking around anymore
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Makes sure the next time the camera looks around it doesn't jump
		firstClick = true;
	}
}
