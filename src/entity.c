//
// Created by el_paco on 21/07/26.
//

#include "entity.h"
#include "raylib.h"
#include <stdio.h>

Entity_Methods entity_vtable[TOTAL_GAME_OBJECTS] = {0};
//Entity entities[1024] = {0};
//int i = 0;

void entity_step(Entity* entity)  {
    entity_vtable[entity->id].step(entity);
}

void entity_draw(Entity* entity) {
    entity_vtable[entity->id].draw(entity);
}

void entity_register(const GameObjectID id, const Entity_Methods entity_methods) {
    entity_vtable[id] = entity_methods;
}

void entity_create(GameObjectID id, float x, float y) {
    entity_createV(id, (Vector2){x, y});
}

void entity_createV(GameObjectID id, Vector2 position) {
    entity_vtable[id].create(position, nullptr);
}