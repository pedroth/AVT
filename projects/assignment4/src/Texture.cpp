#include "Texture.h"
#include <iostream>

BYTE* Texture::get3DPerlinNoise(int octaves, float freq, float amp, int seed, int size) {
	int n = size * size * size;
	int s2 = size * size;
	Perlin noise(octaves,freq,amp,seed);
	BYTE* image = new BYTE[n];
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			for (int k = 0; k < size; k++) {
				float step = 2.0f / ((float)size);
				float x = -1.0f + step * i;
				float y = -1.0f + step * j;
				float z = -1.0f + step * k;
				float noi = noise.Get(x, y, z);
				int aux = (int)((noi + 1.0) * (256.0 / 2.0));
				//std::cout << (BYTE) aux << std::endl;
				image[i * s2 + j * size + k] = (BYTE)aux;
			}
		}
	}
	return image;
}
void Texture::create3DTexture(const BYTE* image, int size) {
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_3D, texId);

	// Transfer the image data from the CPU to the GPU.
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RED, size, size, size, 0, GL_RED, GL_UNSIGNED_BYTE, image);

	// Setting the texture parameters.
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}