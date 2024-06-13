#define NOMINMAX
#include "function.h"

void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label)
{
	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", vector.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", vector.z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%s", label);
}

Vector3 Add(const Vector3& v1, const Vector3& v2)
{
	return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

Vector3 Subtract(const Vector3& v1, const Vector3& v2)
{
	return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

Vector3 Multiply(float scalar, const Vector3& v)
{
	return Vector3(scalar * v.x, scalar * v.y, scalar * v.z);
}

float Dot(const Vector3& v1, const Vector3& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float Length(const Vector3& v)
{
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

float MagnitudeSquared(const Vector3& v)
{
	return (v.x * v.x) + (v.y * v.y) + (v.z * v.z);
}

Vector3 Normalize(const Vector3& v)
{
	return Vector3(v.x / Length(v), v.y / Length(v), v.z / Length(v));
}

void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label)
{
	Novice::ScreenPrintf(x, y, label);
	for (int row = 0; row < 4; row++) {
		for (int column = 0; column < 4; column++) {
			Novice::ScreenPrintf(x + column * kColumnWidth, y + (row + 1) * kRowHeight, "%6.02f", matrix.m[row][column]);
		}
	}
}

Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2)
{
	return {
		m1.m[0][0] + m2.m[0][0],m1.m[0][1] + m2.m[0][1],m1.m[0][2] + m2.m[0][2],m1.m[0][3] + m2.m[0][3],
		m1.m[1][0] + m2.m[1][0],m1.m[1][1] + m2.m[1][1],m1.m[1][2] + m2.m[1][2],m1.m[1][3] + m2.m[1][3],
		m1.m[2][0] + m2.m[2][0],m1.m[2][1] + m2.m[2][1],m1.m[2][2] + m2.m[2][2],m1.m[2][3] + m2.m[2][3],
		m1.m[3][0] + m2.m[3][0],m1.m[3][1] + m2.m[3][1],m1.m[3][2] + m2.m[3][2],m1.m[3][3] + m2.m[3][3]
	};
}

Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2)
{
	return {
		m1.m[0][0] - m2.m[0][0],m1.m[0][1] - m2.m[0][1],m1.m[0][2] - m2.m[0][2],m1.m[0][3] - m2.m[0][3],
		m1.m[1][0] - m2.m[1][0],m1.m[1][1] - m2.m[1][1],m1.m[1][2] - m2.m[1][2],m1.m[1][3] - m2.m[1][3],
		m1.m[2][0] - m2.m[2][0],m1.m[2][1] - m2.m[2][1],m1.m[2][2] - m2.m[2][2],m1.m[2][3] - m2.m[2][3],
		m1.m[3][0] - m2.m[3][0],m1.m[3][1] - m2.m[3][1],m1.m[3][2] - m2.m[3][2],m1.m[3][3] - m2.m[3][3]
	};
}

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2)
{
	return {
		m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0] + m1.m[0][3] * m2.m[3][0],
		m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1] + m1.m[0][3] * m2.m[3][1],
		m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2] + m1.m[0][3] * m2.m[3][2],
		m1.m[0][0] * m2.m[0][3] + m1.m[0][1] * m2.m[1][3] + m1.m[0][2] * m2.m[2][3] + m1.m[0][3] * m2.m[3][3],
		m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0] + m1.m[1][3] * m2.m[3][0],
		m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] + m1.m[1][2] * m2.m[2][1] + m1.m[1][3] * m2.m[3][1],
		m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2] + m1.m[1][3] * m2.m[3][2],
		m1.m[1][0] * m2.m[0][3] + m1.m[1][1] * m2.m[1][3] + m1.m[1][2] * m2.m[2][3] + m1.m[1][3] * m2.m[3][3],
		m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0] + m1.m[2][3] * m2.m[3][0],
		m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] + m1.m[2][2] * m2.m[2][1] + m1.m[2][3] * m2.m[3][1],
		m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2] + m1.m[2][3] * m2.m[3][2],
		m1.m[2][0] * m2.m[0][3] + m1.m[2][1] * m2.m[1][3] + m1.m[2][2] * m2.m[2][3] + m1.m[2][3] * m2.m[3][3],
		m1.m[3][0] * m2.m[0][0] + m1.m[3][1] * m2.m[1][0] + m1.m[3][2] * m2.m[2][0] + m1.m[3][3] * m2.m[3][0],
		m1.m[3][0] * m2.m[0][1] + m1.m[3][1] * m2.m[1][1] + m1.m[3][2] * m2.m[2][1] + m1.m[3][3] * m2.m[3][1],
		m1.m[3][0] * m2.m[0][2] + m1.m[3][1] * m2.m[1][2] + m1.m[3][2] * m2.m[2][2] + m1.m[3][3] * m2.m[3][2],
		m1.m[3][0] * m2.m[0][3] + m1.m[3][1] * m2.m[1][3] + m1.m[3][2] * m2.m[2][3] + m1.m[3][3] * m2.m[3][3]
	};
}

