#include <stdio.h>

#include "raylib.h"
#include "entity.h"

Entity obj_logger_create(Vector2 position) {
    return (Entity) {
        .id = OBJ_TEST_LOGGER,
        .position = position,
    };
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

Entity obj_walker_create(Vector2 position) {
    return (Entity) {
        .id = OBJ_TEST_WALKER,
        .position = position,
    };
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

Entity entities[1024] = {0};
int total_entities = 0;

int main() {
    InitWindow(400, 400, "DEBUG");
    SetTargetFPS(60);

    entity_register(OBJ_TEST_LOGGER, (Entity_Methods){
                        .create = nullptr,
                        .step = obj_logger_step,
                        .draw = obj_logger_draw,
                    });

    entity_register(OBJ_TEST_WALKER, (Entity_Methods){
                        .create = nullptr,
                        .step = obj_walker_step,
                        .draw = obj_walker_draw,
                    });


    entities[total_entities++] = obj_walker_create((Vector2){100, 100});
    entities[total_entities++] = obj_logger_create((Vector2){32, 32});

    while (!WindowShouldClose()) {
        for (int i = 0; i < total_entities; i++) {
            entity_step(&entities[i]);
        }

        if (IsKeyPressed(KEY_N)) {
            entities[total_entities++] = obj_walker_create((Vector2){100, 100});
        }

        BeginDrawing();
        ClearBackground(BLACK);
        for (int i = 0; i < total_entities; i++) {
            entity_draw(&entities[i]);
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
