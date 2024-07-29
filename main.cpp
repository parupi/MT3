#define NOMINMAX
#include "function.h"
#include <cmath>

const char kWindowTitle[] = "";

struct ConicalPendulum {
    Vector3 anchor; // アンカーポイント
    float length; // 紐の長さ
    float halfApexAngle; // 円錐の頂角の半分
    float angle; // 現在の角度
    float angularVelocity; // 角速度ω
};

struct Pendulum {
    Vector3 anchor; // アンカーポイント。固定された端の位置
    float length; // 紐の長さ
    float angle; // 現在の角度
    float angularVelocity; // 角速度ω
    float angularAcceleration; // 各加速度
};

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

    Vector3 position{};

    ConicalPendulum conicalPendulum;
    conicalPendulum.anchor = { 0.0f, 1.0f, 0.0f };
    conicalPendulum.length = 0.8f;
    conicalPendulum.halfApexAngle = 0.7f;
    conicalPendulum.angle = 0.0f;
    conicalPendulum.angularVelocity = 0.0f;

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
            conicalPendulum.angularVelocity = std::sqrt(9.8f / (conicalPendulum.length * std::cos(conicalPendulum.halfApexAngle)));
            conicalPendulum.angle += conicalPendulum.angularVelocity * deltaTime;
        }

        float radius = std::sin(conicalPendulum.halfApexAngle) * conicalPendulum.length;
        float height = std::cos(conicalPendulum.halfApexAngle) * conicalPendulum.length;

        position.x = conicalPendulum.anchor.x + std::cos(conicalPendulum.angle) * radius;
        position.y = conicalPendulum.anchor.y - height;
        position.z = conicalPendulum.anchor.z - std::sin(conicalPendulum.angle) * radius;

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
        }
        ImGui::End();

        DrawGrid(viewProjectionMatrix, viewportMatrix);
        DrawLine(conicalPendulum.anchor, position, viewProjectionMatrix, viewportMatrix, 0xFFFFFFFF);
        DrawPoint(position, viewProjectionMatrix, viewportMatrix, 0xFFFFFFFF);

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
