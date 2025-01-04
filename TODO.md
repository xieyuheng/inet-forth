[net] wire_t as xobject
[net] fix value_print for wire_t

# vm

[vm] mod -- change def_list to def_hash
[vm] mod -- change rule_list to rule_hash

[vm] rename active_wire_list to activity_list
[vm] activity_t -- has wire and rule
[vm] activity_list has activity_t

# curry

use name/n to do explicit curry -- use less wire-pair

# debug

[debug] support dark mode

# main

2008-the-annotated-turing.djvu -- program turing machine in inet
1997-interaction-combinators.pdf -- combinators and lambda
1997-interaction-combinators.pdf -- cellular automata
projects/others/HigherOrderCO/HVM/paper/HVM2.pdf

# later

[example] `examples/lambda.fth` -- use interaction combinators
[example] `examples/turing.fth` -- coding turing machine
[example] polish parsing (from 1990-interaction-nets) as an example -- need primitive string type

# primitive value

[design] node ports can store any value -- not only wire, but also primitive values

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

# parallelism

learn linux thread

shared memory parallel execution of inet
