#include "pch.h"
#include "Vector2.h"


Vector2::Vector2(int x, int y)
{
	this->x = x;
	this->y = y;
	
}

Vector2::Vector2()
{	

}

void Vector2::setX(int sx) 
{
	x = sx;
}

void Vector2::setY(int sy)
{
	y = sy;
}


int Vector2::getX() 
{
	return x;
}

int Vector2::getY() 
{
	return y;
}


void Vector2::suma(Vector2 v1) 
{
	this->x = this->x + v1.x;
	this->y = this->y + v1.y;
}

void Vector2::resta(Vector2 v1)
{
	x = x - v1.x;
	y = y - v1.x;
}

int Vector2::Punto(Vector2 v1)
{
	return (x * v1.x) + (y * v1.y);
}

void Vector2::Cruz(Vector2 v1) 
{

}

Vector2 Vector2::operator+ (Vector2 param)
{
	Vector2 temp;
	temp.x = x + param.x;
	temp.y = y + param.y;
	return temp;
}

Vector2 Vector2::operator- (Vector2 param)
{
	Vector2 temp;
	temp.x = x - param.x;
	temp.y = y - param.y;
	return temp;
}

float Vector2:: operator* (Vector2 param)  {
	int temp;
	temp = (x * param.x) + (y * param.y);
	return temp;
}

Vector2 Vector2::operator*(int x) {
	Vector2 temp;
	temp.x = x * this->x;
	temp.y = y * this->y;
	return temp;
}

//Vector2 Vector2::operator= (Vector2 param) 
//{
//
//}

Vector2::~Vector2() {
}
