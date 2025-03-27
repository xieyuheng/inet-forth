#pragma once

void call(worker_t *worker, const def_t *unknown_def);

void call_primitive(worker_t *worker, const primitive_def_t *def);
void call_function(worker_t *worker, const function_def_t *def);
void call_constant(worker_t *worker, const constant_def_t *def);
void call_node(worker_t *worker, const node_ctor_t *ctor);
