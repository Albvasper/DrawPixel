#include "pch.h"
#include "Matrix.h"
#include "Text.h"
#include <SDL.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include <iostream>
#include <SDL_ttf.h>

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;
bool init();
void close();
SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;
SDL_Texture *texture = NULL;

bool init() {
	bool success = true;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else {
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			printf("Warning: Linear texture filtering not enabled!");
		}
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL) {
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else {
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL) {
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else {
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			}
		}
	}
	return success;
}

void close() {
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	SDL_DestroyTexture(texture);;
	gWindow = NULL;
	gRenderer = NULL;
	TTF_Quit();
	SDL_Quit();
}

//-------------------------------------------------------------------------
//Draw a single pixel
void DrawPixelF(int x, int y) {
	Vector2 v(x, y);
	SDL_SetRenderDrawColor(gRenderer, 64, 156, 146, 0xFF);
	SDL_RenderDrawPoint(gRenderer, v.getX() + SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - v.getY());
}

//Draw Line
void DrawLine(int x1, int y1, int x2, int y2) {
	float m;
	float c;
	float x, y;

	m = ((y2 - y1) / (x2 - x1));
	c = y1 - (m * x1);
	if (m * 1 < 1) {
		for (x = 0; x < x2; x++) {
			x = x + 1;
			y = m * x + c;
			DrawPixelF(x, y);
		}
	}
	else if (m * 1 > 1) {
		for (y = 0; y < y2; y++) {
			y = y + 1;
			x = y - c / m;
			DrawPixelF(x, y);
		}
	}
}

//Draw DDA Line
void DDALine(int x1, int y1, int x2, int y2) {
	float x, y;
	float xIner, yIner;
	float dx, dy;
	int i = 0;
	float deno;
	dx = x2 - x1;
	dy = y2 - y1;
	if (dy < dx) {
		deno = dx;
	}
	if (dy >= dx) {
		deno = dy;
	}
	xIner = dx / deno;
	yIner = dy / deno;
	x = x1;
	y = y1;

	for (i; i <= deno; i++) {
		DrawPixelF(round(x), round(y));
		x = x + xIner;
		y = y + yIner;
	}
}

//Draw Bresenham Line
void BresenhamL(int x, int y, int dx, int dy, int incrX, int incrY) {
	int i = 0;
	int p = (2 * dy) - dx;
	if (dx > dy) {
		while (i < dx) {
			DrawPixelF(x, y);
			x += incrX;
			if (p < 0) {
				p = p + (2 * dy);
			}
			else {
				y += incrY;
				p = p + (2 * (dy - dx));
			}
			i++;
		}
	}
	else {
		while (i < dy) {
			DrawPixelF(x, y);
			y += incrY;
			if (p < 0) {
				p = p + (2 * dx);
			}
			else {
				x += incrX;
				p = p + (2 * (dx - dy));
			}
			i++;
		}
	}
}

//Draw Cruve
void BezierCurve(int xPoints[4], int yPoints[4]) {
	for (float t = 0.0; t < 1.0; t += 0.0005) {
		double x = pow(1 - t, 3) * xPoints[0] + 3 * t * pow(1 - t, 2) * xPoints[1] + 3 * pow(t, 2) * (1 - t) * xPoints[2] + pow(t, 3) * xPoints[3];
		double y = pow(1 - t, 3) * yPoints[0] + 3 * t * pow(1 - t, 2) * yPoints[1] + 3 * pow(t, 2) * (1 - t) * yPoints[2] + pow(t, 3) * yPoints[3];
		DrawPixelF(x, y);
	}
	for (int i = 0; i < 4; i++) {
		DrawPixelF(xPoints[i], xPoints[i]);
	}
}

//Draw a circle
void DrawCircle(int xc, int yc, int r) {
	Vector2 centroV(int xc, int yc);
	int x = 0;
	int y = 0;
	Vector2 pixel(x, y);
	int p;
	x = 0;
	y = r;
	p = 1 - r;
	for (int i = 0; x <= y; i++) {
		if (p <= 0) {
			x = x + 1;
			y = y;
			p = p + 2 * x + 3;
		}
		else if (p > 0) {
			x = x + 1;
			y = y - 1;
			p = 2 * x - 2 * y + 5 + p;
		}
		DrawPixelF(x + xc, y + yc);
		DrawPixelF(x + xc, -y + yc);
		DrawPixelF(-x + xc, y + yc);
		DrawPixelF(-x + xc, -y + yc);
		DrawPixelF(y + xc, x + yc);
		DrawPixelF(y + xc, -x + yc);
		DrawPixelF(-y + xc, x + yc);
		DrawPixelF(-y + xc, -x + yc);
	}
}

