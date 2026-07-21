#include <math.h>
#include <stdlib.h>

#include "raylib.h"
#include "resource_dir.h"
#include "entity.h"

typedef enum {
    LEFT = 0,
    UP = 90,
    RIGHT = 180,
    DOWN = 270,
} Direction;

typedef enum {
    SPR_TEST,
    SPR_BATTER_RIGHT,
    SPR_BATTER_UP,
    SPR_BATTER_LEFT,
    SPR_BATTER_DOWN,
    SPRITES_TOTAL
} SpriteID;

void registerSprites();
void registerSprite(SpriteID id, const char* resource_path);

void setup(void);
void cleanup(void);
void step(void);
void draw(void);
void drawGUI(void);

void updatePlayer(void);
void updateCamera(void);

constexpr int window_width = 800;
constexpr int window_height = 450;

constexpr int camera_width = 400;
constexpr int camera_height = 225;
constexpr float camera_window_ratio = (float) window_width / (float) camera_width;

Texture* sprites = nullptr;
Camera2D camera = {0};
RenderTexture2D application_surface = {0};
constexpr Vector2 origin = {0.0f, 0.0f};

Rectangle destRec = {0};

float cameraX = 0.0f;
float cameraY = 0.0f;

Vector2 position = {0};
float speed = 1.0f;

float image_index = 0.0f;
float image_speed = 0.1f;
float image_number = 4;
bool is_moving = false;
Direction direction = DOWN;

typedef struct {
    Vector2 position;
    float speed;
    float image_index;
    float image_number;
    bool is_moving;
    Direction direction;
} ObjBatter;


int main() {
    setup();
    entity_hi();
    while (!WindowShouldClose()) {
        step();
        draw();
        drawGUI();
    }

    cleanup();
    return 0;
}

void setup(void) {
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    SearchAndSetResourceDir("resources");
    InitWindow(window_width, window_height, "Paco's Game Engine");
    SetTargetFPS(60);

    registerSprites();
    application_surface = LoadRenderTexture(camera_width, camera_height);
    camera.zoom = 1.0f;
}

void cleanup(void) {
    CloseWindow();
}

void step(void) {
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
    } else image_index = 0;
}

void draw(void) {
    BeginTextureMode(application_surface);
    ClearBackground(RAYWHITE);

    BeginMode2D(camera);

    SpriteID player_sprite = 0;
    switch (direction) {
        case UP: player_sprite = SPR_BATTER_UP;
            break;
        case LEFT: player_sprite = SPR_BATTER_LEFT;
            break;
        case RIGHT: player_sprite = SPR_BATTER_RIGHT;
            break;
        case DOWN: player_sprite = SPR_BATTER_DOWN;
            break;
    }

    Rectangle frameRectangle = (Rectangle){16.0f * truncf(image_index), 0, 16, 24};
    DrawTextureRec(sprites[player_sprite], frameRectangle, (Vector2){position.x, position.y}, WHITE);

    for (int sprite = 0; sprite < SPRITES_TOTAL; sprite++) {
        if (IsTextureValid(sprites[sprite])) {
            const Texture* texture = &sprites[sprite];
            DrawTexture(*texture, 20 + 16 * sprite, 16, WHITE);
        }
    }

    EndMode2D();
    EndTextureMode();
}

void drawGUI(void) {
    const Rectangle sourceRec = {
        0.0f, 0.0f, (float) application_surface.texture.width, -(float) application_surface.texture.height
    };
    Camera2D rendering_camera = {0};
    rendering_camera.zoom = 1.0f;

    // Make the camera move to demonstrate the effect
    cameraX = position.x - (float) camera_width / 2;
    cameraY = position.y - (float) camera_height / 2;

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

void updatePlayer(void) {
}

void updateCamera(void) {
}

void registerSprite(const SpriteID id, const char* resource_path) {
    sprites[id] = LoadTexture(resource_path);
}

void registerSprites() {
    sprites = malloc(sizeof(Texture) * SPRITES_TOTAL);

    registerSprite(SPR_TEST, "wabbit_alpha.png");

    registerSprite(SPR_BATTER_RIGHT, "spr_batter_right.png");
    registerSprite(SPR_BATTER_UP, "spr_batter_up.png");
    registerSprite(SPR_BATTER_LEFT, "spr_batter_left.png");
    registerSprite(SPR_BATTER_DOWN, "spr_batter_down.png");
}
