#define NOMINMAX
#include "function.h"
#include <cmath>

const char kWindowTitle[] = "";

struct Spring {
    // アンカーの位置
    Vector3 anchor;
    float naturalLength; // 自然長
    float stiffness; // 合成。ばね定数k
    float dampingCoefficient; // 減衰係数
};

struct Ball {
    Vector3 position; // 位置
    Vector3 velocity; // 速度
    Vector3 acceleration; // 加速度
    float mass; // 質量
    float radius; // 半径
    unsigned int color; // 色
};

//void DrawLine(Vector3 v1, Vector3 v2, Matrix4x4 viewProjectionMatrix, Matrix4x4 viewportMatrix, uint32_t color);

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

    Spring spring{};
    spring.anchor = { 0.0f, 0.0f, 0.0f };
    spring.naturalLength = 1.0f;
    spring.stiffness = 100.0f;
    spring.dampingCoefficient = 2.0f;

    Ball ball{};
    ball.position = { 1.2f, 0.0f, 0.0f };
    ball.mass = 2.0f;
    ball.radius = 0.05f;
    ball.color = 0x0000FFFF;

    float deltaTime = 1.0f / 60.0f;

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
        Matrix4x4 viewProjectionMatrix = viewMatrix * projectionMatrix;

        Vector3 diff = ball.position - spring.anchor;
        float length = Length(diff);
        if (length != 0.0f) {
            Vector3 direction = Normalize(diff);
            Vector3 restPosition = spring.anchor + direction * spring.naturalLength;
            Vector3 displacement = (ball.position - restPosition) * length;
            Vector3 restoringForce = displacement * -spring.stiffness;
            // 減衰抵抗を計算する
            Vector3 dampingForce = ball.velocity * -spring.dampingCoefficient;
            // 減衰抵抗も加味して、物体にかかる力を決定する
            Vector3 force = restoringForce + dampingForce;
            ball.acceleration = force / ball.mass;
        }
        ball.velocity += ball.acceleration * deltaTime;
        ball.position += ball.velocity * deltaTime;

        ///
        /// ↑更新処理ここまで
        ///

        ///
        /// ↓描画処理ここから
        ///

        CameraMove(cameraRotate, cameraPosition, clickPos, keys, preKeys);

        ImGui::Begin("Window");
        ImGui::DragFloat3("ballPosition", &ball.position.x, 0.01f);
        ImGui::End();

        DrawGrid(viewProjectionMatrix, viewportMatrix);
        DrawLine(spring.anchor, ball.position, viewProjectionMatrix, viewportMatrix, 0xFFFFFFFF);
        DrawPoint(ball.position, viewProjectionMatrix, viewportMatrix, ball.color);

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

//void DrawLine(Vector3 v1, Vector3 v2, Matrix4x4 viewProjection, Matrix4x4 viewport, uint32_t color)
//{
//    // 始点と終点を変換
//    Vector3 transformedStart = Transform(Transform(v1, viewProjection), viewport);
//    Vector3 transformedEnd = Transform(Transform(v2, viewProjection), viewport);
//
//    // スクリーン座標に変換
//    int x1 = static_cast<int>(transformedStart.x);
//    int y1 = static_cast<int>(transformedStart.y);
//    int x2 = static_cast<int>(transformedEnd.x);
//    int y2 = static_cast<int>(transformedEnd.y);
//
//    // 線を描画
//    Novice::DrawLine(x1, y1, x2, y2, color);
//}
