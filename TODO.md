[lang] `node_return_output` -- handle new single linked wire

[core] `return_task` instead of `maybe_return_task`
[core] `task_t` has two `principal_port_t`

# thread

[thread] wrap `pthread_cond_t` -- maybe `condvar_t`

# core

[queue] `queue_t` should auto resize -- requires locks which is not acceptable

- maybe rename `queue_t` to `fixed_queue_t`

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
