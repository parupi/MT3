#include "Matrix4x4.h"
#include <array>
#include <cassert>
#include <cmath>

Matrix4x4 Matrix4x4::operator+(const Matrix4x4& mat) const {
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = m[i][j] + mat.m[i][j];
		}
	}
	return result;
}

Matrix4x4 Matrix4x4::operator-(const Matrix4x4& mat) const {
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = m[i][j] - mat.m[i][j];
		}
	}
	return result;
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& mat) const {
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = 0;
			for (int k = 0; k < 4; ++k) {
				result.m[i][j] += m[i][k] * mat.m[k][j];
			}
		}
	}
	return result;
}

Matrix4x4 Matrix4x4::operator*(const float& scalar) const {
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = m[i][j] * scalar;
		}
	}
	return result;
}

Matrix4x4 Matrix4x4::operator/(const float& scalar) const {
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = m[i][j] / scalar;
		}
	}
	return result;
}

Matrix4x4& Matrix4x4::operator+=(const Matrix4x4& mat) {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			m[i][j] += mat.m[i][j];
		}
	}
	return *this;
}

Matrix4x4& Matrix4x4::operator-=(const Matrix4x4& mat) {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			m[i][j] -= mat.m[i][j];
		}
	}
	return *this;
}

Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& mat) {
	Matrix4x4 result = (*this) * mat;
	*this = result;
	return *this;
}

Matrix4x4& Matrix4x4::operator/=(const float& scalar) {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			m[i][j] /= scalar;
		}
	}
	return *this;
}

//// Vector3との乗算を行うメソッドの実装
//Vector3 Matrix4x4::operator*(const Vector3& vec) const {
//	float resultX = m[0][0] * vec.x + m[0][1] * vec.y + m[0][2] * vec.z + m[0][3];
//	float resultY = m[1][0] * vec.x + m[1][1] * vec.y + m[1][2] * vec.z + m[1][3];
//	float resultZ = m[2][0] * vec.x + m[2][1] * vec.y + m[2][2] * vec.z + m[2][3];
//	return Vector3(resultX, resultY, resultZ);
//}

Matrix4x4 Matrix4x4::MakeIdentity4x4()
{
	return {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
}
//
//Matrix4x4 Matrix4x4::Multiply(const Matrix4x4& m1, const Matrix4x4& m2)
//{
//	Matrix4x4 result;
//	for (int i = 0; i < 4; ++i) {
//		for (int j = 0; j < 4; ++j) {
//			result.m[i][j] = 0;
//			for (int k = 0; k < 4; ++k) {
//				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
//			}
//		}
//	}
//	return result;
//}

//Matrix4x4 Matrix4x4::MakeTranslateMatrix(const Vector3& translate)
//{
//	return {
//		1, 0, 0, 0,
//		0, 1, 0, 0,
//		0, 0, 1, 0,
//		translate.x, translate.y, translate.z, 1
//	};
//}
//
//Matrix4x4 Matrix4x4::MakeScaleMatrix(const Vector3& scale)
//{
//	return {
//		scale.x, 0, 0, 0,
//		0, scale.y, 0, 0,
//		0, 0, scale.z, 0,
//		0, 0, 0, 1
//	};
//}

Matrix4x4 Matrix4x4::MakeRotateXMatrix(float radian)
{
	return {
		1, 0, 0, 0,
		0, std::cos(radian), std::sin(radian), 0,
		0,-std::sin(radian), std::cos(radian), 0,
		0, 0, 0, 1
	};
}

Matrix4x4 Matrix4x4::MakeRotateYMatrix(float radian)
{
	return {
		std::cos(radian), 0, -std::sin(radian), 0,
		0, 1, 0, 0,
		std::sin(radian), 0, std::cos(radian), 0,
		0, 0, 0, 1 };
}

Matrix4x4 Matrix4x4::MakeRotateZMatrix(float radian)
{
	return {
		std::cos(radian), std::sin(radian), 0, 0,
		-std::sin(radian), std::cos(radian), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1,
	};
}

//Matrix4x4 Matrix4x4::MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate)
//{
//	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);
//	Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);
//	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
//	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
//	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
//	Matrix4x4 rotateXYZMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));
//	return { Multiply(scaleMatrix, Multiply(rotateXYZMatrix, translateMatrix)) };
//}

Matrix4x4 Matrix4x4::Inverse(const Matrix4x4& m)
{
	float det = Determinant(m);
	if (det == 0) {
		throw std::runtime_error("Matrix is not invertible");
	}

	Matrix4x4 adj = Adjugate(m);
	Matrix4x4 inv;

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			inv.m[i][j] = adj.m[i][j] / det;
		}
	}

	return inv;
}

