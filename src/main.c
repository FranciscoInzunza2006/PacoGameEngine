#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "raylib.h"
#include "entity.h"
#include "raymath.h"

typedef enum {
    RIGHT = 0,
    UP = 90,
    LEFT = 180,
    DOWN = 270,
} Direction;

void obj_logger_create(Entity* self, void* data) {
}

void obj_logger_step(Entity* self) {
    (void) self;

    if (IsKeyPressed(KEY_SPACE)) {
        printf("Hello!\n");
    }
}

void obj_logger_draw(Entity* self) {
    DrawRectangleV(self->position, (Vector2){32, 32}, BLUE);
}

void obj_walker_create(Entity* self, void* data) {
}

void obj_walker_step(Entity* self) {
    const bool up = IsKeyDown(KEY_UP);
    const bool down = IsKeyDown(KEY_DOWN);
    const bool left = IsKeyDown(KEY_LEFT);
    const bool right = IsKeyDown(KEY_RIGHT);

    const int horizontal_direction = right - left;
    const int vertical_direction = down - up;

    const float speed = 1.0f;

    self->position.x += (float) horizontal_direction * speed;
    self->position.y += (float) vertical_direction * speed;
}

void obj_walker_draw(Entity* self) {
    DrawRectangleV(self->position, (Vector2){32, 32}, RED);
}

void obj_gun_create(Entity* self, void* data) {
}

typedef struct {
    Direction direction;
} ObjBulletData;

void obj_gun_step(Entity* self) {
    const bool up = IsKeyPressed(KEY_W);
    const bool left = IsKeyPressed(KEY_A);
    const bool down = IsKeyPressed(KEY_S);
    const bool right = IsKeyPressed(KEY_D);

    const bool shoot = up || left || down || right;
    if (!shoot) return;

    Direction direction;
    if (up) direction = UP;
    else if (left) direction = LEFT;
    else if (right) direction = RIGHT;
    else direction = DOWN;

    ObjBulletData bullet_data = {
        .direction = direction
    };
    entity_create(OBJ_TEST_BULLET, self->position.x, self->position.y, &bullet_data);
}

void obj_gun_draw(Entity* self) {
    DrawRectangleV(self->position, (Vector2){32, 32}, GRAY);
}

void obj_bullet_create(Entity* self, void* data) {
    ObjBulletData* bullet_data = malloc(sizeof(ObjBulletData));

    if (data) {
        memcpy(bullet_data, data, sizeof(ObjBulletData));
    }

    self->data = bullet_data;
}

void obj_bullet_step(Entity* self) {
    ObjBulletData* bullet_data = self->data;

    float angle = bullet_data->direction  * DEG2RAD;
    float speed = 3.0f;
    Vector2 velocity = {
        cosf(angle) * speed,
        -sinf(angle) * speed
    };

    self->position = Vector2Add(self->position, velocity);
}

void obj_bullet_draw(Entity* self) {
    DrawRectangleV(self->position, (Vector2){10, 10}, YELLOW);
}

int main() {
    InitWindow(400, 400, "DEBUG");
    SetTargetFPS(60);

    entity_register(OBJ_TEST_LOGGER, (Entity_Methods){
                        .create = obj_logger_create,
                        .step = obj_logger_step,
                        .draw = obj_logger_draw,
                    });

    entity_register(OBJ_TEST_WALKER, (Entity_Methods){
                        .create = obj_walker_create,
                        .step = obj_walker_step,
                        .draw = obj_walker_draw,
                    });

    entity_register(OBJ_TEST_GUN, (Entity_Methods){
                        .create = obj_gun_create,
                        .step = obj_gun_step,
                        .draw = obj_gun_draw,
                    });

    entity_register(OBJ_TEST_BULLET, (Entity_Methods){
                        .create = obj_bullet_create,
                        .step = obj_bullet_step,
                        .draw = obj_bullet_draw,
                    });


    entity_create(OBJ_TEST_WALKER, 100, 100, nullptr);
    entity_create(OBJ_TEST_LOGGER, 100, 100, nullptr);
    entity_create(OBJ_TEST_GUN, 200, 200, nullptr);

    while (!WindowShouldClose()) {
        entities_step();

        if (IsKeyPressed(KEY_N)) {
            entity_create(OBJ_TEST_WALKER, 100, 100, nullptr);
            //entities[total_entities++] = obj_walker_create((Vector2){100, 100});
        }

        BeginDrawing();
        ClearBackground(BLACK);
        entities_draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
