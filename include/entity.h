//
// Created by el_paco on 21/07/26.
//

#pragma once

#include "raylib.h"

typedef enum {
    SPR_TEST,
    SPR_BATTER_RIGHT,
    SPR_BATTER_UP,
    SPR_BATTER_LEFT,
    SPR_BATTER_DOWN,
    SPRITES_TOTAL
} SpriteID;

typedef enum {
    OBJ_NONE,
    OBJ_TEST_WALKER,
    OBJ_TEST_LOGGER,
    OBJ_TEST_GUN,
    OBJ_TEST_BULLET,
    OBJ_BATTER,
    TOTAL_GAME_OBJECTS
} GameObjectID;

typedef struct {
    GameObjectID id;
    Vector2 position;

    SpriteID sprite;
    float image_speed;
    float image_index;
    int image_number;

    void* data;
} Entity;

typedef struct {
    void (*create)(Entity* self, void* raw_data);
    void (*step)(Entity* self);
    void (*draw)(Entity* self);
} Entity_Methods;

void entity_step(Entity* entity);
void entity_draw(Entity* entity);

void entity_register(GameObjectID id, Entity_Methods entity_methods);

void entity_create(GameObjectID id, float x, float y, void* data);
void entity_createV(GameObjectID id, Vector2 position, void* data);

void entity_draw_self(Entity* self);

void entities_step();
void entities_draw();