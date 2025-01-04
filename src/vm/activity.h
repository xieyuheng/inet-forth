#pragma once

struct activity_t {
    wire_t *wire;
    const rule_t *rule;
};

activity_t *activity_new(wire_t *wire, const rule_t *rule);