Matrix4x4 Inverse(const Matrix4x4& m)
{
	float A
		= m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3]
		+ m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1]
		+ m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2]
		+ m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2]
		+ m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3]
		+ m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0]
		+ m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3]
		+ m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0]
		+ m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1]
		+ m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1]
		+ m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2]
		+ m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0]
		- m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2]
		- m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3]
		- m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1]
		- m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3]
		- m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0]
		- m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2]
		- m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1]
		- m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3]
		- m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0]
		- m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2]
		- m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0]
		- m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1];
	assert(A != 0);
	return {
		(1 / A) * (m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[1][2] * m.m[2][3] * m.m[3][1] + m.m[1][3] * m.m[2][1] * m.m[3][2] - m.m[1][1] * m.m[2][3] * m.m[3][2] - m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[1][3] * m.m[2][2] * m.m[3][1]),
		(1 / A) * (m.m[0][1] * m.m[2][3] * m.m[3][2] + m.m[0][2] * m.m[2][1] * m.m[3][3] + m.m[0][3] * m.m[2][2] * m.m[3][1] - m.m[0][1] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[2][3] * m.m[3][1] - m.m[0][3] * m.m[2][1] * m.m[3][2]),
		(1 / A) * (m.m[0][1] * m.m[1][2] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[3][1] + m.m[0][3] * m.m[1][1] * m.m[3][2] - m.m[0][1] * m.m[1][3] * m.m[3][2] - m.m[0][2] * m.m[1][1] * m.m[3][3] - m.m[0][3] * m.m[1][2] * m.m[3][1]),
		(1 / A) * (m.m[0][1] * m.m[1][3] * m.m[2][2] + m.m[0][2] * m.m[1][1] * m.m[2][3] + m.m[0][3] * m.m[1][2] * m.m[2][1] - m.m[0][1] * m.m[1][2] * m.m[2][3] - m.m[0][2] * m.m[1][3] * m.m[2][1] - m.m[0][3] * m.m[1][1] * m.m[2][2]),

		(1 / A) * (m.m[1][0] * m.m[2][3] * m.m[3][2] + m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[1][3] * m.m[2][2] * m.m[3][0] - m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[1][2] * m.m[2][3] * m.m[3][0] - m.m[1][3] * m.m[2][0] * m.m[3][2]),
		(1 / A) * (m.m[0][0] * m.m[2][2] * m.m[3][3] + m.m[0][2] * m.m[2][3] * m.m[3][0] + m.m[0][3] * m.m[2][0] * m.m[3][2] - m.m[0][0] * m.m[2][3] * m.m[3][2] - m.m[0][2] * m.m[2][0] * m.m[3][3] - m.m[0][3] * m.m[2][2] * m.m[3][0]),
		(1 / A) * (m.m[0][0] * m.m[1][3] * m.m[3][2] + m.m[0][2] * m.m[1][0] * m.m[3][3] + m.m[0][3] * m.m[1][2] * m.m[3][0] - m.m[0][0] * m.m[1][2] * m.m[3][3] - m.m[0][2] * m.m[1][3] * m.m[3][0] - m.m[0][3] * m.m[1][0] * m.m[3][2]),
		(1 / A) * (m.m[0][0] * m.m[1][2] * m.m[2][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] + m.m[0][3] * m.m[1][0] * m.m[2][2] - m.m[0][0] * m.m[1][3] * m.m[2][2] - m.m[0][2] * m.m[1][0] * m.m[2][3] - m.m[0][3] * m.m[1][2] * m.m[2][0]),

		(1 / A) * (m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[1][1] * m.m[2][3] * m.m[3][0] + m.m[1][3] * m.m[2][0] * m.m[3][1] - m.m[1][0] * m.m[2][3] * m.m[3][1] - m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[1][3] * m.m[2][1] * m.m[3][0]),
		(1 / A) * (m.m[0][0] * m.m[2][3] * m.m[3][1] + m.m[0][1] * m.m[2][0] * m.m[3][3] + m.m[0][3] * m.m[2][1] * m.m[3][0] - m.m[0][0] * m.m[2][1] * m.m[3][3] - m.m[0][1] * m.m[2][3] * m.m[3][0] - m.m[0][3] * m.m[2][0] * m.m[3][1]),
		(1 / A) * (m.m[0][0] * m.m[1][1] * m.m[3][3] + m.m[0][1] * m.m[1][3] * m.m[3][0] + m.m[0][3] * m.m[1][0] * m.m[3][1] - m.m[0][0] * m.m[1][3] * m.m[3][1] - m.m[0][1] * m.m[1][0] * m.m[3][3] - m.m[0][3] * m.m[1][1] * m.m[3][0]),
		(1 / A) * (m.m[0][0] * m.m[1][3] * m.m[2][1] + m.m[0][1] * m.m[1][0] * m.m[2][3] + m.m[0][3] * m.m[1][1] * m.m[2][0] - m.m[0][0] * m.m[1][1] * m.m[2][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] - m.m[0][3] * m.m[1][0] * m.m[2][1]),

		(1 / A) * (m.m[1][0] * m.m[2][2] * m.m[3][1] + m.m[1][1] * m.m[2][0] * m.m[3][2] + m.m[1][2] * m.m[2][1] * m.m[3][0] - m.m[1][0] * m.m[2][1] * m.m[3][2] - m.m[1][1] * m.m[2][2] * m.m[3][0] - m.m[1][2] * m.m[2][0] * m.m[3][1]),
		(1 / A) * (m.m[0][0] * m.m[2][1] * m.m[3][2] + m.m[0][1] * m.m[2][2] * m.m[3][0] + m.m[0][2] * m.m[2][0] * m.m[3][1] - m.m[0][0] * m.m[2][2] * m.m[3][1] - m.m[0][1] * m.m[2][0] * m.m[3][2] - m.m[0][2] * m.m[2][1] * m.m[3][0]),
		(1 / A) * (m.m[0][0] * m.m[1][2] * m.m[3][1] + m.m[0][1] * m.m[1][0] * m.m[3][2] + m.m[0][2] * m.m[1][1] * m.m[3][0] - m.m[0][0] * m.m[1][1] * m.m[3][2] - m.m[0][1] * m.m[1][2] * m.m[3][0] - m.m[0][2] * m.m[1][0] * m.m[3][1]),
		(1 / A) * (m.m[0][0] * m.m[1][1] * m.m[2][2] + m.m[0][1] * m.m[1][2] * m.m[2][0] + m.m[0][2] * m.m[1][0] * m.m[2][1] - m.m[0][0] * m.m[1][2] * m.m[2][1] - m.m[0][1] * m.m[1][0] * m.m[2][2] - m.m[0][2] * m.m[1][1] * m.m[2][0])
	};
}

