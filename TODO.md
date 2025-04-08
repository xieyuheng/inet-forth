# net

[net] `node_t` should have `value_array` instead of `ports` -- so that `node_init` does not depend on `ctor->arity`
[net] extract `node_init` -- for init `node_heap`
[net] `node_allocator_t` -- init `node_heap` by `size`
[net] `node_allocator_t` -- has the `per_thread_stack_array`
[net] `node_allocator_t` -- replace `node_new`
[net] remove `node_id_count` from worker -- prepare id in `node_allocator_t`

[net] `principal_port_t` -- setup
[net] `principal_port_t` -- as value

[net] single list `wire_t`
[net] fix `x_inspect_run` -- fix snapshot tests

# thread

[thread] wrap `pthread_cond_t` -- maybe `condvar_t`

# core

[core] `worker_t` -- safe use of `task_queue` -- which might be full

# module system

```
import zero add1 add "nat.fth"
```

# primitive value

[design] write `list-map` as example -- use explicit `dup` -- keep linear variable simple

- use `{ ... }` for un-named program -- just program, no closure over linear variables
- another solution is to use quoted symbol to reference defined program

# debug

[debug] 尝试使用真实的物理学来实现 force
[debug] `node_physics_simulate` -- move by `velocity` and clear `force` for every `node_model`
[debug] remove `node_physics_fake_simulate`
