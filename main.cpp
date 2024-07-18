#define NOMINMAX
#include "function.h"

const char kWindowTitle[] = "LE2B_08_カワグチ_ハルキ";

struct OBB {
    Vector3 center; //!< 中心点
    Vector3 orientations[3]; //!< 座標軸。正規化・直行必須
    Vector3 size; //!< 座標軸方向の長さの半分。中心から面までの距離
};

void DrawOBB(const OBB& obb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);
bool IsCollision(const OBB& obb, const Sphere& sphere);

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

    OBB obb{
        .center{0.0f, 0.0f, 0.0f},
        .orientations{ {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f} },
        .size{0.5f, 0.5f, 0.5f},
    };

    Sphere sphere{
        .center{0.0f, 0.0f, 0.0f},
        .radius{1.0f},
    };

    uint32_t colorOBB = 0xFFFFFFFF;
    bool isHitOBB = false;

    Vector3 obbRotate{ 0.0f, 0.0f, 0.0f };

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

        isHitOBB = IsCollision(obb, sphere);

        if (isHitOBB) {
            colorOBB = 0xFF0000FF;
        }
        else {
            colorOBB = 0xFFFFFFFF;
        }

        ///
        /// ↑更新処理ここまで
        ///

        ///
        /// ↓描画処理ここから
        ///

        CameraMove(cameraRotate, cameraPosition, clickPos, keys, preKeys);

        ImGui::Begin("Window");
        ImGui::DragFloat3("obb.center", &obb.center.x, 0.01f);
        ImGui::DragFloat3("obb.size", &obb.size.x, 0.01f);
        ImGui::DragFloat3("obb.rotate", &obbRotate.x, 0.01f);
        ImGui::DragFloat3("sphere.center", &sphere.center.x, 0.01f);
        ImGui::DragFloat("sphere.radius", &sphere.radius, 0.01f);
        ImGui::End();

        // OBBの回転を計算
        Matrix4x4 rotationMatrix = MakeRotateXYZMatrix(obbRotate);
        obb.orientations[0] = Transform(Vector3{ 1.0f, 0.0f, 0.0f }, rotationMatrix);
        obb.orientations[1] = Transform(Vector3{ 0.0f, 1.0f, 0.0f }, rotationMatrix);
        obb.orientations[2] = Transform(Vector3{ 0.0f, 0.0f, 1.0f }, rotationMatrix);


        DrawGrid(viewProjectionMatrix, viewportMatrix);
        DrawOBB(obb, viewProjectionMatrix, viewportMatrix, colorOBB);
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

void DrawOBB(const OBB& obb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
    Vector3 vertices[8];

    // 8つの頂点を計算
    for (int i = 0; i < 8; ++i) {
        Vector3 vertex = obb.center;
        vertex += obb.orientations[0] * obb.size.x * (i & 1 ? 1.0f : -1.0f);
        vertex += obb.orientations[1] * obb.size.y * (i & 2 ? 1.0f : -1.0f);
        vertex += obb.orientations[2] * obb.size.z * (i & 4 ? 1.0f : -1.0f);
        vertices[i] = Transform(Transform(vertex, viewProjectionMatrix), viewportMatrix);
    }

    // 12本のエッジを描画
    static const int indices[12][2] = {
        { 0, 1 }, { 1, 3 }, { 3, 2 }, { 2, 0 },
        { 4, 5 }, { 5, 7 }, { 7, 6 }, { 6, 4 },
        { 0, 4 }, { 1, 5 }, { 2, 6 }, { 3, 7 }
    };

    for (int i = 0; i < 12; ++i) {
        Novice::DrawLine((int)vertices[indices[i][0]].x, (int)vertices[indices[i][0]].y, (int)vertices[indices[i][1]].x, (int)vertices[indices[i][1]].y, color);
    }
}

bool IsCollision(const OBB& obb, const Sphere& sphere) {
    Vector3 d = sphere.center - obb.center;

    for (int i = 0; i < 3; ++i) {
        float dist = std::abs(Dot(d, obb.orientations[i]));
        if (dist > obb.size.x + sphere.radius) {
            return false;
        }
    }

    return true;
}