Matrix4x4 Transpose(const Matrix4x4& m)
{
	return {
		m.m[0][0], m.m[1][0], m.m[2][0], m.m[3][0],
		m.m[0][1], m.m[1][1], m.m[2][1], m.m[3][1],
		m.m[0][2], m.m[1][2], m.m[2][2], m.m[3][2],
		m.m[0][3], m.m[1][3], m.m[2][3], m.m[3][3]
	};
}

Matrix4x4 MakeIdentity4x4()
{
	return {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
}

Matrix4x4 MakeTranslateMatrix(const Vector3& translate)
{
	return {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		translate.x, translate.y, translate.z, 1
	};
}

Matrix4x4 MakeScaleMatrix(const Vector3& scale)
{
	return {
		scale.x, 0, 0, 0,
		0, scale.y, 0, 0,
		0, 0, scale.z, 0,
		0, 0, 0, 1
	};
}

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix)
{
	Vector3 result;
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;
	return result;
}

Matrix4x4 MakeRotateXMatrix(float radian)
{
	return {
		1, 0, 0, 0,
		0, std::cos(radian), std::sin(radian), 0,
		0,-std::sin(radian), std::cos(radian), 0,
		0, 0, 0, 1
	};
}

Matrix4x4 MakeRotateYMatrix(float radian)
{
	return {
		std::cos(radian), 0, -std::sin(radian), 0,
		0, 1, 0, 0,
		std::sin(radian), 0, std::cos(radian), 0,
		0, 0, 0, 1 };
}

