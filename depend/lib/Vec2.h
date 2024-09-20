#pragma once
#include <iostream>

class Vec2
{
public:

	float x, y;

	friend std::ostream& operator <<(std::ostream& out, const Vec2& v);
	Vec2(float x, float y);
	Vec2(float val);
	Vec2();

	//Assegnazione
	Vec2& operator+=(const Vec2& v);
	Vec2& operator-=(const Vec2& v);
	Vec2& operator*=(const Vec2& v);
	Vec2& operator/=(const Vec2& v);

	//operatori booleani
	bool operator==(const Vec2& v) const;
	bool operator!=(const Vec2& v) const;

	//Aritmetica
	Vec2 operator+(Vec2 v) const;
	Vec2 operator-(Vec2 v);
	float operator*(Vec2 v);
	Vec2 operator*(const float i) const;

	//Funzioni
	float norm();
	Vec2 getNormalized();
	void normalize();
};


