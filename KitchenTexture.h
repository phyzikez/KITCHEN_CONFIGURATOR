#pragma once
#include "GL/glew.h"
#include <iostream>
#include <fstream>
#include <string>

class KitchenTexture
{
public:
    KitchenTexture();
    KitchenTexture(const std::string& fileName, bool isMap = true);
    ~KitchenTexture();
    KitchenTexture(const KitchenTexture& rhs) = delete;
    KitchenTexture& operator = (const KitchenTexture& rhs) = delete;

    bool loadTextureImage(const std::string& fileName, bool generateMipMaps = true);
    void bind(GLuint texUnit = 0);
    void unbind(GLuint texUnit = 0);
protected:
    GLuint texture;
};