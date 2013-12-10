#version 330 core

layout(std140) uniform SharedMatrices {
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};
uniform vec3 LightPosition;
uniform vec3 LightAmbient;
uniform vec3 LightDiffuse;
uniform vec3 LightSpecular;
uniform vec3 LightAttenuation;
uniform float LightRangeLimit;

uniform vec3 MaterialEmit;
uniform vec3 MaterialAmbient;
uniform vec3 MaterialDiffuse;
uniform vec3 MaterialSpecular;
uniform float MaterialShininess;

in vec3 exColor;
out vec4 outColor;

void main(void)
{

	outColor = vec4(exColor,1.0f);

}
