# thread

[thread] `spinlock_t`

# deque

[deque] `deque_t` -- try `spinlock_t`
[deque] `deque_t` -- use mod queue for real -- not just list + lock

# core

[core] `node_t` -- use `spinlock_t`

# module system

```
import zero add1 add "nat.fth"
```

# example

[example] more exampels from the paper

# primitive value

[design] write `list-map` as example -- use explicit `dup` -- keep linear variable simple

- use `{ ... }` for un-named program -- just program, no closure over linear variables
- another solution is to use quoted symbol to reference defined program

# debug

[debug] use real physics force
[debug] `node_physics_simulate` -- move by `velocity` and clear `force` for every `node_model`
[debug] remove `node_physics_fake_simulate`

# thread

[thread] wrap `pthread_cond_t` -- maybe `condvar_t`
