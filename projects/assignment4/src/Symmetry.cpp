#include "Symmetry.h"

Symmetry::Symmetry(Map * original) : originalObjects(original) { }

void Symmetry::draw(ShaderProgram*  shader) {
	for (Map::iterator it = originalObjects->begin(); it != originalObjects->end(); ++it) {
		WorldObject * wo = it->second;
		wo->setPosTrans(transform());
		wo->draw(shader);
		wo->setPosTrans(glm::mat4(1.0f));
	}
}