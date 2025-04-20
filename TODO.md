[core] rename call to `worker_call`
[core] can we implement call by apply?

# module system

```
import zero add1 add "nat.fth"
```

# example

[example] more exampels from the paper

# deque

[deque] `deque_t` -- use mod queue for real -- not just list + lock

# core

[core] `node_t` -- use `spinlock_t`

# primitive value

[design] write `list-map` as example -- use explicit `dup` -- keep linear variable simple

- use `{ ... }` for un-named program -- just program, no closure over linear variables
- another solution is to use quoted symbol to reference defined program

# player

[player] bring back `player_t`
[player] use real physics force
[player] `node_physics_simulate` -- move by `velocity` and clear `force` for every `node_model`
[player] remove `node_physics_fake_simulate`

# thread

[thread] wrap `pthread_cond_t` -- maybe `condvar_t`
