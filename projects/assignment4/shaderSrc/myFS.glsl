#version 330 core

in vec3 exColor;
out vec4 outColor;

void main(void)
{

	outColor = vec4(exColor,1.0f);

}
