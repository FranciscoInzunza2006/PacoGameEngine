#include <stdio.h>

#include "raylib.h"
#include "entity.h"
#include "raymath.h"

typedef enum {
    LEFT = 0,
    UP = 90,
    RIGHT = 180,
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

void obj_gun_step(Entity* self) {
    const bool up = IsKeyDown(KEY_W);
    const bool left = IsKeyDown(KEY_A);
    const bool down = IsKeyDown(KEY_S);
    const bool right = IsKeyDown(KEY_D);

    const bool shoot = up || left || down || right;
    if (!shoot) return;

    entity_create(OBJ_TEST_BULLET, self->position.x, self->position.y, nullptr);
}

void obj_gun_draw(Entity* self) {
    DrawRectangleV(self->position, (Vector2){32, 32}, GRAY);
}

void obj_bullet_create(Entity* self, void* data) {
}

void obj_bullet_step(Entity* self) {

}

void obj_bullet_draw(Entity* self) {
    DrawRectangleV(self->position, (Vector2){10, 10}, YELLOW);
}

typedef struct {

} ObjBulletData;

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
