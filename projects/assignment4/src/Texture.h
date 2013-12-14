#pragma once
#include"engine.h"
#include "perlin.h"

/*for now it just holds one 3d tex */

class Texture {
private:
	GLuint texId;
public:
	Texture() {}
	/* 
		create perlin noise texture
		size - must be a power of two. size x size x size  is the size of returning array 
	*/
	static BYTE* get3DPerlinNoise(int octaves, float freq, float amp, int seed, int size);
	/*
		size - is the width, height and depth of the image.
	*/
	void create3DTexture(const BYTE* image, int size);
};