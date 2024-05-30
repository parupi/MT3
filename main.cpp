#include "function.h"

const char kWindowTitle[] = "LE2B_08_カワグチ_ハルキ";

bool IsCollision(const Segment& segment, const Plane& plane);
void DrawLine(const Segment& segment, const Matrix4x4& viewProjection, const Matrix4x4& viewport, uint32_t color);

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Vector3 cameraTranslate{ 0.0f, 1.9f, -6.49f };
	Vector3 cameraRotate{ 0.26f, 0.0f, 0.0f };
	Vector3 cameraPosition{ 0.0f, 1.0f, -5.0f };
	Vector2Int clickPos{};

	Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, cameraRotate, cameraPosition + cameraTranslate);
	Matrix4x4 viewMatrix = Inverse(cameraMatrix);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
	Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);
	Matrix4x4 viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);

	Plane plane{ {0.0f,1.0f,0.0f}, 1.0f };
	Segment line{ {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f} };
	uint32_t color = 0xFFFFFFFF;
	bool isHit = false;

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

		CameraMove(cameraRotate, cameraPosition, clickPos/*, keys, preKeys*/);

		isHit = IsCollision(line, plane);

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
		ImGui::DragFloat3("Plane.Normal", &plane.normal.x, 0.01f);
		plane.normal = Normalize(plane.normal);
		ImGui::DragFloat("Plane.Distance", &plane.distance, 0.01f);
		ImGui::DragFloat3("Segment.Origin", &line.origin.x, 0.01f);
		ImGui::DragFloat3("Segment.diff", &line.diff.x, 0.01f);
		ImGui::End();

		DrawGrid(viewProjectionMatrix, viewportMatrix);
		DrawLine(line, viewProjectionMatrix, viewportMatrix, color);
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


void DrawLine(const Segment& segment, const Matrix4x4& viewProjection, const Matrix4x4& viewport, uint32_t color){
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
