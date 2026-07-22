//
// Created by el_paco on 21/07/26.
//

#include "entity.h"
#include "raylib.h"
#include <stdio.h>

Entity_Methods entity_vtable[TOTAL_GAME_OBJECTS] = {0};

Entity entities[1024] = {0};
int total_entities = 0;

void entity_step(Entity* entity)  {
    entity_vtable[entity->id].step(entity);
}

void entity_draw(Entity* entity) {
    entity_vtable[entity->id].draw(entity);
}

void entity_register(const GameObjectID id, const Entity_Methods entity_methods) {
    entity_vtable[id] = entity_methods;
}

void entity_create(GameObjectID id, float x, float y, void* data) {
    entity_createV(id, (Vector2){x, y}, data);
}

void entity_createV(GameObjectID id, Vector2 position, void* data) {
    Entity* new_entity = &entities[total_entities++];

    *new_entity = (Entity){
        .id = id,
        .position = position,
    };

    entity_vtable[id].create(new_entity, data);
}

void entities_step() {
    for (int i = 0; i < total_entities; i++) {
        entity_step(&entities[i]);
    }
}
void entities_draw() {
    for (int i = 0; i < total_entities; i++) {
        entity_draw(&entities[i]);
    }
}