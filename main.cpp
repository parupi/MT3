#define NOMINMAX
#include "function.h"
#include <cmath>

const char kWindowTitle[] = "";

struct Ball {
    Vector3 position;
    float mass;
    float radius;
    uint32_t color;
    Vector3 velocity;
    Vector3 acceleration;
};

Vector3 Reflect(const Vector3& input, const Vector3& normal);

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

    Plane plane{};
    plane.normal = Normalize({ -0.2f, 1.2f, -0.3f });
    plane.distance = 0.0f;

    Ball ball{};
    ball.position = { 0.8f, 1.2f, 0.3f };
    ball.mass = 2.0f;
    ball.radius = 0.05f;
    ball.color = 0xFFFFFFFF;
    ball.acceleration = { 0.0f, -9.8f, 0.0f };
    ball.velocity = { 0.0f, 0.0f, 0.0f };

    float deltaTime = 1.0f / 60.0f;
    bool isStart = false;

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

        if (isStart) {
            ball.velocity += ball.acceleration * deltaTime;
            ball.position += ball.velocity * deltaTime;

            if (IsCollision(Sphere{ ball.position, ball.radius }, plane)) {
                Vector3 reflected = Reflect(ball.velocity, plane.normal);

                // 法線ベクトルへの投影
                Vector3 projectToNormal = Project(reflected, plane.normal);

                // 接線ベクトルを計算
                Vector3 tangent = reflected - projectToNormal;

                // 新しい進行方向を計算 (法線方向の速度を減衰させる)
                ball.velocity = projectToNormal * 0.8f + tangent;
            }
        }

        if (ball.position.y <= -3.0f) {
            isStart = false;
        }

        ///
        /// ↑更新処理ここまで
        ///

        ///
        /// ↓描画処理ここから
        ///

        CameraMove(cameraRotate, cameraPosition, clickPos, keys, preKeys);

        ImGui::Begin("Window");
        if (ImGui::Button("start")) {
            isStart = true;
            ball.acceleration = { 0.0f, -9.8f, 0.0f };
            ball.position = { 0.8f, 1.2f, 0.3f };
            ball.velocity = { 0.0f, 0.0f, 0.0f };
            ball.mass = 2.0f;
            ball.radius = 0.05f;
            ball.color = 0xFFFFFFFF;
        }
        ImGui::End();

        DrawGrid(viewProjectionMatrix, viewportMatrix);
        if (isStart) {
            DrawPoint(ball.position, viewProjectionMatrix, viewportMatrix, 0xFFFFFFFF);
        }
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

Vector3 Reflect(const Vector3& input, const Vector3& normal) {
    return input - Multiply(2, Multiply(Dot(input, normal), normal));
}
