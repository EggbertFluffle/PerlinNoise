#ifndef _EGGS_PERLIN_NOISE
#define _EGGS_PERLIN_NOISE

#include <cstdlib>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdio.h>

typedef struct {
	int x;
	int y;
	int z;
	int octave;
} perlinOffsets;

typedef struct {
	float x;
	float y;
} vec2;

void normalize(vec2* vec) {
	float ang = atan2(vec->y, vec->x);
	vec->x = cos(ang);
	vec->y = sin(ang);
}

float magnitude(vec2* vec) {
	return sqrt(pow(vec->x, 2) + pow(vec->y, 2));
}

float dot(vec2* v1, vec2* v2) {
	return v1->x * v2->x + v1->y * v2->y;
}

float lerp(float a, float b, float i) {
	return a + (b - a) * i;
}

float easeInOutCubic(float x) {
	return x < 0.5 ? 4 * x * x * x : 1 - pow(-2 * x + 2, 3) / 2;
}

perlinOffsets* createPerlinOffsets(int octave) {
	perlinOffsets* po = (perlinOffsets*)malloc(sizeof(perlinOffsets));
	srand(time(NULL));
	po->x = rand();
	po->y = rand();
	po->z = rand();
	po->octave = octave;
	return po;
}

float getNoise(perlinOffsets* offsets, float p, int octave) {
	int p0 = static_cast<int>(floor(p));
	int p1 = static_cast<int>(floor(p)) + 1;

	srand(offsets->x + p0 + octave);
	float g0 = float(float(rand()) - RAND_MAX / 2) / float(RAND_MAX / 2);

	srand(offsets->x + p1 + octave);
	float g1 = float(float(rand()) - RAND_MAX / 2) / float(RAND_MAX / 2);

	return (1 - easeInOutCubic(p - p0)) * g0 * (p - p0) + easeInOutCubic(p - p0) * g1 * (p - p1);
}

float getPerlinNoise(perlinOffsets* offsets, float p) {
	float n = 0.0f;
	for(int i = 1; i <= offsets->octave; i++) {
		n += getNoise(offsets, p * (i * 0.5f), i) * (2.0f / float(pow(2, i)));
	}
	return n;
}

#endif
