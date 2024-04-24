#pragma once
#include "Novice.h"
#include <math.h>
#include <cassert>
#include <cmath>

static const int kColumnWidth = 60;
const int kRowHeight = 22;

struct Vector2 {
	float x;
	float y;
};

struct Vector3 {
	float x;
	float y;
	float z;
};

struct Matrix2x2 {
	float m[2][2];
};

struct Matrix3x3 {
	float m[3][3];
};

struct Matrix4x4 {
	float m[4][4];
};

/// <summary>
/// 3次元ベクトルを表示するための関数
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="vector"></param>
/// <param name="label"></param>
void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label);

/// <summary>
/// 3次元ベクトル同士の足し算をする関数
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
Vector3 Add(const Vector3& v1, const Vector3& v2);

/// <summary>
/// 3次元ベクトル同士の引き算をする関数
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
Vector3 Subtract(const Vector3& v1, const Vector3& v2);

/// <summary>
/// 3次元ベクトルをスカラー倍する関数
/// </summary>
/// <param name="scalar"></param>
/// <param name="v"></param>
/// <returns></returns>
Vector3 Multiply(float scalar, const Vector3& v);

/// <summary>
/// 3次元ベクトルの内積を求める関数
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
float Dot(const Vector3& v1, const Vector3& v2);

/// <summary>
/// 3次元ベクトルの大きさを求める関数
/// </summary>
/// <param name="v"></param>
/// <returns></returns>
float Length(const Vector3& v);

/// <summary>
/// 3次元ベクトルの正規化をする関数
/// </summary>
/// <param name="v"></param>
/// <returns></returns>
Vector3 Normalize(const Vector3& v);

void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label);

Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);

Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

Matrix4x4 Inverse(const Matrix4x4& m);

Matrix4x4 Transpose(const Matrix4x4& m);

Matrix4x4 MakeIdentity4x4();

Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

Matrix4x4 MakeScaleMatrix(const Vector3& scale);

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);