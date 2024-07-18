#define NOMINMAX
#include "function.h"

const char kWindowTitle[] = "LE2B_08_カワグチ_ハルキ";

bool IsCollision(const OBB& obb, const Segment& segment);



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

    Segment segment{
         .origin{-0.7f, -0.3f, 0.0f},
         .diff{2.0f, -0.5f, 0.0f}
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

        isHitOBB = IsCollision(obb, segment);

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
        ImGui::DragFloat3("segment.origin", &segment.origin.x, 0.01f);
        ImGui::DragFloat3("segment.diff", &segment.diff.x, 0.01f);
        ImGui::End();

        // OBBの回転を計算
        Matrix4x4 rotationMatrix = MakeRotateXYZMatrix(obbRotate);
        obb.orientations[0] = Transform(Vector3{ 1.0f, 0.0f, 0.0f }, rotationMatrix);
        obb.orientations[1] = Transform(Vector3{ 0.0f, 1.0f, 0.0f }, rotationMatrix);
        obb.orientations[2] = Transform(Vector3{ 0.0f, 0.0f, 1.0f }, rotationMatrix);


        DrawGrid(viewProjectionMatrix, viewportMatrix);
        DrawOBB(obb, viewProjectionMatrix, viewportMatrix, colorOBB);
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

bool IsCollision(const OBB& obb, const Segment& segment) {
    // OBBの中心を基準にした線分の始点
    Vector3 p = segment.origin - obb.center;
    Vector3 d = segment.diff * 0.5f; // 線分の半分の長さ
    Vector3 extent = obb.size; // OBBのサイズ

    // OBBの座標軸を取得
    Vector3 axes[3];
    for (int i = 0; i < 3; ++i) {
        axes[i] = obb.orientations[i];
    }

    // 1. OBBの軸に対する分離軸定理のテスト
    for (int i = 0; i < 3; ++i) {
        Vector3 axis = axes[i];
        float e = extent.x; // OBBの半分のサイズ
        float r = std::abs(Dot(d, axis)); // 線分の半分の長さとOBBの軸のドット積
        float s = std::abs(Dot(p, axis)); // 線分の始点からOBBの中心までの距離とOBBの軸のドット積
        if (s > e + r) {
            return false; // 衝突していない
        }
    }

    // 2. 線分の方向に対する分離軸定理のテスト
    for (int i = 0; i < 3; ++i) {
        Vector3 axis = segment.diff; // 線分の方向
        float e = std::abs(Dot(extent, axes[i])); // OBBのサイズと軸のドット積
        float s = std::abs(Dot(p, axes[i])); // 始点から中心への距離とOBBの軸のドット積
        if (s > e + std::abs(Dot(segment.diff, axes[i]))) {
            return false; // 衝突していない
        }
    }

    return true; // 衝突している
}
