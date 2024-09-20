#pragma once
#include "Vec2.h"
#include <ostream>

using namespace std;
class Mat2
{

public:
	float x11, x12, x21, x22;

	//costruttori
	Mat2();
	Mat2(float x1, float x2, float y1, float y2);
	Mat2(float var);
	Mat2(const Vec2& v1, const Vec2& v2);

	//get e set
	Vec2 getRiga(int i);
	Vec2 getCol(int i);
	float getElem(int i, int j);

	//assegnazione
	Mat2& operator+=(const Mat2& mat);
	Mat2& operator-=(const Mat2& mat);
	Mat2& operator*=(float a);
	Mat2& operator/=(float a);

	//aritmetica
	Mat2 operator+(const Mat2& mat);
	Mat2 operator-(const Mat2& mat);
	Mat2 operator*(float i);
	Mat2 operator/(float i);

	Vec2 operator*(const Vec2& v); //prodotto matrice vettore fatto con l'overload dell'operatore *
	Mat2 operator*(const Mat2& mat); //prodotto matrice matrice (riga per colonna)

	float det();
	Mat2 tras();
	Mat2 inversa();

	//stampa
	friend ostream& operator<<(ostream& out, const Mat2& v);

};


