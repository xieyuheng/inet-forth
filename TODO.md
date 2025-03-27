[docs] improve readme
[docs] improve articles

# core

[core] remove `log_level` -- use `flags.c` instead
[core] `worker_t` -- has `task_queue` instead of `task_list`
[core] `worker_t` -- safe use of `task_queue` -- which might be full
[core] quit using `mod->rule_list` -- save rule in `node_ctor_t`

# primitive value

[design] node ports can store any value -- not only wire, but also primitive values
[design] the most important primitive value is function
[design] write `list-map` as example
- use explicit `copy` -- keep linear variable like simple
- use `{ ... }` for un-named program -- just program, no closure over linear variables
[question] is it possible to have a generic `copy` that can copy any net with a root wire?

# debug

[debug] 尝试使用真实的物理学来实现 force
[debug] `node_physics_simulate` -- move by `velocity` and clear `force` for every `node_model`
[debug] remove `node_physics_fake_simulate`

# module system

支持简单的 import name，同时支持修改 name 以避免冲突
