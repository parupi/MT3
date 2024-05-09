#include "function.h"
#define _USE_MATH_DEFINES

const char kWindowTitle[] = "LE2B_08_カワグチ_ハルキ";

struct Sphere {
	Vector3 center;//中心
	float radius;//半径
};

//グリッドと球の描画
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);
void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	Sphere sphere{ Vector3{},0.5f };
	Vector3 cameraTranslate{ 0.0f,1.9f,-6.49f };
	Vector3 cameraRotate{ 0.26f,0.0f,0.0f };

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

		Matrix4x4 camelaMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, cameraRotate, cameraTranslate);
		Matrix4x4 viewMatriix = Inverse(camelaMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, (float)kWindowWidth / (float)kWindowHeight, 0.1f, 100.0f);
		Matrix4x4 viewProjectionMatrix = Multiply(viewMatriix, projectionMatrix);
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, (float)kWindowWidth, (float)kWindowHeight, 0.0f, 1.0f);

		ImGui::Begin("window");
		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);
		ImGui::DragFloat3("SphereCenter", &sphere.center.x, 0.01f);
		ImGui::DragFloat("SphereRadius", &sphere.radius, 0.01f);
		ImGui::End();

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(viewProjectionMatrix, viewportMatrix);
		DrawSphere(sphere, viewProjectionMatrix, viewportMatrix, 0x000000ff);

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

void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	float pi = float(3.14);
	const uint32_t kSubdivision = 10;
	const float kLatEvery = pi / kSubdivision;
	const float kLonEvery = (2 * pi) / kSubdivision;

	Vector3 a, b, c;

	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		sphere;
		float lat = -pi / 2.0f + kLatEvery * latIndex;

		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = lonIndex * kLonEvery;
			//ワールド座標系での頂点を求める
			a = {
				(sphere.center.x + sphere.radius) * (std::cos(lat) * std::cos(lon)),
				(sphere.center.y + sphere.radius) * (std::sin(lat)),
				(sphere.center.z + sphere.radius) * (std::cos(lat) * std::sin(lon))
			};

			b = {
				(sphere.center.x + sphere.radius) * (std::cos(lat + (pi / kSubdivision)) * std::cos(lon)),
				(sphere.center.y + sphere.radius) * (std::sin(lat + (pi / kSubdivision))),
				(sphere.center.z + sphere.radius) * (std::cos(lat + (pi / kSubdivision)) * std::sin(lon))
			};

			c = {
				(sphere.center.x + sphere.radius) * (std::cos(lat) * std::cos(lon + ((pi * 2) / kSubdivision))),
				(sphere.center.y + sphere.radius) * (std::sin(lat)),
				(sphere.center.z + sphere.radius) * (std::cos(lat) * std::sin(lon + ((pi * 2) / kSubdivision)))
			};

			//a,b,cをScreen座標系まで変換
			Matrix4x4 worldMatrixA = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, a);
			Matrix4x4 worldMatrixB = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, b);
			Matrix4x4 worldMatrixC = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, c);

			Matrix4x4 wvpMatrixA = Multiply(worldMatrixA, viewProjectionMatrix);
			Matrix4x4 wvpMatrixB = Multiply(worldMatrixB, viewProjectionMatrix);
			Matrix4x4 wvpMatrixC = Multiply(worldMatrixC, viewProjectionMatrix);

			Vector3 localA = Transform(a, wvpMatrixA);
			Vector3 localB = Transform(b, wvpMatrixB);
			Vector3 localC = Transform(c, wvpMatrixC);

			Vector3 screenA = Transform(localA, viewportMatrix);
			Vector3 screenB = Transform(localB, viewportMatrix);
			Vector3 screenC = Transform(localC, viewportMatrix);

			//ab,bcで線を引く
			Novice::DrawLine((int)screenA.x, (int)screenA.y, (int)screenB.x, (int)screenB.y, color);
			Novice::DrawLine((int)screenA.x, (int)screenA.y, (int)screenC.x, (int)screenC.y, color);

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