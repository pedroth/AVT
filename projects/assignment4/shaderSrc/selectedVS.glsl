#version 330 core

in vec3 inPosition;
in vec3 inNormal;
in vec2 inTex;
uniform vec3 Color;
out vec3 exColor;

uniform vec3 LightDirection;
uniform float Time0_1;
uniform vec3 SelectedColor;

uniform mat4 ModelMatrix;
layout(std140) uniform SharedMatrices {
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

void main(void) {
	vec4 viewNormal = ViewMatrix * ModelMatrix * vec4(inNormal,0.0);
	vec3 N = normalize(viewNormal.xyz);
	vec4 LightView = ViewMatrix * vec4(LightDirection,0.0);
	vec3 L = -LightView.xyz;

	float NdotL = dot(N,L);
	float diffInten = max(NdotL,0.0);

	vec3 currColor = mix(Color,SelectedColor,Time0_1);

	exColor = diffInten * currColor;

	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(inPosition,1.0f);
}
