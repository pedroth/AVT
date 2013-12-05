#ifndef __UTILS_H__
#define __UTILS_H__

#include <string>

extern const float PI_CONSTANT;
float deg2Rad(float degrees);
int positiveModulo(int i, int n);
std::string readFromFile(const std::string &filePath);
bool isOpenGLError();
void checkOpenGLError(std::string error);

#endif