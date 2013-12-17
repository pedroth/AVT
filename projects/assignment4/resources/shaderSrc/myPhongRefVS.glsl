#version 330 core

uniform vec3 LightPosition;

in vec3 inPosition;
in vec3 inNormal;
in vec2 inTex;

out vec3 exViewPosition;
out vec3 exViewNormal;
out vec3 exLightDistance;


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

	vec4 viewLightPos = ViewMatrix * vec4(LightPosition,1.0);
	vec3 lightDistance = viewLightPos.xyz - exViewPosition;
	exLightDistance = lightDistance.xyz;
	
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(inPosition,1.0f);
}
