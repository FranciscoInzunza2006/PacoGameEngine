#include "raylib.h"
#include "resource_dir.h"

int main() {
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

    int window_width = 800;
    int window_height = 450;

    const int camera_width = 100;
    const int camera_height = 100;

    const float camera_ration = (float)cam

    Camera2D camera = {0};
    camera.zoom = 1.0f;

    RenderTexture2D application_surface = LoadRenderTexture(camera_width, camera_height);

    InitWindow(window_width, window_height, "Hello Raylib");

    SearchAndSetResourceDir("resources");
    Texture wabbit = {0};

    Vector2 position = {0};
    float speed = 1.0f;

    while (!WindowShouldClose()) {
        /// STEP
        const bool up = IsKeyDown(KEY_UP);
        const bool down = IsKeyDown(KEY_DOWN);
        const bool left = IsKeyDown(KEY_LEFT);
        const bool right = IsKeyDown(KEY_RIGHT);

        const int horizontal_direction = right - left;
        const int vertical_direction = down - up;

        position.x += horizontal_direction * speed;
        position.y += vertical_direction * speed;

        camera.target = position;

        /// DRAW
        BeginTextureMode(application_surface);
        BeginMode2D(camera);

        ClearBackground(BLACK);
        DrawRectangle((int) position.x - 16, (int) position.y - 16, 32, 32, RED);
        DrawText("Hello Raylib", 200, 200, 20,WHITE);

        EndMode2D();
        EndTextureMode();

        /// DRAW GUI
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexture(application_surface.texture, 0, 0, WHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
