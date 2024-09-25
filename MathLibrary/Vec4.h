#pragma once
#include <iostream>

class Vec4
{
public:

	float x, y, z, w;

	Vec4(float x, float y, float z, float w);
	Vec4(float val);
	Vec4();

	//Assegnazione
	Vec4& operator+=(const Vec4& v);
	Vec4& operator-=(const Vec4& v);
	Vec4& operator*=(const Vec4& v);
	Vec4& operator/=(const Vec4& v);

	//Bool operator
	bool operator==(const Vec4& v) const;
	bool operator!=(const Vec4& v) const;

	//Aritmetica
	Vec4 operator+(Vec4 v) const;
	Vec4 operator-(Vec4 v);
	float operator*(Vec4 v);
	Vec4 operator*(const float i) const;

	//Funzioni
	float norm();
	Vec4 getNormalized();
	void normalize();
	void print();

	friend std::ostream& operator<<(std::ostream& out, const Vec4& v);

};


