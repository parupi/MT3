#include "function.h"

const char kWindowTitle[] = "LE2B_08_カワグチ_ハルキ";

struct Plane {
	Vector3 normal;		//!< 法線
	float distance;		//!< 距離
};

Vector3 cameraTranslate{ 0.0f, 1.9f, -6.49f };
Vector3 cameraRotate{ 0.26f, 0.0f, 0.0f };
Vector3 cameraPosition{ 0.0f, 1.0f, -5.0f };
Vector2Int clickPos{};

bool IsCollision(const Sphere& sphere, const Plane& plane);
Vector3 Perpendicular(const Vector3& vector);
void DrawPlane(const Plane& plane, const Matrix4x4& viewProjection, const Matrix4x4& viewport,  uint32_t color);
void CameraMove(Vector3& cameraRotation, Vector3& cameraTranslation, Vector2Int& clickPosition, char* keys, char* preKeys);
Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, cameraRotate, cameraPosition + cameraTranslate);
Matrix4x4 viewMatrix = Inverse(cameraMatrix);
Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);
Matrix4x4 viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);

bool IsCollision(const Sphere& s1, const Sphere& s2);

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	Sphere sphere;
	Plane plane{ {0.0f,1.0f,0.0f}, 1.0f };
	sphere = { {0.0f, 0.0f, 0.0f} , 1.0f };
	uint32_t color = 0xFFFFFFFF;
	bool isHit = false;

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		CameraMove(cameraRotate, cameraPosition, clickPos, keys, preKeys);

		isHit = IsCollision(sphere, plane);

		if (isHit) {
			color = 0xFF0000FF;
		}
		else {
			color = 0xFFFFFFFF;
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		ImGui::Begin("Window");
		ImGui::DragFloat3("Sphere.Center", &sphere.center.x, 0.01f);
		ImGui::DragFloat("Sphere.Radius", &sphere.radius, 0.01f);
		ImGui::DragFloat3("Plane.Normal", &plane.normal.x, 0.01f);
		plane.normal = Normalize(plane.normal);
		ImGui::DragFloat("Plane.Distance", &plane.distance, 0.01f);
		ImGui::End();

		DrawGrid(viewProjectionMatrix, viewportMatrix);

		DrawSphere(sphere, viewProjectionMatrix, viewportMatrix, color);
		DrawPlane(plane, viewProjectionMatrix, viewportMatrix, 0xFFFFFFFF);

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
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

void DrawPlane(const Plane& plane, const Matrix4x4& viewProjection, const Matrix4x4& viewport,  uint32_t color) {
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
}
