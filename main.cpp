#define NOMINMAX
#include "function.h"
#define _USE_MATH_DEFINES

const char kWindowTitle[] = "LE2B_08_カワグチ_ハルキ";

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

Vector3 Project(const Vector3& v1, const Vector3& v2);
Vector3 ClosestPoint(const Vector3& point, const Segment& segment);

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	Vector3 cameraTranslate{ 0.0f, 1.9f, -6.49f };
	Vector3 cameraRotate{ 0.26f, 0.0f, 0.0f };
	Vector3 cameraPosition{ 0.0f, 1.0f, -5.0f };
	Segment segment{
		{-2.0f, -1.0f, 0.0f},
		{3.0f,  2.0f,  2.0f}
	};
	Vector3 point{ -1.5f, 0.6f, 0.6f };
	Vector3 project = Project(point - segment.origin, segment.diff);
	Vector3 closestPoint = ClosestPoint(point, segment);
	Sphere pointSphere{ point, 0.01f };
	Sphere closestPointSphere{ closestPoint, 0.01f };


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

		Vector3 start = Transform(Transform(segment.origin, viewProjectionMatrix), viewportMatrix);
		Vector3 end = Transform(Transform(segment.origin + segment.diff, viewProjectionMatrix), viewportMatrix);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		ImGui::Begin("Window");
		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);
		ImGui::DragFloat3("Point", &point.x, 0.01f);
		ImGui::DragFloat3("Segment origin", &segment.origin.x, 0.01f);
		ImGui::DragFloat3("Segment diff", &segment.diff.x, 0.01f);
		ImGui::InputFloat3("Project", &project.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
		ImGui::End();

		DrawGrid(viewProjectionMatrix, viewportMatrix);
		Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), WHITE);
		DrawSphere(pointSphere, viewProjectionMatrix, viewportMatrix, RED);
		DrawSphere(closestPointSphere, viewProjectionMatrix, viewportMatrix, BLACK);

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

Vector3 Project(const Vector3& v1, const Vector3& v2){
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