Matrix4x4 MakeRotateZMatrix(float radian)
{
	return {
		std::cos(radian), std::sin(radian), 0, 0,
		-std::sin(radian), std::cos(radian), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1,
	};
}

Matrix4x4 MakeRotateXYZMatrix(Vector3& radian)
{
	return { Multiply(Multiply(MakeRotateXMatrix(radian.x), MakeRotateYMatrix(radian.y)), MakeRotateZMatrix(radian.z)) };
}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate)
{
	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);
	Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);
	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
	Matrix4x4 rotateXYZMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));
	return { Multiply(scaleMatrix, Multiply(rotateXYZMatrix, translateMatrix)) };
}

Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip)
{
	return { 1 / aspectRatio * (1 / std::tan(fovY / 2)), 0, 0, 0, 0, (1 / std::tan(fovY / 2)), 0, 0, 0, 0, farClip / (farClip / nearClip), 1, 0, 0, -nearClip * farClip / (farClip - nearClip), 0 };
}

Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip)
{
	return { 2 / (right - left), 0, 0, 0, 0, 2 / (top - bottom), 0, 0, 0, 0, 1 / (farClip - nearClip), 0, (left + right) / (left - right), (top + bottom) / (bottom - top), nearClip / (nearClip - farClip), 1 };
}

Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth)
{
	return { width / 2, 0, 0, 0, 0, -height / 2, 0, 0, 0, 0, maxDepth - minDepth, 0, left + (width / 2), top + (height / 2), minDepth, 1 };
}

Vector3 Multiply(const Matrix4x4& mat, const Vector3& vec) {
	Vector3 result;
	result.x = mat.m[0][0] * vec.x + mat.m[0][1] * vec.y + mat.m[0][2] * vec.z + mat.m[0][3];
	result.y = mat.m[1][0] * vec.x + mat.m[1][1] * vec.y + mat.m[1][2] * vec.z + mat.m[1][3];
	result.z = mat.m[2][0] * vec.x + mat.m[2][1] * vec.y + mat.m[2][2] * vec.z + mat.m[2][3];
	return result;
}

Vector3 Cross(const Vector3& v1, const Vector3& v2)
{
	return { v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x };
}

Vector4 Multiply(const Matrix4x4& mat, const Vector4& vec) {
	Vector4 result;

	// 行列の各行とベクトルとの内積を計算
	result.x = mat.m[0][0] * vec.x + mat.m[0][1] * vec.y + mat.m[0][2] * vec.z + mat.m[0][3] * vec.w;
	result.y = mat.m[1][0] * vec.x + mat.m[1][1] * vec.y + mat.m[1][2] * vec.z + mat.m[1][3] * vec.w;
	result.z = mat.m[2][0] * vec.x + mat.m[2][1] * vec.y + mat.m[2][2] * vec.z + mat.m[2][3] * vec.w;
	result.w = mat.m[3][0] * vec.x + mat.m[3][1] * vec.y + mat.m[3][2] * vec.z + mat.m[3][3] * vec.w;

	return result;
}

void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	float pi = float(3.14); // Use standard M_PI constant for better precision
	const uint32_t kSubdivision = 10;
	const float kLatEvery = pi / kSubdivision;
	const float kLonEvery = (2 * pi) / kSubdivision;

	Vector3 a, b, c, d;

	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -pi / 2.0f + kLatEvery * latIndex;
		float nextLat = lat + kLatEvery;

		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = lonIndex * kLonEvery;
			float nextLon = lon + kLonEvery;

			// Calculate vertices of the quad on the sphere surface
			a = {
				sphere.center.x + sphere.radius * std::cos(lat) * std::cos(lon),
				sphere.center.y + sphere.radius * std::sin(lat),
				sphere.center.z + sphere.radius * std::cos(lat) * std::sin(lon)
			};

			b = {
				sphere.center.x + sphere.radius * std::cos(nextLat) * std::cos(lon),
				sphere.center.y + sphere.radius * std::sin(nextLat),
				sphere.center.z + sphere.radius * std::cos(nextLat) * std::sin(lon)
			};

			c = {
				sphere.center.x + sphere.radius * std::cos(lat) * std::cos(nextLon),
				sphere.center.y + sphere.radius * std::sin(lat),
				sphere.center.z + sphere.radius * std::cos(lat) * std::sin(nextLon)
			};

			d = {
				sphere.center.x + sphere.radius * std::cos(nextLat) * std::cos(nextLon),
				sphere.center.y + sphere.radius * std::sin(nextLat),
				sphere.center.z + sphere.radius * std::cos(nextLat) * std::sin(nextLon)
			};

			// Transform to screen space
			Vector3 screenA = Transform(Transform(a, viewProjectionMatrix), viewportMatrix);
			Vector3 screenB = Transform(Transform(b, viewProjectionMatrix), viewportMatrix);
			Vector3 screenC = Transform(Transform(c, viewProjectionMatrix), viewportMatrix);
			Vector3 screenD = Transform(Transform(d, viewProjectionMatrix), viewportMatrix);

			// Draw lines between the vertices to create the sphere wireframe
			Novice::DrawLine((int)screenA.x, (int)screenA.y, (int)screenB.x, (int)screenB.y, color);
			Novice::DrawLine((int)screenA.x, (int)screenA.y, (int)screenC.x, (int)screenC.y, color);
			Novice::DrawLine((int)screenB.x, (int)screenB.y, (int)screenD.x, (int)screenD.y, color);
			Novice::DrawLine((int)screenC.x, (int)screenC.y, (int)screenD.x, (int)screenD.y, color);
		}
	}
}

