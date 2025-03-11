#version 400 core

layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 camMatrix; // Your camera's view-projection matrix

void main() {
    gl_Position = camMatrix * model * vec4(aPos, 1.0);
}