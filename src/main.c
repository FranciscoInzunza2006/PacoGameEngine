#include <math.h>

#include "raylib.h"
#include "resource_dir.h"

int main() {
    // Setup
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    SearchAndSetResourceDir("resources");

    const int window_width = 800;
    const int window_height = 450;

    const int camera_width = 160;
    const int camera_height = 90;

    const float camera_window_ratio = (float) window_width / (float) camera_width;

    InitWindow(window_width, window_height, "Paco's Game Engine");

    Camera2D camera = {0};
    camera.zoom = 1.0f;

    Camera2D rendering_camera = {0};
    rendering_camera.zoom = 1.0f;

    // Like in gamemaker
    RenderTexture2D application_surface = LoadRenderTexture(camera_width, camera_height);

    const Rectangle sourceRec = {
        0.0f, 0.0f, (float) application_surface.texture.width, -(float) application_surface.texture.height
    };
    Rectangle destRec = {0};

    const Vector2 origin = {0.0f, 0.0f};

    float cameraX = 0.0f;
    float cameraY = 0.0f;

    SetTargetFPS(60);

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

        position.x += (float) horizontal_direction * speed;
        position.y += (float) vertical_direction * speed;

        // Make the camera move to demonstrate the effect
        cameraX = position.x - (float)camera_width/2;
        cameraY = position.y - (float)camera_height/2;

        // Set the camera's target to the values computed above
        rendering_camera.target = (Vector2){cameraX, cameraY};

        // Round worldSpace coordinates, keep decimals into screenSpace coordinates
        camera.target.x = truncf(rendering_camera.target.x);
        rendering_camera.target.x -= camera.target.x;
        rendering_camera.target.x *= camera_window_ratio;

        camera.target.y = truncf(rendering_camera.target.y);
        rendering_camera.target.y -= camera.target.y;
        rendering_camera.target.y *= camera_window_ratio;

        destRec = (Rectangle){
            -camera_window_ratio, -camera_window_ratio, window_width + (camera_window_ratio * 2),
            window_height + (camera_window_ratio * 2)
        };

        // Draw
        BeginTextureMode(application_surface);
        ClearBackground(RAYWHITE);

        BeginMode2D(camera);

        DrawRectangle((int) position.x - 16, (int) position.y - 16, 32, 32, RED);

        DrawRectangle(40, 40, 100, 30, BLUE);
        DrawRectangle(10, 20, 10, 100, YELLOW);

        EndMode2D();
        EndTextureMode();

        // Draw GUI
        BeginDrawing();
        ClearBackground(LIGHTGRAY);

        BeginMode2D(rendering_camera);
        DrawTexturePro(application_surface.texture, sourceRec, destRec, origin, 0.0f, WHITE);
        EndMode2D();

        DrawText(TextFormat("Screen resolution: %ix%i", window_width, window_height), 10, 10, 20, DARKBLUE);
        DrawText(TextFormat("World resolution: %ix%i", camera_width, camera_height), 10, 40, 20, DARKGREEN);
        DrawFPS(GetScreenWidth() - 95, 10);
        EndDrawing();
    }

    UnloadRenderTexture(application_surface);
    CloseWindow();
    return 0;
}