void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {
	const float kGridHalfWidth = 2.0f;
	const uint32_t kSubdivision = 10;
	const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision);

	Vector3 zLineStart;
	Vector3 zLineEnd;
	Vector3 xLineStart;
	Vector3 xLineEnd;

	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex) {

		zLineStart = Vector3(xIndex * kGridEvery - kGridHalfWidth, 0, 2);
		zLineEnd = Vector3(xIndex * kGridEvery - kGridHalfWidth, 0, -6);

		//スクリーン座標系まで変換をかける
		Matrix4x4 startWorldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, zLineStart);
		Matrix4x4 endWorldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, zLineEnd);

		Matrix4x4 startwvpMatrix = Multiply(startWorldMatrix, viewProjectionMatrix);
		Matrix4x4 endwvpMatrix = Multiply(startWorldMatrix, viewProjectionMatrix);

		Vector3 startLocal = Transform(zLineStart, startwvpMatrix);
		Vector3 endLocal = Transform(zLineEnd, endwvpMatrix);

		Vector3 startScreen = Transform(startLocal, viewportMatrix);
		Vector3 endScreen = Transform(endLocal, viewportMatrix);

		//変換した座標を使って表示、色は薄い灰色(0xAAAAAAFF)。原点は黒
		if (xIndex == kSubdivision / 2)
		{
			Novice::DrawLine((int)startScreen.x, (int)startScreen.y, (int)endScreen.x, (int)endScreen.y, BLACK);
		}
		else
		{
			Novice::DrawLine((int)startScreen.x, (int)startScreen.y, (int)endScreen.x, (int)endScreen.y, 0xAAAAAAFF);
		}

	}

	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex) {
		xLineStart = Vector3(2, 0, zIndex * kGridEvery - kGridHalfWidth);
		xLineEnd = Vector3(-2, 0, zIndex * kGridEvery - kGridHalfWidth);

		//スクリーン座標系まで変換をかける
		Matrix4x4 startWorldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, xLineStart);
		Matrix4x4 endWorldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, xLineEnd);

		Matrix4x4 startwvpMatrix = Multiply(startWorldMatrix, viewProjectionMatrix);
		Matrix4x4 endwvpMatrix = Multiply(endWorldMatrix, viewProjectionMatrix);

		Vector3 startLocal = Transform(xLineStart, startwvpMatrix);
		Vector3 endLocal = Transform(xLineEnd, endwvpMatrix);

		Vector3 startScreen = Transform(startLocal, viewportMatrix);
		Vector3 endScreen = Transform(endLocal, viewportMatrix);

		//変換した座標を使って表示、色は薄い灰色(0xAAAAAAFF)。原点は黒
		if (zIndex == kSubdivision / 2)
		{
			Novice::DrawLine((int)startScreen.x, (int)startScreen.y, (int)endScreen.x, (int)endScreen.y, BLACK);
		}
		else
		{
			Novice::DrawLine((int)startScreen.x, (int)startScreen.y, (int)endScreen.x, (int)endScreen.y, 0xAAAAAAFF);
		}
	}

}

Vector3 Project(const Vector3& v1, const Vector3& v2) {
	return Multiply(Dot(v1, Normalize(v2)), Normalize(v2));
}

