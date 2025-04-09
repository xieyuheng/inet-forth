# net

[net] `connected_node_iter_t` -- has `node_allocator` and `node`
[net] `connected_node_iter_new`
[net] `connected_node_iter_destroy`

[net] `connected_node_array` -- setup

[net] fix `x_inspect_run` -- fix snapshot tests

[net] `principal_port_t` -- setup
[net] `principal_port_t` -- as value

[net] single list `wire_t`

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
