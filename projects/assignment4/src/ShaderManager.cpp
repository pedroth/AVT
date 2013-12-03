#include "ShaderManager.h"

void ShaderManager::addShaderProgram(int id, GLenum shaderType, const std::string &shaderSource)
{
   for(unsigned i = 0; i < shaderPrograms.size(); i++){
      if(shaderPrograms[i].getId() == id){
         std::cerr << "Error: Program shader with id " << id << " already exists" << std::endl;
         exit(-1);
      }
   }

   ShaderProgram *prog = new ShaderProgram(id);
   prog->addShader(shaderType, shaderSource);
   shaderPrograms.push_back(prog);

}

void ShaderManager::removeShaderProgram(int id, GLuint *programId){
   ShaderProgram *prog;

   for(unsigned i = 0; i < shaderPrograms.size(); i++){
      if(shaderPrograms[i]->getId() == id){
         prog = shaderPrograms[i];
         delete *prog;

         shaderPrograms.erase(shaderPrograms.begin() + i);
         return;
      }
   }
   std::cerr << "Error: Program shader with id " << id << " doesn't exist" << std::endl;
   exit(-1);
}

ShaderProgram* getShaderProgram(int id){
   ShaderProgram *prog;

   for(unsigned i = 0; i < shaderPrograms.size(); i++){
      if(shaderPrograms[i]->getId() == id){
         prog = shaderPrograms[i];
         
         return prog;
      }
   }
   std::cerr << "Error: Program shader with id " << id << " doesn't exist" << std::endl;
   exit(-1);
}