Vector3 ClosestPoint(const Vector3& point, const Segment& segment) {
	// セグメントの方向ベクトルの長さの二乗
	float segLengthSquared = Length(segment.diff) * Length(segment.diff);

	// セグメントが実質的に点である場合
	if (segLengthSquared == 0.0) {
		return segment.origin;
	}

	// point から segment.origin へのベクトル
	Vector3 diffPointOrigin = point - segment.origin;

	// diffPointOrigin を segment.diff に射影したスカラー t を計算
	float t = Dot(diffPointOrigin, segment.diff) / segLengthSquared;

	// t を 0 から 1 の範囲にクランプ
	t = std::max(0.0f, std::min(1.0f, t));

	// 最近接点の計算
	Vector3 closestPoint = segment.origin + segment.diff * t;

	return closestPoint;
}

bool IsCollision(const Sphere& s1, const Sphere& s2) {
	if (Length(s1.center - s2.center) <= s1.radius + s2.radius) {
		return true;
	}
	else {
		return false;
	}
}

bool IsCollision(const Sphere& sphere, const Plane& plane)
{
	// 球体の中心から平面までの距離を計算
	float distanceFromCenterToPlane = Dot(plane.normal, sphere.center) - plane.distance;

	// この距離の絶対値が球体の半径よりも小さいかどうかを確認
	if (abs(distanceFromCenterToPlane) <= sphere.radius) {
		return true; // 当たっている
	}
	else {
		return false; // 当たっていない
	}
}


Vector3 Perpendicular(const Vector3& vector) {
	if (vector.x != 0.0f || vector.y != 0.0f) {
		return{ -vector.y, vector.x, 0.0f };
	}
	return{ 0.0f, -vector.z, vector.y };
}

void DrawPlane(const Plane& plane, const Matrix4x4& viewProjection, const Matrix4x4& viewport, uint32_t color) {
	Vector3 center = Multiply(plane.distance, plane.normal);	// 1
	Vector3 perpendiculars[4];
	perpendiculars[0] = Normalize(Perpendicular(plane.normal));		// 2
	perpendiculars[1] = { -perpendiculars[0].x, -perpendiculars[0].y, -perpendiculars[0].z };		// 3
	perpendiculars[2] = Cross(plane.normal, perpendiculars[0]);		// 4
	perpendiculars[3] = { -perpendiculars[2].x,  -perpendiculars[2].y, -perpendiculars[2].z };		// 5
	// 6
	Vector3 points[4];
	for (int32_t index = 0; index < 4; ++index) {
		Vector3 extend = Multiply(2.0f, perpendiculars[index]);
		Vector3 point = Add(center, extend);
		points[index] = Transform(Transform(point, viewProjection), viewport);
	}
	Novice::DrawLine(int(points[0].x), int(points[0].y), int(points[2].x), int(points[2].y), color);
	Novice::DrawLine(int(points[1].x), int(points[1].y), int(points[3].x), int(points[3].y), color);
	Novice::DrawLine(int(points[2].x), int(points[2].y), int(points[1].x), int(points[1].y), color);
	Novice::DrawLine(int(points[3].x), int(points[3].y), int(points[0].x), int(points[0].y), color);
}

