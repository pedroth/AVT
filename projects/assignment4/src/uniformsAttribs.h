//uniformsAttribs.h

#pragma once

#include <GL/glew.h>

#include <string>
#include <set>


namespace VertexAttribs {
	namespace Names {
		extern const std::string POSITION = "inPos";
		//extern const std::string COLOR;
		extern const std::string NORMAL = "inNormal";
		extern const std::string TEXCOORD = "inTex";
	}
	namespace Indexes {
		extern const GLuint POSITION = 1;
		//extern const GLuint COLOR;
		extern const GLuint NORMAL = 2;
		extern const GLuint TEXCOORD = 3;
	}
}

namespace Uniforms {
	extern const std::string MATRIX = "matrix";
	extern const std::string COLOR = "color";
}

namespace UniformBlocks {
	namespace Names {
		extern const std::string SHARED_MATRICES = "sharedMatrix";
	}
	namespace BindPoints {
		extern const GLuint SHARED_MATRICES = 0;
	}
}