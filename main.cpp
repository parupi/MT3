#define NOMINMAX
#include "function.h"

const char kWindowTitle[] = "LE2B_08_カワグチ_ハルキ";

bool IsCollision(const AABB& aabb, const Segment& segmet);

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
        .max{0.5f, 0.5f, 0.5f},
    };

    Segment segment{
        .origin{-0.7f, -0.3f, 0.0f},
        .diff{2.0f, -0.5f, 0.0f}
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

        isHit = IsCollision(aabb, segment);

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
        ImGui::DragFloat3("sphere.center", &segment.origin.x, 0.01f);
        ImGui::DragFloat3("sphere.radius", &segment.diff.x, 0.01f);
        ImGui::End();

        DrawGrid(viewProjectionMatrix, viewportMatrix);
        DrawAABB(aabb, viewProjectionMatrix, viewportMatrix, color);
        DrawLine(segment, viewProjectionMatrix, viewportMatrix, 0xFFFFFFFF);

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

bool IsCollision(const AABB& aabb, const Segment& segment) {
    // 線分の始点と終点を計算
    Vector3 p0 = segment.origin;
    Vector3 p1 = segment.origin + segment.diff;

    // 線分の各成分に対するt値の最小と最大を求める
    float tmin = 0.0f;
    float tmax = 1.0f;

    // x軸についてチェック
    if (std::abs(p1.x - p0.x) < 1e-8) {
        if (p0.x < aabb.min.x || p0.x > aabb.max.x) {
            return false;
        }
    }
    else {
        float invD = 1.0f / (p1.x - p0.x);
        float t1 = (aabb.min.x - p0.x) * invD;
        float t2 = (aabb.max.x - p0.x) * invD;
        if (t1 > t2) { std::swap(t1, t2); }
        tmin = std::max(tmin, t1);
        tmax = std::min(tmax, t2);
        if (tmin > tmax) { return false; }
    }

    // y軸についてチェック
    if (std::abs(p1.y - p0.y) < 1e-8) {
        if (p0.y < aabb.min.y || p0.y > aabb.max.y) {
            return false;
        }
    }
    else {
        float invD = 1.0f / (p1.y - p0.y);
        float t1 = (aabb.min.y - p0.y) * invD;
        float t2 = (aabb.max.y - p0.y) * invD;
        if (t1 > t2) { std::swap(t1, t2); };
        tmin = std::max(tmin, t1);
        tmax = std::min(tmax, t2);
        if (tmin > tmax) { return false; }
    }

    // z軸についてチェック
    if (std::abs(p1.z - p0.z) < 1e-8) {
        if (p0.z < aabb.min.z || p0.z > aabb.max.z) {
            return false;
        }
    }
    else {
        float invD = 1.0f / (p1.z - p0.z);
        float t1 = (aabb.min.z - p0.z) * invD;
        float t2 = (aabb.max.z - p0.z) * invD;
        if (t1 > t2) { std::swap(t1, t2); };
        tmin = std::max(tmin, t1);
        tmax = std::min(tmax, t2);
        if (tmin > tmax) { return false; }
    }

    return true;
}