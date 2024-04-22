#ifndef _EGGS_PERLIN_NOISE
#define _EGGS_PERLIN_NOISE

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

vec2 createVector(float x, float y) {
	vec2 v;
	v.x = x;
	v.y = y;
	return v;
}

vec2 normalize(vec2 vec) {
	float ang = atan2(vec.y, vec.x);
	vec2 n;
	n.x = cos(ang);
	n.y = sin(ang);
	return n;
}

float magnitude(vec2* vec) {
	return sqrt(pow(vec->x, 2) + pow(vec->y, 2));
}

float dot(vec2 v1, vec2 v2) {
	return v1.x * v2.x + v1.y * v2.y;
}

float lerp(float a, float b, float i) {
	return a + (b - a) * i;
}

float easeInOutCubic(float x) {
	return x < 0.5 ? 4 * x * x * x : 1 - pow(-2 * x + 2, 3) / 2;
}

float getGradient(int seed) {
	srand(seed);
	return float(rand() - RAND_MAX / 2) / float(RAND_MAX / 2);
}

vec2 getGradient(int x, int y, int xSeed, int ySeed) {
	vec2 g;
	
	// Figure out a better way to generate random numbers
	g.x = getGradient(xSeed * y);
	g.y = getGradient(ySeed * x);

	return normalize(g);
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
	int p0 = int(floor(p));
	int p1 = int(floor(p)) + 1;

	return (1 - easeInOutCubic(p - p0)) * getGradient(p0 + offsets->x + octave) * (p - p0) + 
		         easeInOutCubic(p - p0) * getGradient(p1 + offsets->x + octave) * (p - p1);
}

float getPerlinNoise(perlinOffsets* offsets, float p) {
	float n = 0.0f;
	for(int i = 1; i <= offsets->octave; i++) {
		n += getNoise(offsets, p * (i * 0.5f), i) * (2.0f / pow(2, i));
	}
	return n;
}

float getNoise(perlinOffsets* offsets, float u, float v, int octave) {
	int u0 = int(floor(u));
	int u1 = int(floor(u)) + 1;
	int v0 = int(floor(v));
	int v1 = int(floor(v)) + 1;

	// top left influcence
	float i0 = dot(
		getGradient(u0, v0, u0 + offsets->x + octave, v0 + offsets->y + octave),
		createVector(u0 - u, v0 - v)
	);

	// top right influence
	float i1 = dot(
		getGradient(u1, v0, u1 + offsets->x + octave, v0 + offsets->y + octave),
		createVector(u1 - u, v0 - v)
	);

	// bottom left influence
	float i2 = dot(
		getGradient(u0, v1, u0 + offsets->x + octave, v1 + offsets->y + octave),
		createVector(u0 - u, v1 - v)
	);

	// bottom right influence
	float i3 = dot(
		getGradient(u1, v1, u1 + offsets->x + octave, v1 + offsets->y + octave),
		createVector(u1 - u, v1 - v)
	);

	float top = (1.0f - easeInOutCubic(u - u0)) * i0 + easeInOutCubic(u - u0) * i1;
	float bottom = (1.0f - easeInOutCubic(u - u0)) * i2 + easeInOutCubic(u - u0) * i3;

	return (1.0f - easeInOutCubic(v - v0)) * top + easeInOutCubic(v - v0) * bottom;
}

float getPerlinNoise(perlinOffsets* offsets, float u, float v) {
	float n = 0.0f;
	for(int i = 1; i <= offsets->octave; i++) {
		n += getNoise(offsets, u * (i * 0.5f), v * (i * 0.5f), i) * (2.0f / pow(2, i));
	}
	return n;
}

#endif