void DrawMenu() {
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = SCREEN_WIDTH;
	rect.h = 50;
	//SDL_RenderDrawRect(gRenderer, &rect);
	SDL_RenderFillRect(gRenderer, &rect);
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
	TTF_Init();
	SDL_Color color = { 45, 211, 186 };
	Text t(gRenderer, "arial.ttf", 50 , "Draw Pixel!", color);
	t.Display(0, 0, gRenderer);
	SDL_RenderPresent(gRenderer);
}

void DrawAxis() {
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
	for (int y = 0; y < SCREEN_HEIGHT; y += 4) {
		SDL_RenderDrawPoint(gRenderer, SCREEN_WIDTH / 2, y);
		for (int i = 0; i < SCREEN_WIDTH; i += 20) {
			SDL_RenderDrawPoint(gRenderer, i, y);
		}
	}
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
	for (int x = 0; x < SCREEN_WIDTH; x += 4) {
		SDL_RenderDrawPoint(gRenderer, x, SCREEN_HEIGHT / 2);
		for (int i = 0; i < SCREEN_WIDTH; i += 20) {
			SDL_RenderDrawPoint(gRenderer, x, i);
		}
	}
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
	for (int x = 0; x < SCREEN_HEIGHT; x += 1) {
		SDL_RenderDrawPoint(gRenderer, SCREEN_WIDTH / 2, x);
	}
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
	for (int x = 0; x < SCREEN_WIDTH; x += 1) {
		SDL_RenderDrawPoint(gRenderer, x, SCREEN_HEIGHT / 2);
	}
	SDL_RenderPresent(gRenderer);
}

//-------------------------------------------------------------------------------------

int main(int argc, char* args[]) {
	int mouseX;
	int mouseY;
	int mouseX2;
	int mouseY2;
	int mouseX3;
	int mouseY3;
	int mouseX4;
	int mouseY4;
	int x[4];
	int y[4];
	int modeSelector = 0;
	int counter = -1;
	bool leftMouseButtonDown = false;
	texture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, 1920, 1080);
	Uint32* pixels = new Uint32[1920 * 1080];
	memset(pixels, 255, 1920 * 1080 * sizeof(Uint32));

	if (!init()) {
		printf("Failed to initialize!\n");
	}
	else {
		bool quit = false;
		SDL_Event e;
		SDL_RenderClear(gRenderer);
		while (!quit) {
			while (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					quit = true;
				}
			}
			DrawMenu();
			DrawAxis();
			SDL_UpdateTexture(texture, NULL, pixels, 1920 * sizeof(Uint32));
			SDL_WaitEvent(&e);
			switch (e.type) {
				case SDL_QUIT:
					quit = true;
					break;
				case SDL_MOUSEBUTTONUP:
					if (e.button.button == SDL_BUTTON_LEFT)
						leftMouseButtonDown = false;
					break;
				case SDL_MOUSEBUTTONDOWN:
					if (e.button.button == SDL_BUTTON_LEFT)
						leftMouseButtonDown = true;
				case SDL_MOUSEMOTION:
					if (leftMouseButtonDown) {
						counter = counter + 1;
						if (counter == 0) {
							mouseX = e.motion.x - (SCREEN_WIDTH / 2);
							mouseY = -e.motion.y + (SCREEN_HEIGHT / 2);
							x[0] = mouseX;
							y[0] = mouseY;
							DrawPixelF(mouseX, mouseY);
						}

						if (counter == 1) {
							mouseX2 = e.motion.x - (SCREEN_WIDTH / 2);
							mouseY2 = -e.motion.y + (SCREEN_HEIGHT / 2);
							x[1] = mouseX2;
							y[1] = mouseY2;
							DrawPixelF(mouseX2, mouseY2);
						}

						if (counter == 2) {
							mouseX3 = e.motion.x - (SCREEN_WIDTH / 2);
							mouseY3 = -e.motion.y + (SCREEN_HEIGHT / 2);
							x[2] = mouseX3;
							y[2] = mouseY3;
							DrawPixelF(mouseX3, mouseY3);
						}

						if (counter == 3) {
							mouseX4 = e.motion.x - (SCREEN_WIDTH / 2);
							mouseY4 = -e.motion.y + (SCREEN_HEIGHT / 2);
							x[3] = mouseX2;
							y[3] = mouseY2;
							DrawPixelF(mouseX4, mouseY4);
							counter = -1;
							modeSelector = 3;
						}

						switch (modeSelector) {
							case 1:
								DDALine(mouseX, mouseY, mouseX2, mouseY2);
								modeSelector = 0;
								break;

							case 2:
								BresenhamL(mouseX, mouseY, mouseX2, mouseY2, 1, 1);
								modeSelector = 0;
								break;

							case 3:
								BezierCurve(x, y);
								modeSelector = 0;
								break;

							case 4:
								DrawCircle(mouseX, mouseY, mouseX2);
								modeSelector = 0;
								break;
						}
					}
				break;
			}
			SDL_RenderCopy(gRenderer, texture, NULL, NULL);
			SDL_RenderPresent(gRenderer);
		}
	}
	close();
	return 0;
}