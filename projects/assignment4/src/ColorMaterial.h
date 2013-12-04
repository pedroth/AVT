#include"engine.h"

class ColorMaterial {
private:
	glm::vec3 color;
public:
	ColorMaterial();
	glm::vec3 getColor();
	void setColor(glm::vec3 color);
};