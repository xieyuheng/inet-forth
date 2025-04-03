remove `root` from `debug_t`
remove `node_iter`
remove `wire_iter`

pattern note about breaking double links
pattern note about pointer graph -- concrete dependancy graph

parallel memory allocator

[net] node allocator

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
