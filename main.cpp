#include "function.h"

const char kWindowTitle[] = "LE2B_08_カワグチ_ハルキ";

struct AABB {
    Vector3 min;    //!< 最小点
    Vector3 max;    //!< 最大点
};

bool IsCollision(const AABB& aabb1, const AABB& aabb2);
void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

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

    AABB aabb1{
        .min{-0.5f, -0.5f, -0.5f},
        .max{0.0f, 0.0f, 0.0f},
    };

    AABB aabb2{
       .min{0.2f, 0.2f, 0.2f},
       .max{1.0f, 1.0f, 1.0f},
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

        aabb1.min.x = (std::min)(aabb1.min.x, aabb1.max.x);
        aabb1.max.x = (std::max)(aabb1.min.x, aabb1.max.x);
        aabb1.min.y = (std::min)(aabb1.min.y, aabb1.max.y);
        aabb1.max.y = (std::max)(aabb1.min.y, aabb1.max.y);
        aabb1.min.z = (std::min)(aabb1.min.z, aabb1.max.z);
        aabb1.max.z = (std::max)(aabb1.min.z, aabb1.max.z);
        aabb2.min.x = (std::min)(aabb2.min.x, aabb2.max.x);
        aabb2.max.x = (std::max)(aabb2.min.x, aabb2.max.x);
        aabb2.min.y = (std::min)(aabb2.min.y, aabb2.max.y);
        aabb2.max.y = (std::max)(aabb2.min.y, aabb2.max.y);
        aabb2.min.z = (std::min)(aabb2.min.z, aabb2.max.z);
        aabb2.max.z = (std::max)(aabb2.min.z, aabb2.max.z);

        isHit = IsCollision(aabb1, aabb2);

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
        ImGui::DragFloat3("aabb1.min", &aabb1.min.x, 0.1f);
        ImGui::DragFloat3("aabb1.max", &aabb1.max.x, 0.1f);
        ImGui::DragFloat3("aabb2.min", &aabb2.min.x, 0.1f);
        ImGui::DragFloat3("aabb2.max", &aabb2.max.x, 0.1f);
        ImGui::End();

        DrawGrid(viewProjectionMatrix, viewportMatrix);
        DrawAABB(aabb1, viewProjectionMatrix, viewportMatrix, color);
        DrawAABB(aabb2, viewProjectionMatrix, viewportMatrix, 0xFFFFFFFF);

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

bool IsCollision(const AABB& a, const AABB& b)
{
    if ((a.min.x <= b.max.x && a.max.x >= b.min.x) &&
        (a.min.y <= b.max.y && a.max.y >= b.min.y) &&
        (a.min.z <= b.max.z && a.max.z >= b.min.z)) {
        return true;
    }
    else {
        return false;
    }
}

// AABBを描画する関数
void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
    // AABBの8つの頂点を計算
    Vector3 vertices[8] = {
        {aabb.min.x, aabb.min.y, aabb.min.z},
        {aabb.max.x, aabb.min.y, aabb.min.z},
        {aabb.min.x, aabb.max.y, aabb.min.z},
        {aabb.max.x, aabb.max.y, aabb.min.z},
        {aabb.min.x, aabb.min.y, aabb.max.z},
        {aabb.max.x, aabb.min.y, aabb.max.z},
        {aabb.min.x, aabb.max.y, aabb.max.z},
        {aabb.max.x, aabb.max.y, aabb.max.z}
    };

    // 各頂点を変換
    Vector3 transformedVertices[8];
    for (int i = 0; i < 8; ++i) {
        transformedVertices[i] = Transform(Transform(vertices[i], viewProjectionMatrix), viewportMatrix);
    }

    // AABBのエッジを描画
    const int edges[12][2] = {
        {0, 1}, {1, 3}, {3, 2}, {2, 0}, // 前面
        {4, 5}, {5, 7}, {7, 6}, {6, 4}, // 背面
        {0, 4}, {1, 5}, {2, 6}, {3, 7}  // 前面と背面を結ぶエッジ
    };

    for (int i = 0; i < 12; ++i) {
        int x1 = static_cast<int>(transformedVertices[edges[i][0]].x);
        int y1 = static_cast<int>(transformedVertices[edges[i][0]].y);
        int x2 = static_cast<int>(transformedVertices[edges[i][1]].x);
        int y2 = static_cast<int>(transformedVertices[edges[i][1]].y);

        Novice::DrawLine(x1, y1, x2, y2, color);
    }
}
