#version 330 core

in vec3 inPosition;
in vec3 inNormal;
in vec2 inTex;

out vec3 exViewPosition;
out vec3 exViewNormal;
out vec3 exPosition;

uniform mat4 ModelMatrix;
layout(std140) uniform SharedMatrices {
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};


void main(void) {
	//FIXME should be replaced with NormalMatrix
	vec4 viewNorm = ViewMatrix * ModelMatrix * vec4(inNormal,0.0);
	exViewNormal = normalize(viewNorm.xyz);
	
	vec4 viewPos = ViewMatrix * ModelMatrix * vec4(inPosition,1.0f);
	exViewPosition = viewPos.xyz;
	exPosition = inPosition;
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(inPosition,1.0f);
}