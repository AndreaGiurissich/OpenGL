#pragma once
#include "Vec3.h"
#include <iostream>

using namespace std;
class Mat3
{

public:

	float x11, x12, x13,
		  x21, x22, x23,
		  x31, x32, x33;

	Mat3();
	Mat3(float n11, float n12, float n13,
		float n21, float n22, float n23,
		float n31, float n32, float n33);
	Mat3(float val);
	Mat3(Vec3 v1, Vec3 v2, Vec3 v3);

	Vec3 getRiga(int i);
	Vec3 getCol(int i);
	float getElem(int i, int j);

	//assegnazione
	Mat3& operator+=(const Mat3& mat);
	Mat3& operator-=(const Mat3& mat);
	Mat3& operator*=(float a);
	Mat3& operator/=(float a);

	//aritmetica
	Mat3 operator+(const Mat3& mat);
	Mat3 operator-(const Mat3& mat);
	Mat3 operator*(float i);
	Mat3 operator/(float i);

	Vec3 operator*(const Vec3& v); //prodotto matrice vettore fatto con l'overload dell'operatore *
	Mat3 operator*(const Mat3& mat); //prodotto matrice matrice (riga per colonna)

	float det();
	Mat3 tras();
	Mat3 inversa();

	//stampa
	friend ostream& operator<<(ostream& out, const Mat3& v);

};



