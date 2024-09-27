#define _USE_MATH_DEFINES
#include <cmath>
#include "Mat4.h"
#include <iostream>

using namespace std;

Mat4::Mat4()
{
	m[0][0] = 1;
	m[0][1] = 0;
	m[0][2] = 0;
	m[0][3] = 0;

	m[1][0] = 0;
	m[1][1] = 1;
	m[1][2] = 0;
	m[1][3] = 0;

	m[2][0] = 0;
	m[2][1] = 0;
	m[2][2] = 1;
	m[2][3] = 0;

	m[3][0] = 0;
	m[3][1] = 0;
	m[3][2] = 0;
	m[3][3] = 1;
}

Mat4::Mat4(float n11, float n12, float n13, float n14, float n21, float n22, float n23, float n24, float n31,
	float n32, float n33, float n34, float n41, float n42, float n43, float n44)
{
	m[0][0] = n11;
	m[0][1] = n12;
	m[0][2] = n13;
	m[0][3] = n14;

	m[1][0] = n21;
	m[1][1] = n22;
	m[1][2] = n23;
	m[1][3] = n24;

	m[2][0] = n31;
	m[2][1] = n32;
	m[2][2] = n33;
	m[2][3] = n34;

	m[3][0] = n41;
	m[3][1] = n42;
	m[3][2] = n43;
	m[3][3] = n44;
}

Mat4::Mat4(float val)
{
	m[0][0] = val;
	m[0][1] = val;
	m[0][2] = val;
	m[0][3] = val;

	m[1][0] = val;
	m[1][1] = val;
	m[1][2] = val;
	m[1][3] = val;

	m[2][0] = val;
	m[2][1] = val;
	m[2][2] = val;
	m[2][3] = val;

	m[3][0] = val;
	m[3][1] = val;
	m[3][2] = val;
	m[3][3] = val;
}

Mat4::Mat4(Vec4 v1, Vec4 v2, Vec4 v3, Vec4 v4)
{
	m[0][0] = v1.x;
	m[1][0] = v1.y;
	m[2][0] = v1.z;
	m[3][0] = v1.w;

	m[0][1] = v2.x;
	m[1][1] = v2.y;
	m[2][1] = v2.z;
	m[3][1] = v2.w;
	 
	m[0][2] = v3.x;
	m[1][2] = v3.y;
	m[2][2] = v3.z;
	m[3][2] = v3.w;

	m[0][3] = v4.x;
	m[1][3] = v4.y;
	m[2][3] = v4.z;
	m[3][3] = v4.w;

}

Vec4 Mat4::getRiga(int i)
{
	Vec4 v(m[i][0], m[i][1], m[i][2], m[i][3]);
	return v;
}

Vec4 Mat4::getCol(int i)
{
	Vec4 v(m[0][i], m[1][i], m[2][i], m[3][i]);
	return v;
}

float Mat4::getElem(int i, int j)
{
	float elem = m[i][j];
	return elem;
}

Mat4& Mat4::operator+=(const Mat4& mat)
{
	for (int i = 0; i<4; i++)
	{
		for(int j = 0; j<4; j++)
		{
			m[i][j] += mat.m[i][j];
		}
	}
	
	return *this;
}

Mat4& Mat4::operator-=(const Mat4& mat)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m[i][j] -= mat.m[i][j];
		}
	}

	return *this;
}

Mat4& Mat4::operator*=(float a)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m[i][j] *= a;
		}
	}

	return *this;
}

Mat4& Mat4::operator/=(float a)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m[i][j] /= a;
		}
	}

	return *this;
}

Mat4 Mat4::operator+(const Mat4& mat)
{
	return { m[0][0] + mat.m[0][0], m[0][1] + mat.m[0][1], m[0][2] + mat.m[0][2], m[0][3] + mat.m[0][3],
			 m[1][0] + mat.m[1][0], m[1][1] + mat.m[1][1], m[1][2] + mat.m[1][2], m[1][3] + mat.m[1][3],
			 m[2][0] + mat.m[2][0], m[2][1] + mat.m[2][1], m[2][2] + mat.m[2][2], m[2][3] + mat.m[2][3],
			 m[3][0] + mat.m[3][0], m[3][1] + mat.m[3][1], m[3][2] + mat.m[3][2], m[3][3] + mat.m[3][3] };
}

