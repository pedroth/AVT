#version 330 core

in vec3 inPosition;
in vec3 inNormal;
in vec2 inTex;
in vec3 inTangent;
uniform vec3 MaterialDiffuse;
out vec3 exColor;

uniform mat4 ModelMatrix;
layout(std140) uniform SharedMatrices {
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

uniform vec3 LightDirection;

void main(void) {
	//FIXME should be replaced with NormalMatrix
	vec4 viewNormal = ViewMatrix * ModelMatrix * vec4(inNormal,0.0);
	vec3 N = normalize(viewNormal.xyz);
	vec4 LightView = ViewMatrix * vec4(LightDirection,0.0);
	vec3 L = -LightView.xyz;

	float NdotL = dot(N,L);
	float diffInten = max(NdotL,0.0);

	//exColor = LightDirection;
	//exColor = MaterialDiffuse;
	//exColor = N;
	//exColor = inNormal;
	//exColor = inTangent;
	//exColor = vec3(dot(inNormal,inTangent));
	//exColor = vec3(length(inNormal));
	//exColor = vec3(length(inTangent));
	exColor = diffInten * MaterialDiffuse;

	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(inPosition,1.0f);
}
