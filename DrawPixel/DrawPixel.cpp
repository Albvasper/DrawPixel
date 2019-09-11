#include "pch.h"
#include <SDL.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include "Matrix.h"
#include <iostream>

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;
bool init();
void close();
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

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
	gWindow = NULL;
	gRenderer = NULL;
	SDL_Quit();
}

//------------------------------Mis funciones-------------------------------------------
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
void DrawCircle(int r, int xc, int yc) {
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
//-------------------------------------------------------------------------------------

int main(int argc, char* args[]) {
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
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
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
			//---------------------------------------MENU---------------------------------------
			std::cout << "----------------------------" << std::endl;
			std::cout << "Welcome, what do you want to draw? " << std::endl;
			std::cout << "1) Draw a pixel" << std::endl;
			std::cout << "2) Draw a DDA line" << std::endl;
			std::cout << "3) Draw a bresenham line" << std::endl;
			std::cout << "4) Draw a bresenham circle" << std::endl;
			std::cout << "5) Draw a Bezier curve" << std::endl;
			std::cout << "----------------------------" << std::endl;
			short selector;
			std::cin >> selector;
			switch (selector) {
				// Draw a pixel
				case 1:
					int x0P, y0P;
					std::cout << "Point: " << std::endl;
					std::cout << "x: ";
					std::cin >> x0P;
					std::cout << "y: ";
					std::cin >> y0P;
					DrawPixelF(x0P, y0P);
					break;

				//Draw a line
				/*case 2:
					int x0L, y0L, x1L, y1L;
					std::cout << "First point: " << std::endl;
					std::cout << "x: ";
					std::cin >> x0L;
					std::cout << "y: ";
					std::cin >> y0L;
					std::cout << "Second point: " << std::endl;
					std::cout << "x: ";
					std::cin >> x1L;
					std::cout << "y: ";
					std::cin >> y1L;
					DrawLine(x0L, y0L, x1L, y1L);
					break;*/

				//Draw DDA Line
				case 2:
					int x0DDA, y0DDA, x1DDA, y1DDA;
					std::cout << "First point: " << std::endl;
					std::cout << "x: ";
					std::cin >> x0DDA;
					std::cout << "y: ";
					std::cin >> y0DDA;
					std::cout << "Second point: " << std::endl;
					std::cout << "x: ";
					std::cin >> x1DDA;
					std::cout << "y: ";
					std::cin >> y1DDA;
					DDALine(x0DDA, y0DDA, x1DDA, y1DDA);
					break;

				//Draw Bresenham line
				case 3:
					int x0BL, y0BL, x1BL, y1BL;
					std::cout << "First point: " << std::endl;
					std::cout << "x: ";
					std::cin >> x0BL;
					std::cout << "y: ";
					std::cin >> y0BL;
					std::cout << "Second point: " << std::endl;
					std::cout << "x: ";
					std::cin >> x1BL;
					std::cout << "y: ";
					std::cin >> y1BL;
					BresenhamL(x0BL, y0BL, x1BL, y1BL, 1, 1);
					break;

				//Draw bresenham circle
				case 4:
					int x0C, y0C, rC;
					std::cout << "Point of origin: " << std::endl;
					std::cout << "x: ";
					std::cin >> x0C;
					std::cout << "y: ";
					std::cin >> y0C;
					std::cout << "Radius: " << std::endl;
					std::cout << "r: ";
					std::cin >> rC;
					DrawCircle(rC, x0C, y0C);
					break;

						
				//Draw Bezier curve
				case 5:
					int xPoints[4];
					int yPoints[4];
					std::cout << "Enter x and y: " << std::endl;
					for (int i = 0; i < 4; i++) {
						std::cout << "x: ";
						std::cin >> xPoints[i];
						std::cout << "y: ";
						std::cin >> yPoints[i];
						std::cout << std::endl;
					}
					BezierCurve(xPoints, yPoints);
					break;

				default:
					break;
			}
			//-------------------------------------------------------------------------------------

			Matrix matrix1(2, 2);
			matrix1(0, 0) = 1;
			matrix1(0, 1) = 2;
			matrix1(1, 0) = 3;
			matrix1(1, 1) = 4;
			Matrix matrix2(2, 2);
			matrix2(0, 0) = 2;
			matrix2(0, 1) = 2;
			matrix2(1, 0) = 2;
			matrix2(1, 1) = 2;
			std::cout << "Matrix result: " << std::endl;
			std::cout << "(0,0): " << std::endl;
			std::cout << "(0,1): " << std::endl;
			std::cout << "(1,0): " << std::endl;
			std::cout << "(1,1): " << std::endl;
			
			SDL_RenderPresent(gRenderer);
		}
	}
	close();
	return 0;
}