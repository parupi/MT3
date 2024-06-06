#include "function.h"

const char kWindowTitle[] = "LE2B_08_カワグチ_ハルキ";

struct Triangle {
	Vector3 vertices[3];
};

bool IsCollision(const Triangle& triangle, const Segment& segment);
void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

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

    Plane plane{ {0.0f,1.0f,0.0f}, 1.0f };
    Segment line{ {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f} };
    Triangle triangle{ { {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f} } };
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

        isHit = IsCollision(triangle, line);

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
        ImGui::DragFloat3("Triangle.Vertex0", &triangle.vertices[0].x, 0.01f);
        ImGui::DragFloat3("Triangle.Vertex1", &triangle.vertices[1].x, 0.01f);
        ImGui::DragFloat3("Triangle.Vertex2", &triangle.vertices[2].x, 0.01f);
        ImGui::DragFloat3("Segment.Start", &line.origin.x, 0.01f);
        ImGui::DragFloat3("Segment.Diff", &line.diff.x, 0.01f);
        ImGui::End();

        DrawGrid(viewProjectionMatrix, viewportMatrix);
        DrawLine(line, viewProjectionMatrix, viewportMatrix, color);
        DrawTriangle(triangle, viewProjectionMatrix, viewportMatrix, 0xFFFFFFFF);

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

bool IsCollision(const Triangle& triangle, const Segment& segment)
{
    // 三角形の頂点から法線ベクトルを計算
    Vector3 edge1 = triangle.vertices[1] - triangle.vertices[0];
    Vector3 edge2 = triangle.vertices[2] - triangle.vertices[0];
    Vector3 normal = Cross(edge1, edge2);
    normal = Normalize(normal);

    // 平面の方程式を定義
    Plane plane{ normal, Dot(normal, triangle.vertices[0]) };

    // 線分と平面の交差判定
    float dot = Dot(plane.normal, segment.diff);

    // 平面と線分が平行な場合、交差しない
    if (dot == 0.0f) {
        return false;
    }

    // 線分の始点から終点へのベクトルの割合 t を計算
    float t = (plane.distance - Dot(segment.origin, plane.normal)) / dot;

    // tが0から1の間にある場合、交点が線分上にある
    if (t >= 0.0f && t <= 1.0f) {
        // 交点の座標を計算
        Vector3 intersection = segment.origin + segment.diff * t;

        // 交点が三角形内にあるかを判定
        for (int i = 0; i < 3; ++i) {
            Vector3 edge = triangle.vertices[(i + 1) % 3] - triangle.vertices[i];
            Vector3 vp = intersection - triangle.vertices[i];
            Vector3 crossProduct = Cross(edge, vp);
            if (Dot(normal, crossProduct) < 0) {
                return false;
            }
        }
        return true;
    }
    return false;
}


void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	Vector3 transformedVertices[3];
	for (int i = 0; i < 3; ++i) {
		transformedVertices[i] = Transform(Transform(triangle.vertices[i], viewProjectionMatrix), viewportMatrix);
	}

	for (int i = 0; i < 3; ++i) {
		int x1 = static_cast<int>(transformedVertices[i].x);
		int y1 = static_cast<int>(transformedVertices[i].y);
		int x2 = static_cast<int>(transformedVertices[(i + 1) % 3].x);
		int y2 = static_cast<int>(transformedVertices[(i + 1) % 3].y);

		Novice::DrawLine(x1, y1, x2, y2, color);
	}
}

