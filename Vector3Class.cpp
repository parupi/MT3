#include "Vector3Class.h"

// 加算
Vector3Class Vector3Class::operator+(const Vector3Class& obj) const { return Vector3Class(x + obj.x, y + obj.y, z + obj.z); }

// 減算
Vector3Class Vector3Class::operator-(const Vector3Class& obj) const { return Vector3Class(x - obj.x, y - obj.y, z - obj.z); }

// 乗算
Vector3Class Vector3Class::operator*(const Vector3Class& obj) const { return Vector3Class(x * obj.x, y * obj.y, z * obj.z); }

// 乗算(スカラー倍)
Vector3Class Vector3Class::operator*(const float& scalar) const { return Vector3Class(x * scalar, y * scalar, z * scalar); }

// 除算
Vector3Class Vector3Class::operator/(const Vector3Class& obj) const { return Vector3Class(x / obj.x, y / obj.y, z / obj.z); }

// +=
Vector3Class& Vector3Class::operator+=(const Vector3Class& other) {
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

// -=
Vector3Class& Vector3Class::operator-=(const Vector3Class& other) {
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}

// *=
Vector3Class& Vector3Class::operator*=(const Vector3Class& other) {
	x *= other.x;
	y *= other.y;
	z *= other.z;
	return *this;
}

// /=
Vector3Class& Vector3Class::operator/=(const Vector3Class& other) {
	x /= other.x;
	y /= other.y;
	z /= other.z;
	return *this;
}