Mat4 Mat4::operator-(const Mat4& mat)
{
	return { m[0][0] - mat.m[0][0], m[0][1] - mat.m[0][1], m[0][2] - mat.m[0][2], m[0][3] - mat.m[0][3],
			 m[1][0] - mat.m[1][0], m[1][1] - mat.m[1][1], m[1][2] - mat.m[1][2], m[1][3] - mat.m[1][3],
			 m[2][0] - mat.m[2][0], m[2][1] - mat.m[2][1], m[2][2] - mat.m[2][2], m[2][3] - mat.m[2][3],
			 m[3][0] - mat.m[3][0], m[3][1] - mat.m[3][1], m[3][2] - mat.m[3][2], m[3][3] - mat.m[3][3] };
}

Mat4 Mat4::operator*(float i)
{
	return { m[0][0] * i, m[0][1] * i, m[0][2] * i, m[0][3] * i,
			 m[1][0] * i, m[1][1] * i, m[1][2] * i, m[1][3] * i,
			 m[2][0] * i, m[2][1] * i, m[2][2] * i, m[2][3] * i,
			 m[3][0] * i, m[3][1] * i, m[3][2] * i, m[3][3] * i };

}

Mat4 Mat4::operator/(float i)
{
	return { m[0][0] / i, m[0][1] / i, m[0][2] / i, m[0][3] / i,
			 m[1][0] / i, m[1][1] / i, m[1][2] / i, m[1][3] / i,
			 m[2][0] / i, m[2][1] / i, m[2][2] / i, m[2][3] / i,
			 m[3][0] / i, m[3][1] / i, m[3][2] / i, m[3][3] / i };
}

Vec4 Mat4::operator*(const Vec4& v)
{
	return Vec4(m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w,
				m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w,
				m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w,
				m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w);
}


Mat4 Mat4::operator*(const Mat4& mat)
{
	float a0 = m[0][0] * mat.m[0][0] + m[0][1] * mat.m[1][0] + m[0][2] * mat.m[2][0] + m[0][3] * mat.m[3][0];
	float b0 = m[0][0] * mat.m[0][1] + m[0][1] * mat.m[1][1] + m[0][2] * mat.m[2][1] + m[0][3] * mat.m[3][1];
	float c0 = m[0][0] * mat.m[0][2] + m[0][1] * mat.m[1][2] + m[0][2] * mat.m[2][2] + m[0][3] * mat.m[3][2];
	float d0 = m[0][0] * mat.m[0][3] + m[0][1] * mat.m[1][3] + m[0][2] * mat.m[2][3] + m[0][3] * mat.m[3][3];

	float a1 = m[1][0] * mat.m[0][0] + m[1][1] * mat.m[1][0] + m[1][2] * mat.m[2][0] + m[1][3] * mat.m[3][0];
	float b1 = m[1][0] * mat.m[0][1] + m[1][1] * mat.m[1][1] + m[1][2] * mat.m[2][1] + m[1][3] * mat.m[3][1];
	float c1 = m[1][0] * mat.m[0][2] + m[1][1] * mat.m[1][2] + m[1][2] * mat.m[2][2] + m[1][3] * mat.m[3][2];
	float d1 = m[1][0] * mat.m[0][3] + m[1][1] * mat.m[1][3] + m[1][2] * mat.m[2][3] + m[1][3] * mat.m[3][3];

	float a2 = m[2][0] * mat.m[0][0] + m[2][1] * mat.m[1][0] + m[2][2] * mat.m[2][0] + m[2][3] * mat.m[3][0];
	float b2 = m[2][0] * mat.m[0][1] + m[2][1] * mat.m[1][1] + m[2][2] * mat.m[2][1] + m[2][3] * mat.m[3][1];
	float c2 = m[2][0] * mat.m[0][2] + m[2][1] * mat.m[1][2] + m[2][2] * mat.m[2][2] + m[2][3] * mat.m[3][2];
	float d2 = m[2][0] * mat.m[0][3] + m[2][1] * mat.m[1][3] + m[2][2] * mat.m[2][3] + m[2][3] * mat.m[3][3];

	float a3 = m[3][0] * mat.m[0][0] + m[3][1] * mat.m[1][0] + m[3][2] * mat.m[2][0] + m[3][3] * mat.m[3][0];
	float b3 = m[3][0] * mat.m[0][1] + m[3][1] * mat.m[1][1] + m[3][2] * mat.m[2][1] + m[3][3] * mat.m[3][1];
	float c3 = m[3][0] * mat.m[0][2] + m[3][1] * mat.m[1][2] + m[3][2] * mat.m[2][2] + m[3][3] * mat.m[3][2];
	float d3 = m[3][0] * mat.m[0][3] + m[3][1] * mat.m[1][3] + m[3][2] * mat.m[2][3] + m[3][3] * mat.m[3][3];

	return { a0, b0, c0, d0,
			 a1, b1, c1, d1,
			 a2, b2, c2, d2,
			 a3, b3, c3, d3 };
}

