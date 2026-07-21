//
// Created by el_paco on 21/07/26.
//

#include "entity.h"

#include <stdio.h>

Entity_Methods entity_vtable[TOTAL_GAME_OBJECTS] = {0};

void entity_step(Entity entity)  {
    entity_vtable[entity.id].step(&entity);
}

void entity_draw(Entity entity) {
    entity_vtable[entity.id].draw(&entity);
}

void entity_hi() {
    printf("Hello, World!\n");
}