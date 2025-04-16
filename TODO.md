ambr worker_delete_node worker_recycle_node

[core] worker_connect.c
[core] `worker_apply_node` -- fix data race!

[core] `worker_t` -- has `log` file

[core] `step_task` -- should not lock `stdout` when handling `DEBUG_TASK_LOG`

[core] `task_history_t` -- setup
[core] `task_history_t` -- has `event_message_array`
[core] `task_history_new`
[core] `task_history_destroy`
[core] `task_history_record`
[core] `task_history_print`

[core] `task_t` -- track scheduling history on `DEBUG_TASK_LOG`

[core] `task_print` -- call `task_history_print`

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