float Mat4::det()
{
	float a0 = m[0][0] * ((m[1][1] * m[2][2] * m[3][3]) + (m[1][2] * m[2][3] * m[3][1]) + (m[1][3] * m[2][1] * m[3][2]) - 
						  (m[1][3] * m[2][2] * m[3][1]) - (m[1][2] * m[2][1] * m[3][3]) - (m[1][1] * m[2][3] * m[3][2]));

	float a1 = m[0][1] * ((m[1][0] * m[2][2] * m[3][3]) + (m[1][2] * m[2][3] * m[3][0]) + (m[1][3] * m[2][0] * m[3][2]) -
					      (m[1][3] * m[2][2] * m[3][0]) - (m[1][2] * m[2][0] * m[3][3]) - (m[1][0] * m[2][3] * m[3][2]));

	float a2 = m[0][2] * ((m[1][0] * m[2][1] * m[3][3]) + (m[1][1] * m[2][3] * m[3][0]) + (m[1][3] * m[2][0] * m[3][1]) -
						  (m[1][3] * m[2][1] * m[3][0]) - (m[1][1] * m[2][0] * m[3][3]) - (m[1][0] * m[2][3] * m[3][1]));

	float a3 = m[0][3] * ((m[1][0] * m[2][1] * m[3][2]) + (m[1][1] * m[2][2] * m[3][0]) + (m[1][2] * m[2][0] * m[3][1]) -
						  (m[1][2] * m[2][1] * m[3][0]) - (m[1][1] * m[2][0] * m[3][2]) - (m[1][0] * m[2][2] * m[3][1]));

	float det = a0 - a1 + a2 - a3;
	return det;
}

Mat4 Mat4::tras()
{
	return { m[0][0], m[1][0], m[2][0], m[3][0],
			 m[0][1], m[1][1], m[2][1], m[3][1],
			 m[0][2], m[1][2], m[2][2], m[3][2],
			 m[0][3], m[1][3], m[2][3], m[3][3] };
}


//traslazione
Mat4 Mat4::translation(Vec3 d) {
	Mat4 mat(1.0f, 0.0f, 0.0f, d.x,
		     0.0f, 1.0f, 0.0f, d.y,
		     0.0f, 0.0f, 1.0f, d.z,
		     0.0f, 0.0f, 0.0f, 1.0f);
	return *this * mat;
}

//scala
Mat4 Mat4::scale( Vec3 s) {
	 Mat4 mat(s.x, 0.0f, 0.0f, 0.0f,
			  0.0f, s.y, 0.0f, 0.0f,
			  0.0f, 0.0f, s.z, 0.0f,
			  0.0f, 0.0f, 0.0f, 1.0f);
	 return *this * mat;
}

//rotazione
Mat4 Mat4::rotation(float degrees, Vec3 axis) {
	Mat4 result;

	float radians = degrees * (M_PI / 180.0f);
	float cosTheta = std::cos(radians);
	float sinTheta = std::sin(radians);
	float oneMinusCosTheta = 1.0f - cosTheta;

	float x = axis.x;
	float y = axis.y;
	float z = axis.z;

	result.m[0][0] = cosTheta + x * x * oneMinusCosTheta;
	result.m[0][1] = x * y * oneMinusCosTheta - z * sinTheta;
	result.m[0][2] = x * z * oneMinusCosTheta + y * sinTheta;
	result.m[0][3] = 0.0f;
		   
	result.m[1][0] = y * x * oneMinusCosTheta + z * sinTheta;
	result.m[1][1] = cosTheta + y * y * oneMinusCosTheta;
	result.m[1][2] = y * z * oneMinusCosTheta - x * sinTheta;
	result.m[1][3] = 0.0f;
		   
	result.m[2][0] = z * x * oneMinusCosTheta - y * sinTheta;
	result.m[2][1] = z * y * oneMinusCosTheta + x * sinTheta;
	result.m[2][2] = cosTheta + z * z * oneMinusCosTheta;
	result.m[2][3] = 0.0f;
		   
	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return *this * result;
}

