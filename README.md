# inet-forth

Moore's Law is broken.

![Figure 6.11: MIPS/Clock-Frequency Trend for Intel CPUs](assets/images/mips-clock-frequency-trend-for-intel-cpus.png)

(Figure 6.11: MIPS/Clock-Frequency Trend for Intel CPUs, from ["Is Parallel Programming Hard, And, If So, What Can You Do About It?"](https://www.kernel.org/pub/linux/kernel/people/paulmck/perfbook/perfbook.html), by Paul E. McKenney)

The exponential increase in single-threaded performance halted in about 2003.
Therefore, increasing performance will increasingly require parallelism.

How do we program a machine with many cores?

One possible solution is the graph-based computation model
-- [interaction nets](https://en.wikipedia.org/wiki/Interaction_nets) --
where a program in this computation model can automatically
make use of any number cores in the machine.

The project -- **inet-forth** -- is an implementation of interaction nets
as a [forth-like language](https://en.wikipedia.org/wiki/forth_(programming_language)).

## Syntax

```xml
define-node <name> <input-ports> -- <output-ports> end
define-rule <name> <name> <function-body> end
define <name> <function-body> end
```

## Examples

### Natural Number

Define three nodes `(zero)`, `(add1)` and `(add)`:

```
define-node zero -- value! end
define-node add1 prev -- value! end
define-node add target! addend -- result end
```

```
value!   value!        value
  |        |             |
(zero)   (add1)        (add)
           |           /   \
          prev    target!  addend
```

The rule between `(add1)` and `(add)`:

```
define-rule add1 add
  ( addend result ) ( prev )
  prev addend add
  add1 result connect
end
```

```
     value             value            value
       |                 |                |
     (add)     =>                =>     (add1)
     /   \                 \              |
(add1)   addend            addend       (add)
   |                 |                  /   \
 prev              prev              prev   addend
```

To apply this rule is to disconnect and delete `(add1)` and `(add)` and reconnect newly exposed wires:

- `( addend result )` save the wires that were connected to `(add)` to local variable `addend` and `result`.
- `( prev )` save the wire that was connected to `(add1)` to local variable `prev`.
- `prev` push local variable to the stack.
- `addend` push local variable to the stack.
- `add` take two arguments from the stack and create a new `(add)` node.
- `add1` take one argument from the stack and create a new `(add1)` node.
- `result` push local variable to the stack.
- `connect` take two wires from the stack and connect them.

The rule between `(zero)` and `(add)`:

```
define-rule zero add
  ( addend result )
  addend result connect
end
```

```
     value          value         value
       |              |             |
     (add)     =>             =>   |
     /   \              \            \
(zero)   addend        addend       addend
```

To apply this rule is to disconnect and delete `(zero)` and `(add)` and reconnect newly exposed wires:

- `( addend result )` save the wires that were connected to `(add)` to local variable `addend` and `result`.
- `addend` push local variable to the stack.
- `result` push local variable to the stack.
- `connect` take two wires from the stack and connect them.

Example interaction:

```
       |                   |                   |              |
     (add)               (add1)              (add1)         (add1)
     /   \                 |                   |              |
(add1)    (add1)         (add)               (add1)         (add1)
   |        |    =>      /   \       =>        |        =>    |
(add1)    (add1)    (add1)    (add1)         (add)          (add1)
   |        |          |        |            /   \            |
(zero)    (zero)    (zero)    (add1)    (zero)   (add1)     (add1)
                                |                  |          |
                              (zero)             (add1)     (zero)
                                                   |
                                                 (zero)
```

The whole program with test:

```
define-node zero -- value! end
define-node add1 prev -- value! end
define-node add target! addend -- result end

define-rule zero add
  ( addend result )
  addend result connect
end

define-rule add1 add
  ( addend result ) ( prev )
  prev addend add
  add1 result connect
end

define two
  zero add1 add1
end

two two add

wire-print-net
run
wire-print-net
```

<details>
<summary>output</summary>

```xml
<net>
<root>
(add₇)-result-<>-
</root>
<body>
(add1₃)-value!-<>-!target-(add₇)
(add1₆)-value!-<>-addend-(add₇)
(add1₅)-value!-<>-prev-(add1₆)
(zero₄)-value!-<>-prev-(add1₅)
(add1₂)-value!-<>-prev-(add1₃)
(zero₁)-value!-<>-prev-(add1₂)
</body>
</net>

<net>
<root>
(add1₉)-value!-<>-
</root>
<body>
(add1₁₁)-value!-<>-prev-(add1₉)
(add1₆)-value!-<>-prev-(add1₁₁)
(add1₅)-value!-<>-prev-(add1₆)
(zero₄)-value!-<>-prev-(add1₅)
</body>
</net>
```

</details>

### List

```
define-node null -- value! end
define-node cons tail head -- value! end
define-node append target! rest -- result end

define-rule null append
  ( rest result )
  rest result connect
end

define-rule cons append
  ( rest result ) ( tail head )
  tail rest append
  head cons result connect
end

define-node sole -- value! end

null sole cons sole cons sole cons
null sole cons sole cons sole cons
append

wire-print-net
run
wire-print-net
```

<details>
<summary>output</summary>

```xml
<net>
<root>
(append₁₅)-result-<>-
</root>
<body>
(cons₇)-value!-<>-!target-(append₁₅)
(cons₁₄)-value!-<>-rest-(append₁₅)
(cons₁₂)-value!-<>-tail-(cons₁₄)
(sole₁₃)-value!-<>-head-(cons₁₄)
(cons₁₀)-value!-<>-tail-(cons₁₂)
(sole₁₁)-value!-<>-head-(cons₁₂)
(null₈)-value!-<>-tail-(cons₁₀)
(sole₉)-value!-<>-head-(cons₁₀)
(cons₅)-value!-<>-tail-(cons₇)
(sole₆)-value!-<>-head-(cons₇)
(cons₃)-value!-<>-tail-(cons₅)
(sole₄)-value!-<>-head-(cons₅)
(null₁)-value!-<>-tail-(cons₃)
(sole₂)-value!-<>-head-(cons₃)
</body>
</net>

<net>
<root>
(cons₁₇)-value!-<>-
</root>
<body>
(cons₁₉)-value!-<>-tail-(cons₁₇)
(sole₆)-value!-<>-head-(cons₁₇)
(cons₂₁)-value!-<>-tail-(cons₁₉)
(sole₄)-value!-<>-head-(cons₁₉)
(cons₁₄)-value!-<>-tail-(cons₂₁)
(sole₂)-value!-<>-head-(cons₂₁)
(cons₁₂)-value!-<>-tail-(cons₁₄)
(sole₁₃)-value!-<>-head-(cons₁₄)
(cons₁₀)-value!-<>-tail-(cons₁₂)
(sole₁₁)-value!-<>-head-(cons₁₂)
(null₈)-value!-<>-tail-(cons₁₀)
(sole₉)-value!-<>-head-(cons₁₀)
</body>
</net>
```

</details>

### More

For more examples, please see the [examples/](examples/) directory.

## Docs

- [Programming with Interaction Nets](docs/articles/programming-with-interaction-nets.md)
- [反应网编程](docs/articles/反应网编程.md)

## Community

- [Discord / concatenative #inet](https://discord.gg/EcUfwRkbdx)

## Install

Dependencies:

- debian/ubuntu: `sudo apt install libx11-dev`

Compile:

```
git clone https://github.com/cicada-lang/inet-forth
cd inet-forth
make -j
make test
```

The compiled binary `./bin/inet-forth` is the command-line program.

```sh
$ ./bin/inet-forth
inet-forth 0.1.0

commands:
  run -- run files
  self-test -- run self test
  version -- print version
  help -- print help message
```

For examples:

```sh
./bin/inet-forth run examples/readme/nat.fth
```

## Development

```shell
gmake -j       # compile src/ files to lib/ and bin/
gmake run      # compile and run the command-line program
gmake test     # compile and run test
gmake clean    # clean up compiled files
```

Using [tsan (ThreadSanitizer)](https://github.com/google/sanitizers/wiki/threadsanitizercppmanual)
to test data race in parallel program:

```shell
LDFLAGS=-fsanitize=thread CFLAGS=-fsanitize=thread gmake -j
gmake self-test
```

## Implementations

- [inet-cute](https://github.com/cicada-lang/inet-cute)
- [inet-js](https://github.com/cicada-lang/inet-js)
- [inet-forth](https://github.com/cicada-lang/inet-forth)
- [inet-lisp](https://github.com/cicada-lang/inet-lisp)

## References

**Papers**:

- [1990-interaction-nets](./docs/references/1990-interaction-nets.pdf)
- [1997-interaction-combinators](./docs/references/1997-interaction-combinators.pdf)

**Books**:

- [scalable c](https://github.com/booksbyus/scalable-c)

**Languages**:

- [forth](https://en.wikipedia.org/wiki/Forth_(programming_language))

## Contributions

To make a contribution, fork this project and create a pull request.

Please read the [STYLE-GUIDE.md](STYLE-GUIDE.md) before you change the code.

Remember to add yourself to [AUTHORS](AUTHORS).
Your line belongs to you, you can write a little
introduction to yourself but not too long.

## License

[GPLv3](LICENSE)
