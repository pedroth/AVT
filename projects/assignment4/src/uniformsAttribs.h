//uniformsAttribs.h

#pragma once

#include <GL/glew.h>

#include <string>
#include <set>


namespace VertexAttribs {
	namespace Names {
		extern const std::string POSITION;
		//extern const std::string COLOR;
		extern const std::string NORMAL;
		extern const std::string TEXCOORD;
	}
	namespace Indexes {
		extern const GLuint POSITION;
		//extern const GLuint COLOR;
		extern const GLuint NORMAL;
		extern const GLuint TEXCOORD;
	}
}

namespace Uniforms {
	extern const std::string MATRIX;
	extern const std::string COLOR;
}

namespace UniformBlocks {
	namespace Names {
		extern const std::string SHARED_MATRICES;
	}
	namespace BindPoints {
		extern const GLuint SHARED_MATRICES;
	}
}