#pragma once
#include "Vec4.h"
#include <iostream>

#include "Vec3.h"

using namespace std;
class Mat4
{
public:

	float m[4][4];

	Mat4();
	Mat4(float n11, float n12, float n13, float n14,
		float n21, float n22, float n23, float n24,
		float n31, float n32, float n33, float n34,
		float n41, float n42, float n43, float n44);
	Mat4(float val);
	Mat4(Vec4 v1, Vec4 v2, Vec4 v3, Vec4 v4);

	Vec4 getRiga(int i);
	Vec4 getCol(int i);
	float getElem(int i, int j);

	//assegnazione
	Mat4& operator+=(const Mat4& mat);
	Mat4& operator-=(const Mat4& mat);
	Mat4& operator*=(float a);
	Mat4& operator/=(float a);

	//aritmetica
	Mat4 operator+(const Mat4& mat);
	Mat4 operator-(const Mat4& mat);
	Mat4 operator*(float i);
	Mat4 operator/(float i);

	Vec4 operator*(const Vec4& v); //prodotto matrice vettore fatto con l'overload dell'operatore *
	Mat4 operator*(const Mat4& mat); //prodotto matrice matrice (riga per colonna)

	float det();
	Mat4 tras();
	Mat4 inversa();

	Mat4 traslazione(float x, float y, float z);
	Mat4 scala(float x, float y, float z);
	Mat4 rotazioneX(float angolo);
	Mat4 rotazioneY(float angolo);
	Mat4 rotazioneZ(float angolo);
	Mat4 persp(float fovy, float width, float height, float near, float far);

	Mat4 lookAt(Vec3 eye, Vec3 center, Vec3 up);

	//stampa
	friend ostream& operator<<(ostream& out, const Mat4& v);

};