void CameraMove(Vector3& cameraRotation, Vector3& cameraTranslation, Vector2Int& clickPosition, char* keys, char* preKeys) {
	// カーソルを動かすときの感度
	const float mouseSensitivity = 0.003f;
	// カメラの移動速度
	const float moveSpeed = 0.005f;

	// 各フラグ
	static bool isLeftClicked = false;
	static bool isWheelClicked = false;
	static bool isDebugCamera = false;

	// 回転を考慮する
	Matrix4x4 rotationMatrix = MakeRotateXYZMatrix(cameraRotation);
	Vector3 X = { 1.0f, 0.0f, 0.0f };
	Vector3 Y = { 0.0f, 1.0f, 0.0f };
	Vector3 Z = { 0.0f, 0.0f, -1.0f };

	Vector3 rotatedX = Transform(X, rotationMatrix);
	Vector3 rotatedY = Transform(Y, rotationMatrix);
	Vector3 rotatedZ = Transform(Z, rotationMatrix);

	if (keys[DIK_SPACE] && preKeys[DIK_SPACE] == 0) {
		isDebugCamera = !isDebugCamera;
	}

	if (isDebugCamera) {

		/// ========カメラ操作========
		// カメラの回転を更新する
		if (Novice::IsPressMouse(0) == 1) {
			if (!isLeftClicked) {
				// マウスがクリックされたときに現在のマウス位置を保存する
				Novice::GetMousePosition(&clickPosition.x, &clickPosition.y);
				isLeftClicked = true;
			}
			else {
				// マウスがクリックされている間はカメラの回転を更新する
				Vector2Int currentMousePos;
				Novice::GetMousePosition(&currentMousePos.x, &currentMousePos.y);

				float deltaX = static_cast<float>(currentMousePos.x - clickPosition.x);
				float deltaY = static_cast<float>(currentMousePos.y - clickPosition.y);

				cameraRotation.x += deltaY * mouseSensitivity;
				cameraRotation.y += deltaX * mouseSensitivity;

				// 現在のマウス位置を保存する
				clickPosition = currentMousePos;
			}
		}
		else {
			// マウスがクリックされていない場合はフラグをリセットする
			isLeftClicked = false;
		}

		// カメラの位置を更新する
		if (Novice::IsPressMouse(2) == 1) {
			if (!isWheelClicked) {
				// マウスがクリックされたときに現在のマウス位置を保存する
				Novice::GetMousePosition(&clickPosition.x, &clickPosition.y);
				isWheelClicked = true;
			}
			else {
				// マウスがクリックされている間はカメラの位置を更新する
				Vector2Int currentMousePos;
				Novice::GetMousePosition(&currentMousePos.x, &currentMousePos.y);

				float deltaX = static_cast<float>(currentMousePos.x - clickPosition.x);
				float deltaY = static_cast<float>(currentMousePos.y - clickPosition.y);

				cameraTranslation = Subtract(cameraTranslation, rotatedX * deltaX * mouseSensitivity);
				cameraTranslation += rotatedY * deltaY * mouseSensitivity;

				// 現在のマウス位置を保存する
				clickPosition = currentMousePos;
			}
		}
		else {
			// マウスがクリックされていない場合はフラグをリセットする
			isWheelClicked = false;
		}

		// マウスホイールの移動量を取得する
		int wheelDelta = -Novice::GetWheel();

		// マウスホイールの移動量に応じてカメラの移動を更新する
		cameraTranslation += rotatedZ * float(wheelDelta) * moveSpeed;
		/// =====================
		
	}
	//ImGui::Begin("DebugCamera");
	//ImGui::Text("flag %d", isDebugCamera);
	//ImGui::End();
}

bool IsCollision(const Segment& segment, const Plane& plane)
{
	float dot = Dot(plane.normal, segment.diff);
	// 平面と線分が平行の場合、交差しない
	if (dot == 0.0f) {
		return false;
	}

	// tの値を計算
	float t = (plane.distance - Dot(segment.origin, plane.normal)) / dot;

	// tが0から1の間にある場合、線分は平面と交差する
	if (t >= 0.0f && t <= 1.0f) {
		return true;
	}
	else {
		return false;
	}
}


void DrawLine(const Segment& segment, const Matrix4x4& viewProjection, const Matrix4x4& viewport, uint32_t color) {
	// 始点と終点を変換
	Vector3 transformedStart = Transform(Transform(segment.origin, viewProjection), viewport);
	Vector3 transformedEnd = Transform(Transform(segment.diff, viewProjection), viewport);

	// スクリーン座標に変換
	int x1 = static_cast<int>(transformedStart.x);
	int y1 = static_cast<int>(transformedStart.y);
	int x2 = static_cast<int>(transformedEnd.x);
	int y2 = static_cast<int>(transformedEnd.y);

	// 線を描画
	Novice::DrawLine(x1, y1, x2, y2, color);
}