//Perspective
Mat4 Mat4::perspective(float fov, float aspect, float near, float far) {

	Mat4 result;

	float yScale = 1.0 / tan((M_PI / 180) * fov / 2);
	float xScale = yScale / aspect;

	result.m[0][0] = xScale;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;
	result.m[1][0] = 0.0f;
	result.m[1][1] = yScale;
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;
	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = (far + near) / (near - far);
	result.m[2][3] = (2.0f * far * near) / (near - far);
	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = -1.0f;
	result.m[3][3] = 0.0f;

	return *this * result;
}

Mat4 Mat4::orthographic(float fov, float aspect, float near, float far) {

	Mat4 result;

	float yScale = 1.0 / near * (tan((M_PI / 180) * fov / 2));
	float xScale = yScale / aspect;

	result.m[0][0] = xScale;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;
	result.m[1][0] = 0.0f;
	result.m[1][1] = yScale;
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;
	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = 2.0f / (near - far);
	result.m[2][3] = (far + near) / (near - far);
	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return *this * result;
}

//LookAt
Mat4 Mat4::lookAt(Vec3 eye, Vec3 center, Vec3 up) {
	Vec3 forward = (eye - center).normalize();
	Vec3 right = (up.cross(forward)).normalize();
	Vec3 newUp = (forward.cross(right)).normalize();

	Mat4 result(
		right.x, right.y, right.z, -right.dot(eye),
		newUp.x, newUp.y, newUp.z, -newUp.dot(eye),
	    forward.x, forward.y, forward.z, -forward.dot(eye),
		0.0f, 0.0f, 0.0f, 1.0f
	);

	return *this * result;
}

//LookAt Cubemap
Mat4 Mat4::lookAtCubemap(Vec3 eye, Vec3 target, Vec3 up) {
	Vec3 forward = (target - eye).normalize();
	Vec3 right = (forward.cross(up)).normalize();
	Vec3 newUp = right.cross(forward);

	Mat4 result(
		right.x, right.y, right.z, 0.0f,
		newUp.x, newUp.y, newUp.z, 0.0f,
		-forward.x, -forward.y, -forward.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);

	return *this * result;
}

const float* Mat4::value_ptr() const {
	return &m[0][0];
}

