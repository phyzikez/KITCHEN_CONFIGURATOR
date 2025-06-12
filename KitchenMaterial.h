#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <array>
#include <vector>
#include <list>
#include <string>
#include <format>
#include "KitchenTexture.h"
#include "KitchenShader.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// Material For Kitchen Meshes
class KitchenMaterial : public KitchenTexture, public KitchenShader 
{
public:
	KitchenMaterial(KitchenTexture* texture, KitchenShader* shader, const std::string& nameMat);
	~KitchenMaterial();
	KitchenTexture* getTexture();
	KitchenShader* getShader();
	std::string getMaterialName() { return matName; }
	glm::vec3 getAmbient() { return ambient; }
	glm::vec3 getSpecular() { return specular; }
	GLint getShininess() { return shininess; }
	void setAmbient(glm::vec3 amb) { ambient = amb; }
	void setSpecular(glm::vec3 spec) { specular = spec; }
	void setShininess(GLint sh) { shininess = sh; }
private:
	KitchenTexture* texture;
	KitchenShader* shader;
	std::string matName;
	glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	glm::vec3 specular = glm::vec3(0.7f, 0.7f, 0.7f);
	GLint shininess = 32.0f;
};

enum class MATERIAL_TYPE 
{
	TEST,
	SKYBOX,
	GROUND,
	PAINT,
	DECO,
	STONE,
	MARBLE,
	WOOD,
	TILE
};

enum class SHADER_TYPE 
{
	SKYBOX,
	SCENE
};

class KitchenMaterialPack 
{
public:
	KitchenMaterialPack();
	~KitchenMaterialPack();
	KitchenMaterial* getMaterial(MATERIAL_TYPE type, int idx);
	KitchenShader* getShader(SHADER_TYPE type);
	int getMaterialCount(MATERIAL_TYPE type);
	void addMaterial(MATERIAL_TYPE type, KitchenMaterial* newMat);
private:
	std::map<SHADER_TYPE, KitchenShader*> shaders;
	std::map<MATERIAL_TYPE, std::vector<KitchenMaterial*>> materials;
};