bool IsCollision(const Triangle& triangle, const Segment& segment)
{
	// 三角形の頂点から法線ベクトルを計算
	Vector3 edge1 = triangle.vertices[1] - triangle.vertices[0];
	Vector3 edge2 = triangle.vertices[2] - triangle.vertices[0];
	Vector3 normal = Cross(edge1, edge2);
	normal = Normalize(normal);

	// 平面の方程式を定義
	Plane plane{ normal, Dot(normal, triangle.vertices[0]) };

	// 線分と平面の交差判定
	float dot = Dot(plane.normal, segment.diff);

	// 平面と線分が平行な場合、交差しない
	if (dot == 0.0f) {
		return false;
	}

	// 線分の始点から終点へのベクトルの割合 t を計算
	float t = (plane.distance - Dot(segment.origin, plane.normal)) / dot;

	// tが0から1の間にある場合、交点が線分上にある
	if (t >= 0.0f && t <= 1.0f) {
		// 交点の座標を計算
		Vector3 intersection = segment.origin + segment.diff * t;

		// 交点が三角形内にあるかを判定
		for (int i = 0; i < 3; ++i) {
			Vector3 edge = triangle.vertices[(i + 1) % 3] - triangle.vertices[i];
			Vector3 vp = intersection - triangle.vertices[i];
			Vector3 crossProduct = Cross(edge, vp);
			if (Dot(normal, crossProduct) < 0) {
				return false;
			}
		}
		return true;
	}
	return false;
}


void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	Vector3 transformedVertices[3];
	for (int i = 0; i < 3; ++i) {
		transformedVertices[i] = Transform(Transform(triangle.vertices[i], viewProjectionMatrix), viewportMatrix);
	}

	for (int i = 0; i < 3; ++i) {
		int x1 = static_cast<int>(transformedVertices[i].x);
		int y1 = static_cast<int>(transformedVertices[i].y);
		int x2 = static_cast<int>(transformedVertices[(i + 1) % 3].x);
		int y2 = static_cast<int>(transformedVertices[(i + 1) % 3].y);

		Novice::DrawLine(x1, y1, x2, y2, color);
	}
}

bool IsCollision(const AABB& a, const AABB& b)
{
	if ((a.min.x <= b.max.x && a.max.x >= b.min.x) &&
		(a.min.y <= b.max.y && a.max.y >= b.min.y) &&
		(a.min.z <= b.max.z && a.max.z >= b.min.z)) {
		return true;
	}
	else {
		return false;
	}
}

// AABBを描画する関数
void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	// AABBの8つの頂点を計算
	Vector3 vertices[8] = {
		{aabb.min.x, aabb.min.y, aabb.min.z},
		{aabb.max.x, aabb.min.y, aabb.min.z},
		{aabb.min.x, aabb.max.y, aabb.min.z},
		{aabb.max.x, aabb.max.y, aabb.min.z},
		{aabb.min.x, aabb.min.y, aabb.max.z},
		{aabb.max.x, aabb.min.y, aabb.max.z},
		{aabb.min.x, aabb.max.y, aabb.max.z},
		{aabb.max.x, aabb.max.y, aabb.max.z}
	};

	// 各頂点を変換
	Vector3 transformedVertices[8];
	for (int i = 0; i < 8; ++i) {
		transformedVertices[i] = Transform(Transform(vertices[i], viewProjectionMatrix), viewportMatrix);
	}

	// AABBのエッジを描画
	const int edges[12][2] = {
		{0, 1}, {1, 3}, {3, 2}, {2, 0}, // 前面
		{4, 5}, {5, 7}, {7, 6}, {6, 4}, // 背面
		{0, 4}, {1, 5}, {2, 6}, {3, 7}  // 前面と背面を結ぶエッジ
	};

	for (int i = 0; i < 12; ++i) {
		int x1 = static_cast<int>(transformedVertices[edges[i][0]].x);
		int y1 = static_cast<int>(transformedVertices[edges[i][0]].y);
		int x2 = static_cast<int>(transformedVertices[edges[i][1]].x);
		int y2 = static_cast<int>(transformedVertices[edges[i][1]].y);

		Novice::DrawLine(x1, y1, x2, y2, color);
	}
}

// 球とAABBの当たり判定を行う関数
bool IsCollision(const AABB& aabb, const Sphere& sphere) {
	// 最近接点を計算
	float closestX = std::max(aabb.min.x, std::min(sphere.center.x, aabb.max.x));
	float closestY = std::max(aabb.min.y, std::min(sphere.center.y, aabb.max.y));
	float closestZ = std::max(aabb.min.z, std::min(sphere.center.z, aabb.max.z));

	// 最近接点と球の中心との距離を計算
	float distanceX = sphere.center.x - closestX;
	float distanceY = sphere.center.y - closestY;
	float distanceZ = sphere.center.z - closestZ;

	// 距離の二乗を計算
	float distanceSquared = (distanceX * distanceX) + (distanceY * distanceY) + (distanceZ * distanceZ);

	// 距離の二乗が半径の二乗以下であれば衝突している
	return distanceSquared < (sphere.radius * sphere.radius);
}
