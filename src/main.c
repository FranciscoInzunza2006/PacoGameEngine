#include <math.h>
#include <stddef.h>
#include <stdlib.h>

#include "raylib.h"
#include "resource_dir.h"

Texture* sprites = NULL;

enum DIRECTION {
    LEFT = 0,
    UP = 90,
    RIGHT = 180,
    DOWN = 270,
};

enum SPRITES_ID {
    SPR_TEST,
    SPR_BATTER_RIGHT,
    SPR_BATTER_UP,
    SPR_BATTER_LEFT,
    SPR_BATTER_DOWN,
    SPR_BATTER_WALKING_RIGHT,
    SPR_BATTER_WALKING_UP,
    SPR_BATTER_WALKING_LEFT,
    SPR_BATTER_WALKING_DOWN,
    SPRITES_TOTAL
};

void registerSprite(enum SPRITES_ID sprite, const char* resource_path) {
    sprites[sprite] = LoadTexture(resource_path);
}

void registerSprites() {
    sprites = malloc(sizeof(Texture) * SPRITES_TOTAL);

    registerSprite(SPR_TEST, "wabbit_alpha.png");
    registerSprite(SPR_BATTER_RIGHT, "spr_batter_right.png");
    registerSprite(SPR_BATTER_UP, "spr_batter_up.png");
    registerSprite(SPR_BATTER_LEFT, "spr_batter_left.png");
    registerSprite(SPR_BATTER_DOWN, "spr_batter_down.png");

    registerSprite(SPR_BATTER_WALKING_RIGHT, "spr_batter_walking_right.png");
    registerSprite(SPR_BATTER_WALKING_UP, "spr_batter_walking_up.png");
    registerSprite(SPR_BATTER_WALKING_LEFT, "spr_batter_walking_left.png");
    registerSprite(SPR_BATTER_WALKING_DOWN, "spr_batter_walking_down.png");
}

int main() {
    // Setup
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    SearchAndSetResourceDir("resources");

    const int window_width = 800;
    const int window_height = 450;

    const int camera_width = 400;
    const int camera_height = 225;

    const float camera_window_ratio = (float) window_width / (float) camera_width;

    InitWindow(window_width, window_height, "Paco's Game Engine");
    registerSprites();

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

    float image_index = 0.0f;
    float image_speed = 0.2f;
    float image_number = 4;
    bool is_moving = false;
    enum DIRECTION direction = DOWN;

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

        is_moving = horizontal_direction != 0 || vertical_direction != 0;
        if (is_moving) {
            if (vertical_direction != 0) {
                if (vertical_direction == 1) direction = DOWN;
                else direction = UP;
            } else {
                if (horizontal_direction == 1) direction = RIGHT;
                else direction = LEFT;
            }

            image_index += image_speed;
            if (image_index > image_number) image_index -= image_number;
        }
        else image_index = 0;


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

        enum SPRITES_ID player = 0;
        if (!is_moving) {
            switch (direction) {
                case UP: player = SPR_BATTER_DOWN; break;
                case LEFT: player = SPR_BATTER_LEFT; break;
                case RIGHT: player = SPR_BATTER_RIGHT; break;
                case DOWN: player = SPR_BATTER_DOWN; break;
            }
        } else {

        }

        //DrawRectangle((int) position.x - 16, (int) position.y - 16, 32, 32, RED);

        for (int sprite = 0; sprite <  SPRITES_TOTAL; sprite++) {
            if (IsTextureValid(sprites[sprite])) {
                const Texture* texture = &sprites[sprite];
                DrawTexture(*texture, 20 + 16 * sprite, 16, WHITE);
            }
        }

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
