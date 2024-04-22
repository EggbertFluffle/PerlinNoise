#include "perlin.h"
#include <time.h>
#include <stdlib.h>
#include <raylib.h> 
#include <stdio.h>

const int width = 600;
const int height = 400;

void calculatePixels(perlinOffsets* po, float pixels[width][height]) {
	po = createPerlinOffsets(4);

	for(int x = 0; x < width; x++) {
		for(int y = 0; y < height; y++) {
			pixels[x][y] = 0;
		}
	}

	for(int x = 0; x < width; x++) {
		float p = getPerlinNoise(po, float(x) / float(width) * 10.0f);
		int max = int((p + 1.0f) * (height / 2));
		for(int y = height - 1; y >= max; y--) {
			pixels[x][height - y] = 1;
		}
	}
}

int main() {
	InitWindow(width, height, "Perlin Noise");

	srand(time(NULL));

	SetTargetFPS(30);

	perlinOffsets* po = createPerlinOffsets(2);

	float pixels[width][height];

	calculatePixels(po, pixels);

	while(!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(RAYWHITE);

		PollInputEvents();
		if(IsKeyDown(KEY_R)) {
			calculatePixels(po, pixels);
		} else if(IsKeyDown(KEY_Q)) {
			CloseWindow();
		}

		for(int x = 0; x < width; x++) {
			for(int y = 0; y < height; y++) {
				unsigned char g = pixels[x][y] * 255;
				Color c = (Color){g, g, g, 255};
				DrawPixel(x, y, c);
			}
		}

		EndDrawing();
	}

	CloseWindow();
	return 0;
}
