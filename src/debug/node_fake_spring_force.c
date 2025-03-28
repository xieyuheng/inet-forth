#include "index.h"

static vec2_t
spring_force(vec2_t first, vec2_t second) {
    vec2_t delta = vec2_sub(second, first);
    double distance = vec2_length(delta);

    double C = 0.03;
    double spring_length = 5;

    double factor = log(fabs(distance / spring_length));
    vec2_t force = vec2_mul_scalar(delta, C * factor);;

    if (vec2_is_nan(force))
        return vec2_zeros();

    return force;
}

void
node_fake_spring_force(node_physics_t *self, hash_t *node_hash, hash_t *node_model_hash) {
    (void) self;

    node_t *node = hash_first(node_hash);
    while (node) {
        for (size_t i = 0; i < node->ctor->arity; i++) {
            value_t *value = node_get(node, i);
            if (!is_wire(value)) continue;

            wire_t *wire = as_wire(value);
            wire_t *opposite = wire_opposite(wire);
            if (wire &&
                wire->node &&
                opposite &&
                opposite->node)
            {
                node_model_t *node_model1 = hash_get(node_model_hash, (void *) wire->node->id);
                node_model_t *node_model2 = hash_get(node_model_hash, (void *) opposite->node->id);

                vec2_t force = spring_force(node_model1->position, node_model2->position);

                node_model1->force.x += force.x / 2;
                node_model1->force.y += force.y / 2;

                node_model2->force.x -= force.x / 2;
                node_model2->force.y -= force.y / 2;
            }
        }

        node = hash_next(node_hash);
    }

;
}
