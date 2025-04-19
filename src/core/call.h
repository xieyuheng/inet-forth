#pragma once

void call(worker_t *worker, const def_t *unknown_def);

void call_primitive(worker_t *worker, const primitive_t *def);
void call_node_ctor(worker_t *worker, const node_ctor_t *ctor);
