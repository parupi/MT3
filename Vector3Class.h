#pragma once
#include <cmath>

class Vector3Class {
public:
	float x;
	float y;
	float z;

	Vector3Class(float x_ = 0, float y_ = 0, float z_ = 0) : x(x_), y(y_), z(z_) {}

	// 加算
	Vector3Class operator+(const Vector3Class& obj) const;
	// 減算
	Vector3Class operator-(const Vector3Class& obj) const;
	// 乗算
	Vector3Class operator*(const Vector3Class& obj) const;
	// 乗算(スカラー倍)
	Vector3Class operator*(const float& scalar) const;
	// 除算
	Vector3Class operator/(const Vector3Class& obj) const;
	// +=
	Vector3Class& operator+=(const Vector3Class& obj);
	// -=
	Vector3Class& operator-=(const Vector3Class& obj);
	// *=
	Vector3Class& operator*=(const Vector3Class& obj);
	// /=
	Vector3Class& operator/=(const Vector3Class& obj);
};