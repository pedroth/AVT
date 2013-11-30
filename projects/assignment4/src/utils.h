//utils.h

#ifndef __UTILS_H__
#define __UTILS_H__

#include <string>

extern const float PI_CONSTANT;
float deg2Rad(float degrees);
std::string readFromFile(const std::string &filePath);
bool isOpenGLError();
void checkOpenGLError(std::string error);

#endif