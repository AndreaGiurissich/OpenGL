#include "Mat2.h"
#include <iostream>

using namespace std;

Mat2::Mat2()
{
	x11 = 1;
	x12 = 0;
	x21 = 0;
	x22 = 1;
}

Mat2::Mat2(float x1, float x2, float y1, float y2)
{
	x11 = x1;
	x12 = x2;
	x21 = y1;
	x22 = y2;
}

Mat2::Mat2(float var)
{
	x11 = var;
	x12 = var;
	x21 = var;
	x22 = var;
}

Mat2::Mat2(const Vec2& v1, const Vec2& v2)
{
	x11 = v1.x;
	x21 = v2.y;
	x12 = v1.x;
	x22 = v2.y;
}

Vec2 Mat2::getRiga(int i)
{
	if (i == 1)
	{
		Vec2 v(x11, x12);
		return v;
	}
	if (i == 2)
	{
		Vec2 v(x21, x22);
		return v;
	}
}

Vec2 Mat2::getCol(int i)
{
	if (i == 1)
	{
		Vec2 v(x11, x21);
		return v;
	}
	if (i == 2)
	{
		Vec2 v(x12, x22);
		return v;
	}
}

float Mat2::getElem(int i, int j)
{
	if (i == 1 && j == 1)
		return x11;
	if (i == 1 && j == 2)
		return x12;
	if (i == 2 && j == 1)
		return x21;
	if (i == 2 && j == 2)
		return x22;
}

Mat2& Mat2::operator+=(const Mat2& mat)
{
	x11 += mat.x11;
	x12 += mat.x12;
	x21 += mat.x21;
	x22 += mat.x22;

	return *this;
}

Mat2& Mat2::operator-=(const Mat2& mat)
{
	x11 -= mat.x11;
	x12 -= mat.x12;
	x21 -= mat.x21;
	x22 -= mat.x22;

	return *this;
}

Mat2& Mat2::operator*=(float a)
{
	x11 *= a;
	x12 *= a;
	x21 *= a;
	x22 *= a;

	return *this;
}

Mat2& Mat2::operator/=(float a)
{
	x11 /= a;
	x12 /= a;
	x21 /= a;
	x22 /= a;

	return *this;
}

Mat2 Mat2::operator+(const Mat2& mat)
{
	return { x11 + mat.x11, x12 + mat.x12, x21 + mat.x21, x22 + mat.x22 };
}

Mat2 Mat2::operator-(const Mat2& mat)
{
	return { x11 - mat.x11, x12 - mat.x12, x21 - mat.x21, x22 - mat.x22 };
}

Mat2 Mat2::operator*(float i)
{
	return { x11 * i , x12 * i, x21 * i, x22 * i };
}

Mat2 Mat2::operator/(float i)
{
	return { x11 / i , x12 / i, x21 / i, x22 / i };
}

Vec2 Mat2::operator*(const Vec2& v)
{
	return Vec2(x11 * v.x + x12 * v.y, x21 * v.x + x22 * v.y);
}

Mat2 Mat2::operator*(const Mat2& mat)
{
	return { x11 * mat.x11 + x12 * mat.x21, x11 * mat.x12 + x12 * mat.x22,
			x21 * mat.x11 + x22 * mat.x21, x21 * mat.x12 + x22 * mat.x22 };
}

float Mat2::det()
{
	return x11 * x22 - x12 * x21;
}

Mat2 Mat2::tras()
{
	return { x11, x21, x12, x22 };
}

Mat2 Mat2::inversa()
{
	float det = this->det();
	if (det == 0)
	{
		std::cout << "Matrice non invertibile" << endl;
		return *this;
	}
	else
	{
		return Mat2(x22, -x12, -x21, x11) / det;
	}
}

ostream& operator<<(ostream& out, const Mat2& v)
{
	out << "| " << v.x11 << " " << v.x12 << " |\n";
	out << "| " << v.x21 << " " << v.x22 << " |\n";
	return out;
}
