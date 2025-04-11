[net] simplify `wire_print` functions
[debug] remove `debug/` for now

[net] `wire_t` -- be single linked -- only has a pointer to `value_t fuzed`
[lang] `node_take_input` -- handle new single linked wire

[core] extract `connect`
[net] `wire_connect` -- the critical section

[lang] `node_return_output` -- handle new single linked wire

`return_task` instead of `maybe_return_task`

[core] `task_t` has two `principal_port_t`

# net

[net] `wire_t` -- be single list -- only has a pointer to `value_t next`

- `next` might be `wire_t` or `principal_port_t`

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

[debug] use real physics force
[debug] `node_physics_simulate` -- move by `velocity` and clear `force` for every `node_model`
[debug] remove `node_physics_fake_simulate`
