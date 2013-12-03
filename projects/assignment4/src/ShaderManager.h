#ifndef _SHADERMANAGER_H_
#define _SHADERMANAGER_H_

#include <iostream>
#include <string>
#include <vector>
#include "ShaderProgram.h"

#include "GL/glew.h"
#include "GL/freeglut.h"

class ShaderManager{
private:
   std::vector<ShaderProgram*> shaderPrograms;

public:
   
   void addShaderProgram(int id, GLenum shaderType, const std::string &shaderSource);

   ShaderProgram* getShaderProgram(int id);

   void removeShaderProgram(int id);


};

#endif