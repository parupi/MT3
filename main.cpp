#include "function.h"

const char kWindowTitle[] = "LE2B_08_カワグチ_ハルキ";

Vector3 cameraTranslate{ 0.0f, 1.9f, -6.49f };
Vector3 cameraRotate{ 0.26f, 0.0f, 0.0f };
Vector3 cameraPosition{ 0.0f, 1.0f, -5.0f };
Vector2Int clickPosition;
bool isLeftClicked = false;
bool isWheelClicked = false;
float mouseSensitivity = 0.001f;
float moveSpeed = 0.001f;
void UpdateCameraRotation();
void UpdateCameraTranslation();
void UpdateCameraZoom();
void UpdateCamera();

bool IsCollision(const Sphere& s1, const Sphere& s2);

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	Sphere sphere[2]{};
	sphere[0] = { {0.0f, 0.0f, 0.0f} , 1.0f };
	sphere[1] = { {1.0f, 0.0f, 1.0f} , 1.0f };
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

		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, cameraRotate, cameraPosition + cameraTranslate);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

		UpdateCamera();

		isHit = IsCollision(sphere[0], sphere[1]) == true;

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
		ImGui::DragFloat3("Sphere[0].Center", &sphere[0].center.x, 0.01f);
		ImGui::DragFloat3("Sphere[0].Radius", &sphere[0].radius, 0.01f);
		ImGui::DragFloat3("Sphere[1].Center", &sphere[1].center.x, 0.01f);
		ImGui::DragFloat3("Sphere[1].Radius", &sphere[1].radius, 0.01f);
		ImGui::End();

		DrawGrid(viewProjectionMatrix, viewportMatrix);

		DrawSphere(sphere[0], viewProjectionMatrix, viewportMatrix, color);
		DrawSphere(sphere[1], viewProjectionMatrix, viewportMatrix, 0xFFFFFFFF);

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

void UpdateCameraRotation() {
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

			cameraRotate.x += deltaY * mouseSensitivity;
			cameraRotate.y += deltaX * mouseSensitivity;

			// 現在のマウス位置を保存する
			clickPosition = currentMousePos;
		}
	}
	else {
		// マウスがクリックされていない場合はフラグをリセットする
		isLeftClicked = false;
	}
}

void UpdateCameraTranslation() {
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

			cameraTranslate.x -= deltaX * mouseSensitivity;
			cameraTranslate.y += deltaY * mouseSensitivity;

			// 現在のマウス位置を保存する
			clickPosition = currentMousePos;
		}
	}
	else {
		// マウスがクリックされていない場合はフラグをリセットする
		isWheelClicked = false;
	}
}

void UpdateCameraZoom() {
	int wheelDelta = Novice::GetWheel();
	cameraTranslate.z += wheelDelta * moveSpeed;
}

void UpdateCamera() {
	UpdateCameraRotation();
	UpdateCameraTranslation();
	UpdateCameraZoom();
}
