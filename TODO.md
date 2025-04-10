# net

[net] `principal_port_new`
[net] `principal_port_destroy`
[net] `principal_port_t` -- as `value_t`

[net] `wire_t` -- be single list -- only has a pointer to next

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