Mat4 Mat4::inversa()
{
	float det = this->det();
	if (det == 0)
	{
		cout << "Matrice non invertibile" << endl;
		return {};
	}
	else
	{
		float a11 = ((m[1][1] * m[2][2] * m[3][3]) + (m[1][2] * m[2][3] * m[3][1]) + (m[1][3] * m[2][1] * m[3][2]) -
					 (m[1][3] * m[2][2] * m[3][1]) - (m[1][2] * m[2][1] * m[3][3]) - (m[1][1] * m[2][3] * m[3][2])) / det;
		float a12 = ((m[1][0] * m[2][2] * m[3][3]) + (m[1][2] * m[2][3] * m[3][0]) + (m[1][3] * m[2][0] * m[3][2]) -
					 (m[1][3] * m[2][2] * m[3][0]) - (m[1][2] * m[2][0] * m[3][3]) - (m[1][0] * m[2][3] * m[3][2])) / det;
		float a13 = ((m[1][0] * m[2][1] * m[3][3]) + (m[1][1] * m[2][3] * m[3][0]) + (m[1][3] * m[2][0] * m[3][1]) -
					 (m[1][3] * m[2][1] * m[3][0]) - (m[1][1] * m[2][0] * m[3][3]) - (m[1][0] * m[2][3] * m[3][1])) / det;
		float a14 = ((m[1][0] * m[2][1] * m[3][2]) + (m[1][1] * m[2][2] * m[3][0]) + (m[1][2] * m[2][0] * m[3][1]) -
					 (m[1][2] * m[2][1] * m[3][0]) - (m[1][1] * m[2][0] * m[3][2]) - (m[1][0] * m[2][2] * m[3][1])) / det;

		float a21 = ((m[0][1] * m[2][2] * m[3][3]) + (m[0][2] * m[2][3] * m[3][1]) + (m[0][3] * m[2][1] * m[3][2]) -
					 (m[0][3] * m[2][2] * m[3][1]) - (m[0][2] * m[2][1] * m[3][3]) - (m[0][1] * m[2][3] * m[3][2])) / det;
		float a22 = ((m[0][0] * m[2][2] * m[3][3]) + (m[0][2] * m[2][3] * m[3][0]) + (m[0][3] * m[2][0] * m[3][2]) -
					 (m[0][3] * m[2][2] * m[3][0]) - (m[0][2] * m[2][0] * m[3][3]) - (m[0][0] * m[2][3] * m[3][2])) / det;
		float a23 = ((m[0][0] * m[2][1] * m[3][3]) + (m[0][1] * m[2][3] * m[3][0]) + (m[0][3] * m[2][0] * m[3][1]) -
					 (m[0][3] * m[2][1] * m[3][0]) - (m[0][1] * m[2][0] * m[3][3]) - (m[0][0] * m[2][3] * m[3][1])) / det;
		float a24 = ((m[0][0] * m[2][1] * m[3][2]) + (m[0][1] * m[2][2] * m[3][0]) + (m[0][2] * m[2][0] * m[3][1]) -
					 (m[0][2] * m[2][1] * m[3][0]) - (m[0][1] * m[2][0] * m[3][2]) - (m[0][0] * m[2][2] * m[3][1])) / det;

		float a31 = ((m[0][1] * m[1][2] * m[3][3]) + (m[0][2] * m[1][3] * m[3][1]) + (m[0][3] * m[1][1] * m[3][2]) -
					 (m[0][3] * m[1][2] * m[3][1]) - (m[0][2] * m[1][1] * m[3][3]) - (m[0][1] * m[1][3] * m[3][2])) / det;
		float a32 = ((m[0][0] * m[1][2] * m[3][3]) + (m[0][2] * m[1][3] * m[3][0]) + (m[0][3] * m[1][0] * m[3][2]) -
					 (m[0][3] * m[1][2] * m[3][0]) - (m[0][2] * m[1][0] * m[3][3]) - (m[0][0] * m[1][3] * m[3][2])) / det;
		float a33 = ((m[0][0] * m[1][1] * m[3][3]) + (m[0][1] * m[1][3] * m[3][0]) + (m[0][3] * m[1][0] * m[3][1]) -
					 (m[0][3] * m[1][1] * m[3][0]) - (m[0][1] * m[1][0] * m[3][3]) - (m[0][0] * m[1][3] * m[3][1])) / det;
		float a34 = ((m[0][0] * m[1][1] * m[3][2]) + (m[0][1] * m[1][2] * m[3][0]) + (m[0][2] * m[1][0] * m[3][1]) -
					 (m[0][2] * m[1][1] * m[3][0]) - (m[0][1] * m[1][0] * m[3][2]) - (m[0][0] * m[1][2] * m[3][1])) / det;

		float a41 = ((m[0][1] * m[1][2] * m[2][3]) + (m[0][2] * m[1][3] * m[2][1]) + (m[0][3] * m[1][1] * m[2][2]) -
					 (m[0][3] * m[1][2] * m[2][1]) - (m[0][2] * m[1][1] * m[2][3]) - (m[0][1] * m[1][3] * m[2][2])) / det;
		float a42 = ((m[0][0] * m[1][2] * m[2][3]) + (m[0][2] * m[1][3] * m[2][0]) + (m[0][3] * m[1][0] * m[2][2]) -
					 (m[0][3] * m[1][2] * m[2][0]) - (m[0][2] * m[1][0] * m[2][3]) - (m[0][0] * m[1][3] * m[2][2])) / det;
		float a43 = ((m[0][0] * m[1][1] * m[2][3]) + (m[0][1] * m[1][3] * m[2][0]) + (m[0][3] * m[1][0] * m[2][1]) -
					 (m[0][3] * m[1][1] * m[2][0]) - (m[0][1] * m[1][0] * m[2][3]) - (m[0][0] * m[1][3] * m[2][1])) / det;
		float a44 = ((m[0][0] * m[1][1] * m[2][2]) + (m[0][1] * m[1][2] * m[2][0]) + (m[0][2] * m[1][0] * m[2][1]) -
					 (m[0][2] * m[1][1] * m[2][0]) - (m[0][1] * m[1][0] * m[2][2]) - (m[0][0] * m[1][2] * m[2][1])) / det;

		Mat4 mat  = Mat4(a11, a12, a13, a14,
					     a21, a22, a23, a24,
					     a31, a32, a33, a34,
					     a41, a42, a43, a44);

		return mat.tras();
	}
}

ostream& operator<<(ostream& out, const Mat4& v)
{
	
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			out << " "<< v.m[i][j] << " s";
		}
		out << "\n";
	}
	return out;
}
