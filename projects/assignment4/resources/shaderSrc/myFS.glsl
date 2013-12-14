#version 330 core

in vec3 exColor;
in vec3 exPosition;
uniform sampler3D UTexture;
out vec4 outColor;

void main(void)
{
	float noise = texture(UTexture, normalize(exPosition) * 0.5 + 0.5).r;
	outColor = vec4(noise * exColor,1.0f);

}
