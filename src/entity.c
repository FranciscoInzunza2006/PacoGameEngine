//
// Created by el_paco on 21/07/26.
//

#include "entity.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

static bool has_vtable(GameObjectID id);
static bool is_valid_entity(const Entity* entity);

void entity_cleanup(Entity* entity);
void entity_default_cleanup(Entity* entity);

Entity_Methods entity_vtable[TOTAL_GAME_OBJECTS] = {0};

Entity entities[1024] = {0};
int total_entities = 0;

void entity_register(const GameObjectID id, const Entity_Methods entity_methods) {
    if (has_vtable(id)) printf("WARNING: VTable redefinition of %d", id);

    entity_vtable[id] = entity_methods;
}

void entity_create(const GameObjectID id, void* data, const float x, const float y) {
    entity_createV(id, data, (Vector2){x, y});
}

void entity_createV(const GameObjectID id, void* data, const Vector2 position) {
    Entity* new_entity = &entities[total_entities];
    *new_entity = (Entity){
        .id = id,
        .instance_id = total_entities,
        .position = position,
    };
    total_entities++;

    if (entity_vtable[id].create)
        entity_vtable[id].create(new_entity, data);
}

void entity_step(Entity* entity) {
    if (!is_valid_entity(entity)) return;
    if (!entity_vtable[entity->id].step) return;

    entity_vtable[entity->id].step(entity);
}

void entity_draw(Entity* entity) {
    if (!is_valid_entity(entity)) return;
    if (!entity_vtable[entity->id].draw) return;

    entity_vtable[entity->id].draw(entity);
}

void entity_destroy(Entity* entity) {
    if (!is_valid_entity(entity)) return;

    if (entity_vtable[entity->id].destroy)
        entity_vtable[entity->id].destroy(entity);

    entity_cleanup(entity);

    entity->id = -1;
    entity->instance_id = -1;
}

void entity_cleanup(Entity* entity) {
    if (!is_valid_entity(entity)) return;

    if (entity_vtable[entity->id].cleanup) {
        entity_vtable[entity->id].cleanup(entity);
    } else {
        entity_default_cleanup(entity);
    }

    //entity_vtable[entity->id].cleanup(entity);
}

void entity_step_all() {
    for (int i = 0; i < total_entities; i++) {
        entity_step(&entities[i]);
    }
}

void entity_draw_all() {
    for (int i = 0; i < total_entities; i++) {
        entity_draw(&entities[i]);
    }
}

void entity_default_cleanup(Entity* entity) {
    free(entity->data);
    entity->data = nullptr;
}

static bool has_vtable(const GameObjectID id) {
    const Entity_Methods* vtable = &entity_vtable[id];

    return vtable->create != nullptr ||
           vtable->step != nullptr ||
           vtable->draw != nullptr ||
           vtable->destroy != nullptr ||
           vtable->cleanup != nullptr;
}

static bool is_valid_entity(const Entity* entity) {
    return entity->id >= 0 && entity->instance_id >= 0;
}