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
		gWindow = SDL_CreateWindow("Draw Pixel SDL!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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
	SDL_DestroyTexture(texture);
	gWindow = NULL;
	gRenderer = NULL;
	TTF_Quit();
	SDL_Quit();
}

//-------------------------------------------------------------------------
//Draw a single pixel
void DrawPixelF(int x, int y) {
	Vector2 v(x, y);
	SDL_SetRenderDrawColor(gRenderer, 8, 174, 234, 0xFF);
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
void BresenhamL(int x0, int y0, int x1, int y1, int incrX, int incrY) {
	int dx, dy, p, x, y;

	dx = x1 - x0;
	dy = y1 - y0;
	x = x0;
	y = y0;
	p = 2 * dy - dx;
	while (x < x1) {
		if (p >= 0) {
			DrawPixelF(x, y);
			y = y + 1;
			p = p + 2 * dy - 2 * dx;
		}
		else {
			DrawPixelF(x, y);
			p = p + 2 * dy;
		}
		x = x + 1;
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
	Vector2 centerV(int xc, int yc);
	int x = 0;
	int y = 0;
	Vector2 pixel(x, y);
	int p;
	x = 0;
	y = r;
	p = (5 / 4) - r;
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
	SDL_RenderFillRect(gRenderer, &rect);

	SDL_SetRenderDrawColor(gRenderer, 8, 174, 234, 0x00);
	SDL_Rect pixelButton;
	pixelButton.x = 400;
	pixelButton.y = 10;
	pixelButton.w = 100;
	pixelButton.h = 35;
	SDL_RenderFillRect(gRenderer, &pixelButton);

	SDL_SetRenderDrawColor(gRenderer, 8, 174, 234, 0x00);
	SDL_Rect ddaButton;
	ddaButton.x = 520;
	ddaButton.y = 10;
	ddaButton.w = 100;
	ddaButton.h = 35;
	SDL_RenderFillRect(gRenderer, &ddaButton);

	SDL_SetRenderDrawColor(gRenderer, 8, 174, 234, 0x00);
	SDL_Rect brshmButton;
	brshmButton.x = 640;
	brshmButton.y = 10;
	brshmButton.w = 100;
	brshmButton.h = 35;
	SDL_RenderFillRect(gRenderer, &brshmButton);

	SDL_SetRenderDrawColor(gRenderer, 8, 174, 234, 0x00);
	SDL_Rect bezierButton;
	bezierButton.x = 760;
	bezierButton.y = 10;
	bezierButton.w = 100;
	bezierButton.h = 35;
	SDL_RenderFillRect(gRenderer, &bezierButton);

	SDL_SetRenderDrawColor(gRenderer, 8, 174, 234, 0x00);
	SDL_Rect circleButton;
	circleButton.x = 880;
	circleButton.y = 10;
	circleButton.w = 100;
	circleButton.h = 35;
	SDL_RenderFillRect(gRenderer, &circleButton);

	SDL_SetRenderDrawColor(gRenderer, 42, 245, 152, 0x00);
	SDL_Rect clearButton;
	clearButton.x = 1810;
	clearButton.y = 10;
	clearButton.w = 100;
	clearButton.h = 35;
	SDL_RenderFillRect(gRenderer, &clearButton);

	//SDL_RenderDrawRect(gRenderer, &rect);
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
	TTF_Init();
	SDL_Color blueC = { 8, 174, 234 };
	SDL_Color whiteC = { 255, 255, 255 };

	Text t(gRenderer, "arial.ttf", 45 , "Draw Pixel!", blueC);
	t.Display(10, 0, gRenderer);

	Text b1(gRenderer, "arial.ttf", 25, "Pixel", whiteC);
	b1.Display(420, 14, gRenderer);

	Text b2(gRenderer, "arial.ttf", 25, "DDA", whiteC);
	b2.Display(540, 14, gRenderer);

	Text b3(gRenderer, "arial.ttf", 25, "Brshm", whiteC);
	b3.Display(655, 14, gRenderer);

	Text b4(gRenderer, "arial.ttf", 25, "Bezier", whiteC);
	b4.Display(775, 14, gRenderer);

	Text b5(gRenderer, "arial.ttf", 25, "Circle", whiteC);
	b5.Display(900, 14, gRenderer);

	Text b6(gRenderer, "arial.ttf", 25, "X", whiteC);
	b6.Display(1850, 14, gRenderer);
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
	int mouseX1;
	int mouseY1;
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
	int ifcounterSelector = 0;
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
					if (e.button.button == SDL_BUTTON_LEFT) {
						leftMouseButtonDown = true;
					}

					if (leftMouseButtonDown == true) {
						mouseX = e.motion.x;
						mouseY = e.motion.y;
						//Pixel Button
						if (mouseX > 400 && mouseX < 500 && mouseY > 10 && mouseY < 45) {
							ifcounterSelector = 1;
						}

						//DDA Button
						if (mouseX > 520 && mouseX < 620 && mouseY > 10 && mouseY < 45) {
							ifcounterSelector = 2;
						}

						//Bresenham Button
						if (mouseX > 640 && mouseX < 740 && mouseY > 10 && mouseY < 45) {
							ifcounterSelector = 3;
						}

						//Bezier Button
						if (mouseX > 760 && mouseX < 860 && mouseY > 10 && mouseY < 45) {
							ifcounterSelector = 4;
						}

						//Cricle Button
						if (mouseX > 880 && mouseX < 980 && mouseY > 10 && mouseY < 45) {
							ifcounterSelector = 5;
						}

						//Clear Button
						if (mouseX > 1810 && mouseX < 1910 && mouseY > 10 && mouseY < 45) {
							close();
							return 0;
						}

						switch (ifcounterSelector) {
							case 1:
								counter = counter + 1;
								if (counter == 0) {
									mouseX1 = e.motion.x - (SCREEN_WIDTH / 2);
									mouseY1 = -e.motion.y + (SCREEN_HEIGHT / 2);
									DrawPixelF(mouseX1, mouseY1);
									counter = -1;
									modeSelector = 1;
								}
								break;
							case 2:
								counter = counter + 1;
								if (counter == 0) {
									mouseX1 = e.motion.x - (SCREEN_WIDTH / 2);
									mouseY1 = -e.motion.y + (SCREEN_HEIGHT / 2);
									DrawPixelF(mouseX1, mouseY1);
								}

								if (counter == 1) {
									mouseX2 = e.motion.x - (SCREEN_WIDTH / 2);
									mouseY2 = -e.motion.y + (SCREEN_HEIGHT / 2);
									DrawPixelF(mouseX2, mouseY2);
									counter = -1;
									modeSelector = 2;
								}
								break;

							case 3:
								counter = counter + 1;
								if (counter == 0) {
									mouseX1 = e.motion.x - (SCREEN_WIDTH / 2);
									mouseY1 = -e.motion.y + (SCREEN_HEIGHT / 2);
									DrawPixelF(mouseX1, mouseY1);
								}

								if (counter == 1) {
									mouseX2 = e.motion.x - (SCREEN_WIDTH / 2);
									mouseY2 = -e.motion.y + (SCREEN_HEIGHT / 2);
									DrawPixelF(mouseX2, mouseY2);
									counter = -1;
									modeSelector = 3;
								}
								break;
							
							case 4:
								counter = counter + 1;
								if (counter == 0) {
									mouseX1 = e.motion.x - (SCREEN_WIDTH / 2);
									mouseY1 = -e.motion.y + (SCREEN_HEIGHT / 2);
									x[0] = mouseX1;
									y[0] = mouseY1;
									DrawPixelF(mouseX1, mouseY1);
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
									modeSelector = 4;
								}
								break;
							case 5:
								counter = counter + 1;
								if (counter == 0) {
									mouseX1 = e.motion.x - (SCREEN_WIDTH / 2);
									mouseY1 = -e.motion.y + (SCREEN_HEIGHT / 2);
									DrawPixelF(mouseX1, mouseY1);
								}

								if (counter == 1) {
									mouseX2 = e.motion.x - (SCREEN_WIDTH / 2);
									mouseY2 = -e.motion.y + (SCREEN_HEIGHT / 2);
									DrawPixelF(mouseX2, mouseY2);
									counter = -1;
									modeSelector = 5;
								}
								break;
						}

						switch (modeSelector) {
							case 1:
								DrawPixelF(mouseX1, mouseY1);
								modeSelector = 0;
								break;

							case 2:
								DDALine(mouseX1, mouseY1, mouseX2, mouseY2);
								modeSelector = 0;
								break;

							case 3:
								BresenhamL(mouseX1, mouseY1, mouseX2, mouseY2, 1, 1);
								modeSelector = 0;
								break;

							case 4:
								BezierCurve(x, y);
								modeSelector = 0;
								break;

							case 5:
								DrawCircle(mouseX1, mouseY1, mouseX2 - mouseX1);
								modeSelector = 0;
								break;
						}
					}
				break;

				case SDL_MOUSEMOTION:
					break;
			}
			DrawAxis();	
			DrawMenu();
			Matrix m(2, 2);
			m.setRows(2);
			m.setCols(2);
			m(0, 0) = 1;
			m(0, 1) = 2;
			m(1, 0) = 3;
			m(1, 1) = 4;
			Matrix m1(2, 2);
			m1.setRows(2);
			m1.setCols(2);
			m1(0, 0) = 4;
			m1(0, 1) = 3;
			m1(1, 0) = 2;
			m1(1, 1) = 1;
			m.plus(m, m1);
			SDL_RenderCopy(gRenderer, texture, NULL, NULL);
			SDL_RenderPresent(gRenderer);
		}

	}
	close();
	return 0;
}
