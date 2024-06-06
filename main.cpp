#define NOMINMAX
#include "function.h"

const char kWindowTitle[] = "LE2B_08_カワグチ_ハルキ";

bool IsCollision(const AABB& aabb, const Sphere& sphere);

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

    AABB aabb{
        .min{-0.5f, -0.5f, -0.5f},
        .max{0.0f, 0.0f, 0.0f},
    };

    Sphere sphere{
        {0.0f, 0.0f, 0.0f}, 1.0f
    };

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

        Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, cameraRotate, cameraPosition + cameraTranslate);
        Matrix4x4 viewMatrix = Inverse(cameraMatrix);
        Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
        Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);
        Matrix4x4 viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);

        aabb.min.x = (std::min)(aabb.min.x, aabb.max.x);
        aabb.max.x = (std::max)(aabb.min.x, aabb.max.x);
        aabb.min.y = (std::min)(aabb.min.y, aabb.max.y);
        aabb.max.y = (std::max)(aabb.min.y, aabb.max.y);
        aabb.min.z = (std::min)(aabb.min.z, aabb.max.z);
        aabb.max.z = (std::max)(aabb.min.z, aabb.max.z);

        isHit = IsCollision(aabb, sphere);

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

        CameraMove(cameraRotate, cameraPosition, clickPos, keys, preKeys);
        
        ImGui::Begin("Window");
        ImGui::DragFloat3("aabb1.min", &aabb.min.x, 0.01f);
        ImGui::DragFloat3("aabb1.max", &aabb.max.x, 0.01f);
        ImGui::DragFloat3("sphere.center", &sphere.center.x, 0.01f);
        ImGui::DragFloat("sphere.radius", &sphere.radius, 0.01f);
        ImGui::End();

        DrawGrid(viewProjectionMatrix, viewportMatrix);
        DrawAABB(aabb, viewProjectionMatrix, viewportMatrix, color);
        DrawSphere(sphere, viewProjectionMatrix, viewportMatrix, 0xFFFFFFFF);

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

// 球とAABBの当たり判定を行う関数
bool IsCollision(const AABB& aabb, const Sphere& sphere) {
    // 最近接点を計算
    float closestX = std::max(aabb.min.x, std::min(sphere.center.x, aabb.max.x));
    float closestY = std::max(aabb.min.y, std::min(sphere.center.y, aabb.max.y));
    float closestZ = std::max(aabb.min.z, std::min(sphere.center.z, aabb.max.z));

    // 最近接点と球の中心との距離を計算
    float distanceX = sphere.center.x - closestX;
    float distanceY = sphere.center.y - closestY;
    float distanceZ = sphere.center.z - closestZ;

    // 距離の二乗を計算
    float distanceSquared = (distanceX * distanceX) + (distanceY * distanceY) + (distanceZ * distanceZ);

    // 距離の二乗が半径の二乗以下であれば衝突している
    return distanceSquared < (sphere.radius * sphere.radius);
}
