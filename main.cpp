#define NOMINMAX
#include "function.h"
#include <cmath>

const char kWindowTitle[] = "Bezier";

Vector3 Leap(const Vector3& v1, const Vector3& v2, float t);
void DrawBezier(const Vector3& controlPoint0, const Vector3& controlPoint1, const Vector3& controlPoint2, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);
void DrawPoints(const Vector3 controlPoints[], int numPoints, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

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

    Vector3 controlPoints[3] = {
        {-0.8f, 0.58f, 1.0f},
        {1.76f, 1.0f, -0.3f},
        {0.94f, -0.7f, 2.3f},
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

        ///
        /// ↑更新処理ここまで
        ///

        ///
        /// ↓描画処理ここから
        ///

        CameraMove(cameraRotate, cameraPosition, clickPos, keys, preKeys);

        ImGui::Begin("Window");
        ImGui::DragFloat3("controlPoints[0]", &controlPoints[0].x, 0.01f);
        ImGui::DragFloat3("controlPoints[1]", &controlPoints[1].x, 0.01f);
        ImGui::DragFloat3("controlPoints[2]", &controlPoints[2].x, 0.01f);
        ImGui::End();

        DrawGrid(viewProjectionMatrix, viewportMatrix);

        // ベジェ曲線の描画
        DrawBezier(controlPoints[0], controlPoints[1], controlPoints[2], viewProjectionMatrix, viewportMatrix, 0xFFFFFFFF);

        DrawPoints(controlPoints, 3, viewProjectionMatrix, viewportMatrix, 0x000000FF);

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

Vector3 Leap(const Vector3& v1, const Vector3& v2, float t)
{
    return {
        v1.x + (v2.x - v1.x) * t,
        v1.y + (v2.y - v1.y) * t,
        v1.z + (v2.z - v1.z) * t
    };
}

void DrawBezier(const Vector3& controlPoint0, const Vector3& controlPoint1, const Vector3& controlPoint2, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
    const int numSegments = 100;
    for (int i = 0; i < numSegments; ++i) {
        float t1 = float(i) / numSegments;
        float t2 = float(i + 1) / numSegments;

        Vector3 p1 = Leap(Leap(controlPoint0, controlPoint1, t1), Leap(controlPoint1, controlPoint2, t1), t1);
        Vector3 p2 = Leap(Leap(controlPoint0, controlPoint1, t2), Leap(controlPoint1, controlPoint2, t2), t2);

        Vector3 screenP1 = Transform(p1, viewProjectionMatrix);
        Vector3 screenP2 = Transform(p2, viewProjectionMatrix);

        screenP1 = Transform(screenP1, viewportMatrix);
        screenP2 = Transform(screenP2, viewportMatrix);

        Novice::DrawLine((int)screenP1.x, (int)screenP1.y, (int)screenP2.x, (int)screenP2.y, color);
    }
}

void DrawPoints(const Vector3 controlPoints[], int numPoints, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
    for (int i = 0; i < numPoints; ++i) {
        Sphere screenPos = { controlPoints[i], 0.01f };
        DrawSphere(screenPos, viewProjectionMatrix, viewportMatrix, color);
    }
}
