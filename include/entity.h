//
// Created by el_paco on 21/07/26.
//

#pragma once

#include "raylib.h"

typedef enum {
    OBJ_BATTER,
    OBJ_TEST,
    TOTAL_GAME_OBJECTS
} GameObjectID;

typedef struct {
    GameObjectID id;
    Vector2 position;
    void* data;
} Entity;

typedef struct {
    void (*step)(Entity* self);
    void (*draw)(Entity* self);
} Entity_Methods;

void entity_step(Entity entity);
void entity_draw(Entity entity);
void entity_hi();