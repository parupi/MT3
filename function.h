#pragma once
#include "Novice.h"
#include <math.h>
#include <cassert>
#include <cmath>
#include <imgui.h>
#include "assert.h"
#include "Vector3.h"
#include <iostream>
#include <algorithm>
#define _USE_MATH_DEFINES

static const int kColumnWidth = 60;
const int kRowHeight = 22;

const int kWindowWidth = 1280;
const int kWindowHeight = 720;

struct Vector2 {
	float x;
	float y;
};

struct Vector2Int {
	int x;
	int y;
};

struct Vector4 {
	float w;
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

struct Sphere {
	Vector3 center;//中心
	float radius;//半径
};

struct Line {
	Vector3 origin;		//!< 始点
	Vector3 diff;		//!< 終点への差分ベクトル
};

struct Ray {
	Vector3 origin;		//!< 始点
	Vector3 diff;		//!< 終点への差分ベクトル
};

struct Segment {
	Vector3 origin;		//!< 始点
	Vector3 diff;		//!< 終点への差分ベクトル
};

struct Plane {
	Vector3 normal;		//!< 法線
	float distance;		//!< 距離
};

struct Triangle {
	Vector3 vertices[3];
};


struct AABB {
	Vector3 min;    //!< 最小点
	Vector3 max;    //!< 最大点
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

float MagnitudeSquared(const Vector3& v);

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

Matrix4x4 MakeRotateXMatrix(float radian);

Matrix4x4 MakeRotateYMatrix(float radian);

Matrix4x4 MakeRotateZMatrix(float radian);

Matrix4x4 MakeRotateXYZMatrix(Vector3& radian);

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

// 1. 透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

// 2. 正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);

// 3. ビューポート変換行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

Vector3 Multiply(const Matrix4x4& mat, const Vector3& vec);

// クロス積
Vector3 Cross(const Vector3& v1, const Vector3& v2);

Vector4 Multiply(const Matrix4x4& mat, const Vector4& vec);

//グリッドと球の描画
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);
void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);

Vector3 Project(const Vector3& v1, const Vector3& v2);

Vector3 ClosestPoint(const Vector3& point, const Segment& segment);

bool IsCollision(const Sphere& s1, const Sphere& s2);

bool IsCollision(const Sphere& sphere, const Plane& plane);

Vector3 Perpendicular(const Vector3& vector);

void DrawPlane(const Plane& plane, const Matrix4x4& viewProjection, const Matrix4x4& viewport, uint32_t color);
// デバッグカメラの関数化
void CameraMove(Vector3& cameraRotation, Vector3& cameraTranslation, Vector2Int& clickPosition, char* keys, char* preKeys);

bool IsCollision(const Segment& segment, const Plane& plane);
void DrawLine(const Segment& segment, const Matrix4x4& viewProjection, const Matrix4x4& viewport, uint32_t color);

bool IsCollision(const Triangle& triangle, const Segment& segment);
void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

bool IsCollision(const AABB& aabb1, const AABB& aabb2);
void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);
