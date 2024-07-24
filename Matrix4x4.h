#pragma once
//#include "Vector3.h"
#include <initializer_list>
#include <stdexcept>

class Matrix4x4 {
public:
	float m[4][4];

	// コンストラクタ
	Matrix4x4() {
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				m[i][j] = 0.0f;
			}
		}
	}

	// Initializer listを受け取るコンストラクタ
	Matrix4x4(std::initializer_list<float> list) {
		if (list.size() != 16) {
			throw std::invalid_argument("Initializer list must contain exactly 16 elements.");
		}

		auto it = list.begin();
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				m[i][j] = *it++;
			}
		}
	}

	// 演算子オーバーロード
	Matrix4x4 operator+(const Matrix4x4& mat) const;
	Matrix4x4 operator-(const Matrix4x4& mat) const;
	Matrix4x4 operator*(const Matrix4x4& mat) const;
	Matrix4x4 operator*(const float& scalar) const;
	Matrix4x4 operator/(const float& scalar) const;

	Matrix4x4& operator+=(const Matrix4x4& mat);
	Matrix4x4& operator-=(const Matrix4x4& mat);
	Matrix4x4& operator*=(const Matrix4x4& mat);
	Matrix4x4& operator/=(const float& scalar);

	// Vector3との乗算
	//Vector3 operator*(const Vector3& vec) const;

	// 単位行列を作る関数
	static Matrix4x4 MakeIdentity4x4();
	// Matrix4x4同士の掛け算をする関数
	//static Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

	//static Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

	//static Matrix4x4 MakeScaleMatrix(const Vector3& scale);

	static Matrix4x4 MakeRotateXMatrix(float radian);

	static Matrix4x4 MakeRotateYMatrix(float radian);

	static Matrix4x4 MakeRotateZMatrix(float radian);

	//static Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

	static Matrix4x4 Inverse(const Matrix4x4& m);

	//static Matrix4x4 ConversScale(const Vector3& v);

	//static Matrix4x4 ConversTranslate(const Vector3& v);

private:
	static float Determinant(const Matrix4x4& m);
	static Matrix4x4 Adjugate(const Matrix4x4& m);
};