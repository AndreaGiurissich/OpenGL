#pragma once
#include <string>
#include "GLCommon.h"


std::string FileRead(std::string filepath);

class Shader
{
	public:
		GLuint ID;
		Shader(std::string vertexPath, std::string fragmentPath);

		void setInt(const char* uniform, int value);

		void UseProgram();
		void Delete();

};
