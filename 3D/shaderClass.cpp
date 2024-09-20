#include "shaderClass.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


std::string FileRead(std::string filepath)
{
	std::string Code = "";
	std::ifstream file;
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		file.open(filepath.c_str());

		std::stringstream fileStream;
		//Legge il contenuto del file in streams

		fileStream << file.rdbuf(); //Legge il file in un buffer

		file.close();

		Code = fileStream.str();

	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	return Code;
}

Shader::Shader(std::string vertexPath, std::string fragmentPath)
{
	int success, success2, success3;

	const std::string vertexCode = FileRead(vertexPath);
	const std::string fragmentCode = FileRead(fragmentPath);

	//compiliamo vertex shader
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	const char* vertSrc = vertexCode.c_str();

	glShaderSource(vs, 1, &vertSrc, nullptr);
	glCompileShader(vs);

	glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(vs, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	vertSrc = nullptr;

	//compiliamo fragment shader
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fragSrc = fragmentCode.c_str();

	glShaderSource(fs, 1, &fragSrc, nullptr);
	glCompileShader(fs);

	glGetShaderiv(fs, GL_COMPILE_STATUS, &success2);
	if (!success2)
	{
		char infoLog2[512];
		glGetShaderInfoLog(fs, 512, NULL, infoLog2);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog2 << std::endl;
	}

	fragSrc = nullptr;

	//creiamo lo shader program
	ID = glCreateProgram();

	glAttachShader(ID, vs);
	glAttachShader(ID, fs);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success3);
	if (!success3)
	{
		char infoLog3[512];
		glGetProgramInfoLog(ID, 512, NULL, infoLog3);
		std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog3 << std::endl;
	}

	glDeleteShader(vs);
	glDeleteShader(fs);
}

void Shader::setInt(const char* uniform, int value)
{
	UseProgram();
	glUniform1i(glGetUniformLocation(ID, uniform), value);
}

void Shader::UseProgram()
{
	glUseProgram(ID);
}

void Shader::Delete()
{
	glDeleteProgram(ID);
}

