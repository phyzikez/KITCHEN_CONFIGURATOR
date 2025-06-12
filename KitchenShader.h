#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

enum ShaderMode
{
	VERTEX,
	FRAGMENT,
	PROGRAM
};

class KitchenShader
{
public:
	KitchenShader();
	KitchenShader(const char* vsFilename, const char* fsFilename);
	~KitchenShader();

	// Vertex & Fragment Shaders
	bool loadShaders(const char* vsFilename, const char* fsFilename);
	void  checkCompileErrors(GLuint shader, ShaderMode mode);
	std::string fileToString(const std::string& filename);

	void use();

	void setUniform(const GLchar* name, const glm::vec2& v);
	void setUniform(const GLchar* name, const glm::vec3& v);
	void setUniform(const GLchar* name, const glm::vec4& v);
	void setUniform(const GLchar* name, const glm::mat4& m);
	void setUniform(const GLchar* name, const GLfloat f);
	void setUniform(const GLchar* name, const GLint v);

	GLint getUniformLocation(const GLchar* name);
	void setUniformSampler(const GLchar* name, const GLint& slot);

protected:
	GLuint mHandle;
	std::map<std::string, GLint> mUniformLocations;
};