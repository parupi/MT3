#define NOMINMAX
#include "function.h"
#include <cmath>

const char kWindowTitle[] = "";

void DrawPoint(const Vector3& position, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);
void DrawLine(const Vector3& start, const Vector3& end, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);
Vector3 TransformCoord(const Vector3& v, const Matrix4x4& m);

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

    Vector3 translates[3] = {
        {0.2f, 1.0f, 0.0f},
        {0.4f, 0.0f, 0.0f},
        {0.3f, 0.0f, 0.0f},
    };

    Vector3 rotates[3] = {
        {0.0f, 0.0f, -6.8f},
        {0.0f, 0.0f, -1.4f},
        {0.0f, 0.0f, 0.0f },
    };

    Vector3 scales[3] = {
        {1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f},
    };

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
        Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);
        Matrix4x4 viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);

        // 親子付けされた変換行列の計算
        Matrix4x4 shoulderMatrix = MakeAffineMatrix(scales[0], rotates[0], translates[0]);
        Matrix4x4 elbowMatrix = Multiply(shoulderMatrix, MakeAffineMatrix(scales[1], rotates[1], translates[1]));
        Matrix4x4 handMatrix = Multiply(elbowMatrix, MakeAffineMatrix(scales[2], rotates[2], translates[2]));

        // 各点の位置を抽出
        Vector3 shoulderPos = { shoulderMatrix.m[3][0], shoulderMatrix.m[3][1], shoulderMatrix.m[3][2] };
        Vector3 elbowPos = { elbowMatrix.m[3][0], elbowMatrix.m[3][1], elbowMatrix.m[3][2] };
        Vector3 handPos = { handMatrix.m[3][0], handMatrix.m[3][1], handMatrix.m[3][2] };

        ///
        /// ↑更新処理ここまで
        ///

        ///
        /// ↓描画処理ここから
        ///

        CameraMove(cameraRotate, cameraPosition, clickPos, keys, preKeys);

        ImGui::Begin("Window");
        ImGui::DragFloat3("translates[0]", &translates[0].x, 0.01f);
        ImGui::DragFloat3("rotates[0]", &rotates[1].x, 0.01f);
        ImGui::DragFloat3("scales[0]", &scales[0].x, 0.01f);
        ImGui::DragFloat3("translates[1]", &translates[1].x, 0.01f);
        ImGui::DragFloat3("rotates[1]", &rotates[2].x, 0.01f);
        ImGui::DragFloat3("scales[1]", &scales[1].x, 0.01f);
        ImGui::DragFloat3("translates[2]", &translates[2].x, 0.01f);
        ImGui::DragFloat3("rotates[2]", &rotates[2].x, 0.01f);
        ImGui::DragFloat3("scales[2]", &scales[2].x, 0.01f);
        ImGui::End();

        DrawGrid(viewProjectionMatrix, viewportMatrix);

        // 各点の位置を描画
        DrawPoint(shoulderPos, viewProjectionMatrix, viewportMatrix, 0xFF0000FF);
        DrawPoint(elbowPos, viewProjectionMatrix, viewportMatrix, 0x00FF00FF);
        DrawPoint(handPos, viewProjectionMatrix, viewportMatrix, 0x0000FFFF);

        // 点と点の間に線を描画
        DrawLine(shoulderPos, elbowPos, viewProjectionMatrix, viewportMatrix, 0xFFFFFFFF);
        DrawLine(elbowPos, handPos, viewProjectionMatrix, viewportMatrix, 0xFFFFFFFF);

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

// 点を描画する関数
void DrawPoint(const Vector3& position, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
    Sphere sphere = { position, 0.1f };
    DrawSphere(sphere, viewProjectionMatrix, viewportMatrix, color);
}

// 線を描画する関数
void DrawLine(const Vector3& start, const Vector3& end, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
    Vector3 startScreen = TransformCoord(start, viewProjectionMatrix);
    startScreen = TransformCoord(startScreen, viewportMatrix);
    Vector3 endScreen = TransformCoord(end, viewProjectionMatrix);
    endScreen = TransformCoord(endScreen, viewportMatrix);
    Novice::DrawLine(int(startScreen.x), int(startScreen.y), int(endScreen.x), int(endScreen.y), color);
}

// 3Dベクトルを4x4行列で変換する関数
Vector3 TransformCoord(const Vector3& v, const Matrix4x4& m) {
    Vector3 result;
    float w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];
    result.x = (v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0]) / w;
    result.y = (v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1]) / w;
    result.z = (v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2]) / w;
    return result;
}
