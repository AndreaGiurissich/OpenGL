#include "Mat3.h"
#include <iostream>

using namespace std;

Mat3::Mat3()
{
	x11 = 1;
	x12 = 0;
	x13 = 0;
	x21 = 0;
	x22 = 1;
	x23 = 0;
	x31 = 0;
	x32 = 0;
	x33 = 1;
}

Mat3::Mat3(float n11, float n12, float n13,
	float n21, float n22, float n23,
	float n31, float n32, float n33)
{
	x11 = n11;
	x12 = n12;
	x13 = n13;
	x21 = n21;
	x22 = n22;
	x23 = n23;
	x31 = n31;
	x32 = n32;
	x33 = n33;
}

Mat3::Mat3(float val)
{
	x11 = val;
	x12 = val;
	x13 = val;
	x21 = val;
	x22 = val;
	x23 = val;
	x31 = val;
	x32 = val;
	x33 = val;
}

Mat3::Mat3(Vec3 v1, Vec3 v2, Vec3 v3)
{
	x11 = v1.x;
	x21 = v2.y;
	x31 = v3.z;
	x12 = v1.x;
	x22 = v2.y;
	x32 = v3.z;
	x13 = v1.x;
	x23 = v2.y;
	x33 = v3.z;
}

Vec3 Mat3::getRiga(int i)
{
	if (i == 1)
	{
		Vec3 v(x11, x12, x13);
		return v;
	}
	if (i == 2)
	{
		Vec3 v(x21, x22, x23);
		return v;
	}
	if (i == 3)
	{
		Vec3 v(x31, x32, x33);
		return v;
	}
}

Vec3 Mat3::getCol(int i)
{
	if (i == 1)
	{
		Vec3 v(x11, x21, x31);
		return v;
	}
	if (i == 2)
	{
		Vec3 v(x12, x22, x32);
		return v;
	}
	if (i == 3)
	{
		Vec3 v(x13, x23, x33);
		return v;
	}
}

float Mat3::getElem(int i, int j)
{
	if (i == 1 && j == 1)
		return x11;
	if (i == 1 && j == 2)
		return x12;
	if (i == 1 && j == 3)
		return x13;
	if (i == 2 && j == 1)
		return x21;
	if (i == 2 && j == 2)
		return x22;
	if (i == 2 && j == 3)
		return x23;
	if (i == 3 && j == 1)
		return x31;
	if (i == 3 && j == 2)
		return x32;
	if (i == 3 && j == 3)
		return x33;
}

Mat3& Mat3::operator+=(const Mat3& mat)
{
	x11 += mat.x11;
	x12 += mat.x12;
	x13 += mat.x13;
	x21 += mat.x21;
	x22 += mat.x22;
	x23 += mat.x23;
	x31 += mat.x31;
	x32 += mat.x32;
	x33 += mat.x33;

	return *this;
}

Mat3& Mat3::operator-=(const Mat3& mat)
{
	x11 -= mat.x11;
	x12 -= mat.x12;
	x13 -= mat.x13;
	x21 -= mat.x21;
	x22 -= mat.x22;
	x23 -= mat.x23;
	x31 -= mat.x31;
	x32 -= mat.x32;
	x33 -= mat.x33;

	return *this;
}

Mat3& Mat3::operator*=(float a)
{
	x11 *= a;
	x12 *= a;
	x13 *= a;
	x21 *= a;
	x22 *= a;
	x23 *= a;
	x31 *= a;
	x32 *= a;
	x33 *= a;

	return *this;
}

Mat3& Mat3::operator/=(float a)
{
	x11 /= a;
	x12 /= a;
	x13 /= a;
	x21 /= a;
	x22 /= a;
	x23 /= a;
	x31 /= a;
	x32 /= a;
	x33 /= a;

	return *this;
}

Mat3 Mat3::operator+(const Mat3& mat)
{
	return{ x11 + mat.x11, x12 + mat.x12, x13 + mat.x13,
			x21 + mat.x21, x22 + mat.x22, x23 + mat.x23,
			x31 + mat.x31, x32 + mat.x32, x33 + mat.x33 };
}

Mat3 Mat3::operator-(const Mat3& mat)
{
	return{ x11 - mat.x11, x12 - mat.x12, x13 - mat.x13,
			x21 - mat.x21, x22 - mat.x22, x23 - mat.x23,
			x31 - mat.x31, x32 - mat.x32, x33 - mat.x33 };
}

Mat3 Mat3::operator*(float i)
{
	return{ x11 * i, x12 * i, x13 * i,
			x21 * i, x22 * i, x23 * i,
			x31 * i, x32 * i, x33 * i };
}

Mat3 Mat3::operator/(float i)
{
	return{ x11 / i, x12 / i, x13 / i,
			x21 / i, x22 / i, x23 / i,
			x31 / i, x32 / i, x33 / i };
}

Vec3 Mat3::operator*(const Vec3& v)
{
	return Vec3(x11 * v.x + x12 * v.y + x13 * v.z,
		x21 * v.x + x22 * v.y + x23 * v.z,
		x31 * v.x + x32 * v.y + x33 * v.z);
}

Mat3 Mat3::operator*(const Mat3& mat)
{
	return{ x11 * mat.x11 + x12 * mat.x21 + x13 * mat.x31, x11 * mat.x12 + x12 * mat.x22 + x13 * mat.x32, x11 * mat.x13 + x12 * mat.x23 + x13 * mat.x33,
			x21 * mat.x11 + x22 * mat.x21 + x23 * mat.x31, x21 * mat.x12 + x22 * mat.x22 + x23 * mat.x32, x21 * mat.x13 + x22 * mat.x23 + x23 * mat.x33,
			x31 * mat.x11 + x32 * mat.x21 + x33 * mat.x31, x31 * mat.x12 + x32 * mat.x22 + x33 * mat.x32, x31 * mat.x13 + x32 * mat.x23 + x33 * mat.x33 };
}

float Mat3::det()
{
	return x11 * x22 * x33 + x12 * x23 * x31 + x13 * x21 * x32 - x13 * x22 * x31 - x12 * x21 * x33 - x11 * x23 * x32;
}

Mat3 Mat3::tras()
{
	return Mat3(x11, x21, x31, 
				 x12, x22, x32, 
				 x13, x23, x33);
}

Mat3 Mat3::inversa()
{
	float det = this->det();
	if (det == 0)
	{
		cout << "Matrice non invertibile" << endl;
		return Mat3();
	}
	else
	{
		return Mat3(x22 * x33 - x23 * x32, x13 * x32 - x12 * x33, x12 * x23 - x13 * x22,
			x23 * x31 - x21 * x33, x11 * x33 - x13 * x31, x13 * x21 - x11 * x23,
			x21 * x32 - x22 * x31, x12 * x31 - x11 * x32, x11 * x22 - x12 * x21) / det;
	}
}

ostream& operator<<(ostream& out, const Mat3& v)
{
	out << "| " << v.x11 << " " << v.x12 << " " << v.x13 << " |\n";
	out << "| " << v.x21 << " " << v.x22 << " " << v.x23 << " |\n";
	out << "| " << v.x31 << " " << v.x32 << " " << v.x33 << " |\n";

	return out;
}