//Matrix4x4 Matrix4x4::ConversScale(const Vector3& scale)
//{
//	Matrix4x4 mat = MakeIdentity4x4();
//	mat.m[0][0] = scale.x;
//	mat.m[1][1] = scale.y;
//	mat.m[2][2] = scale.z;
//	return mat;
//}

//Matrix4x4 Matrix4x4::ConversTranslate(const Vector3& translate)
//{
//	Matrix4x4 mat = MakeIdentity4x4();
//	mat.m[0][3] = translate.x;
//	mat.m[1][3] = translate.y;
//	mat.m[2][3] = translate.z;
//	return mat;
//}

float Matrix4x4::Determinant(const Matrix4x4& m)
{
	float det =
		m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0] - m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0] -
		m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0] + m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0] +
		m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0] - m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0] -
		m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1] + m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1] +
		m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1] - m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1] -
		m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1] + m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1] +
		m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2] - m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2] -
		m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2] + m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2] +
		m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2] - m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2] -
		m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3] + m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3] +
		m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3] - m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3] -
		m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3] + m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3];
	return det;
}

Matrix4x4 Matrix4x4::Adjugate(const Matrix4x4& m)
{
	Matrix4x4 adj;

	adj.m[0][0] = m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[1][2] * m.m[2][3] * m.m[3][1] + m.m[1][3] * m.m[2][1] * m.m[3][2] - m.m[1][1] * m.m[2][3] * m.m[3][2] - m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[1][3] * m.m[2][2] * m.m[3][1];
	adj.m[0][1] = m.m[0][1] * m.m[2][3] * m.m[3][2] + m.m[0][2] * m.m[2][1] * m.m[3][3] + m.m[0][3] * m.m[2][2] * m.m[3][1] - m.m[0][1] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[2][3] * m.m[3][1] - m.m[0][3] * m.m[2][1] * m.m[3][2];
	adj.m[0][2] = m.m[0][1] * m.m[1][2] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[3][1] + m.m[0][3] * m.m[1][1] * m.m[3][2] - m.m[0][1] * m.m[1][3] * m.m[3][2] - m.m[0][2] * m.m[1][1] * m.m[3][3] - m.m[0][3] * m.m[1][2] * m.m[3][1];
	adj.m[0][3] = m.m[0][1] * m.m[1][3] * m.m[2][2] + m.m[0][2] * m.m[1][1] * m.m[2][3] + m.m[0][3] * m.m[1][2] * m.m[2][1] - m.m[0][1] * m.m[1][2] * m.m[2][3] - m.m[0][2] * m.m[1][3] * m.m[2][1] - m.m[0][3] * m.m[1][1] * m.m[2][2];

	adj.m[1][0] = m.m[1][0] * m.m[2][3] * m.m[3][2] + m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[1][3] * m.m[2][2] * m.m[3][0] - m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[1][2] * m.m[2][3] * m.m[3][0] - m.m[1][3] * m.m[2][0] * m.m[3][2];
	adj.m[1][1] = m.m[0][0] * m.m[2][2] * m.m[3][3] + m.m[0][2] * m.m[2][3] * m.m[3][0] + m.m[0][3] * m.m[2][0] * m.m[3][2] - m.m[0][0] * m.m[2][3] * m.m[3][2] - m.m[0][2] * m.m[2][0] * m.m[3][3] - m.m[0][3] * m.m[2][2] * m.m[3][0];
	adj.m[1][2] = m.m[0][0] * m.m[1][3] * m.m[3][2] + m.m[0][2] * m.m[1][0] * m.m[3][3] + m.m[0][3] * m.m[1][2] * m.m[3][0] - m.m[0][0] * m.m[1][2] * m.m[3][3] - m.m[0][2] * m.m[1][3] * m.m[3][0] - m.m[0][3] * m.m[1][0] * m.m[3][2];
	adj.m[1][3] = m.m[0][0] * m.m[1][2] * m.m[2][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] + m.m[0][3] * m.m[1][0] * m.m[2][2] - m.m[0][0] * m.m[1][3] * m.m[2][2] - m.m[0][2] * m.m[1][0] * m.m[2][3] - m.m[0][3] * m.m[1][2] * m.m[2][0];

	adj.m[2][0] = m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[1][1] * m.m[2][3] * m.m[3][0] + m.m[1][3] * m.m[2][0] * m.m[3][1] - m.m[1][0] * m.m[2][3] * m.m[3][1] - m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[1][3] * m.m[2][1] * m.m[3][0];
	adj.m[2][1] = m.m[0][0] * m.m[2][3] * m.m[3][1] + m.m[0][1] * m.m[2][0] * m.m[3][3] + m.m[0][3] * m.m[2][1] * m.m[3][0] - m.m[0][0] * m.m[2][1] * m.m[3][3] - m.m[0][1] * m.m[2][3] * m.m[3][0] - m.m[0][3] * m.m[2][0] * m.m[3][1];
	adj.m[2][2] = m.m[0][0] * m.m[1][1] * m.m[3][3] + m.m[0][1] * m.m[1][3] * m.m[3][0] + m.m[0][3] * m.m[1][0] * m.m[3][1] - m.m[0][0] * m.m[1][3] * m.m[3][1] - m.m[0][1] * m.m[1][0] * m.m[3][3] - m.m[0][3] * m.m[1][1] * m.m[3][0];
	adj.m[2][3] = m.m[0][0] * m.m[1][3] * m.m[2][1] + m.m[0][1] * m.m[1][0] * m.m[2][3] + m.m[0][3] * m.m[1][1] * m.m[2][0] - m.m[0][0] * m.m[1][1] * m.m[2][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] - m.m[0][3] * m.m[1][0] * m.m[2][1];

	adj.m[3][0] = m.m[1][0] * m.m[2][2] * m.m[3][1] + m.m[1][1] * m.m[2][0] * m.m[3][2] + m.m[1][2] * m.m[2][1] * m.m[3][0] - m.m[1][0] * m.m[2][1] * m.m[3][2] - m.m[1][1] * m.m[2][2] * m.m[3][0] - m.m[1][2] * m.m[2][0] * m.m[3][1];
	adj.m[3][1] = m.m[0][0] * m.m[2][1] * m.m[3][2] + m.m[0][1] * m.m[2][2] * m.m[3][0] + m.m[0][2] * m.m[2][0] * m.m[3][1] - m.m[0][0] * m.m[2][2] * m.m[3][1] - m.m[0][1] * m.m[2][0] * m.m[3][2] - m.m[0][2] * m.m[2][1] * m.m[3][0];
	adj.m[3][2] = m.m[0][0] * m.m[1][2] * m.m[3][1] + m.m[0][1] * m.m[1][0] * m.m[3][2] + m.m[0][2] * m.m[1][1] * m.m[3][0] - m.m[0][0] * m.m[1][1] * m.m[3][2] - m.m[0][1] * m.m[1][2] * m.m[3][0] - m.m[0][2] * m.m[1][0] * m.m[3][1];
	adj.m[3][3] = m.m[0][0] * m.m[1][1] * m.m[2][2] + m.m[0][1] * m.m[1][2] * m.m[2][0] + m.m[0][2] * m.m[1][0] * m.m[2][1] - m.m[0][0] * m.m[1][2] * m.m[2][1] - m.m[0][1] * m.m[1][0] * m.m[2][2] - m.m[0][2] * m.m[1][1] * m.m[2][0];

	return adj;
}
