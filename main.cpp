#include "function.h"

const char kWindowTitle[] = "LC1B_10_カワグチ_ハルキ_MT3";

void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {
    const float kGridHalfWidth = 2.0f;
    const uint32_t kSubdivision = 10;
    const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision);

    // 奥から手前への線を引いていく
    for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex) {
        // ワールド座標系での始点と終点を計算
        float xPos = -kGridHalfWidth + xIndex * kGridEvery;
        Vector4 startWorld(xPos, 0.0f, -kGridHalfWidth, 1.0f);
        Vector4 endWorld(xPos, 0.0f, kGridHalfWidth, 1.0f);

        // ワールド座標系からクリップ座標系への変換
        Vector4 startClip = Multiply(viewProjectionMatrix, startWorld);
        Vector4 endClip = Multiply(viewProjectionMatrix, endWorld);

        // クリップ座標系からスクリーン座標系への変換（ビューポート変換）
        Vector4 startScreen = Multiply(viewportMatrix, startClip);
        Vector4 endScreen = Multiply(viewportMatrix, endClip);

        // スクリーン座標系で線を描画
        Novice::DrawLine(
            static_cast<int>(startScreen.x),  // 始点のスクリーン X 座標
            static_cast<int>(startScreen.y),  // 始点のスクリーン Y 座標
            static_cast<int>(endScreen.x),    // 終点のスクリーン X 座標
            static_cast<int>(endScreen.y),    // 終点のスクリーン Y 座標
            0xFFFFFFFF);  // 白色で線を描画
    }

    // 左から右も同じように順々に引いていく
    for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex) {
        // ワールド座標系での始点と終点を計算
        float zPos = -kGridHalfWidth + zIndex * kGridEvery;
        Vector4 startWorld(-kGridHalfWidth, 0.0f, zPos, 1.0f);
        Vector4 endWorld(kGridHalfWidth, 0.0f, zPos, 1.0f);

        // ワールド座標系からクリップ座標系への変換
        Vector4 startClip = Multiply(viewProjectionMatrix, startWorld);
        Vector4 endClip = Multiply(viewProjectionMatrix, endWorld);

        // クリップ座標系からスクリーン座標系への変換（ビューポート変換）
        Vector4 startScreen = Multiply(viewportMatrix, startClip);
        Vector4 endScreen = Multiply(viewportMatrix, endClip);

        // スクリーン座標系で線を描画
        Novice::DrawLine(
            static_cast<int>(startScreen.x),  // 始点のスクリーン X 座標
            static_cast<int>(startScreen.y),  // 始点のスクリーン Y 座標
            static_cast<int>(endScreen.x),    // 終点のスクリーン X 座標
            static_cast<int>(endScreen.y),    // 終点のスクリーン Y 座標
            0xFFFFFFFF);  // 白色で線を描画
    }
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

    // 透視投影行列の作成
    float fovY = 60.0f; // 視野角（縦方向）
    float aspectRatio = kWindowWidth / kWindowHeight; // アスペクト比
    float nearClip = 0.1f; // クリップ面（近い方）
    float farClip = 100.0f; // クリップ面（遠い方）
   

    // ビューポート変換行列の作成
    float viewportLeft = 0.0f; // ビューポートの左上隅の X 座標
    float viewportTop = 0.0f; // ビューポートの左上隅の Y 座標
    float viewportWidth = kWindowWidth; // ビューポートの幅
    float viewportHeight = kWindowHeight; // ビューポートの高さ
    float viewportMinDepth = 0.0f; // クリップ面（近い方）
    float viewportMaxDepth = 1.0f; // クリップ面（遠い方）
   

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

        Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(fovY, aspectRatio, nearClip, farClip);
        Matrix4x4 viewportMatrix = MakeViewportMatrix(viewportLeft, viewportTop, viewportWidth, viewportHeight, viewportMinDepth, viewportMaxDepth);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

        // viewProjectionMatrix と viewportMatrix を使用して DrawGrid 関数を呼び出す
        DrawGrid(projectionMatrix, viewportMatrix);

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


