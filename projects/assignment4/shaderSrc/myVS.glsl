#version 330 core

in vec3 inPosition;
in vec3 inNormal;
in vec2 inTex;
uniform vec3 Color;
out vec3 exColor;

uniform mat4 ModelMatrix;
layout(std140) uniform SharedMatrices {
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

void main(void) {
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(inPosition,1.0f);
	exColor = Color;
}
