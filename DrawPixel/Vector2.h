#pragma once

class Vector2 {	

	private:
		int x, y;

	public:
		Vector2(int x, int y);
		Vector2();
		void setX(int sx);
		void setY(int sy);
		int getX();
		int getY();
		void suma(Vector2 v1);
		void resta(Vector2 v1);
		int Punto(Vector2 v1);
		void Cruz(Vector2 v1);
		Vector2 operator+ (Vector2);
		Vector2 operator- (Vector2);
		Vector2 operator* (int x);
		float operator* (Vector2 param);
		//Vector2 operator= (Vector2);
		~Vector2();
};

