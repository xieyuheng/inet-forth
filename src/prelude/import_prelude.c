#include "index.h"

void
import_prelude(mod_t *mod) {
    // bool

    define_constant(mod, "false", xfalse);
    define_constant(mod, "true", xtrue);
    define_primitive_fn_1(mod, "not", xbool_not);
    define_primitive_fn_2(mod, "and", xbool_and);
    define_primitive_fn_2(mod, "or", xbool_or);

    // value

    define_primitive_fn_2(mod, "eq", x_eq);
    define_primitive_fn(mod, "dup", x_dup);
    define_primitive_fn(mod, "swap", x_swap);
    define_primitive_fn(mod, "drop", x_drop);
    define_primitive_fn(mod, "rot", x_rot);
    define_primitive_fn(mod, "over", x_over);
    define_primitive_fn(mod, "tuck", x_tuck);

    // testing

    define_primitive_fn(mod, "ok", x_ok);

    // int

    define_primitive_fn_2(mod, "add", xint_add);
    define_primitive_fn_2(mod, "sub", xint_sub);
    define_primitive_fn_2(mod, "mul", xint_mul);
    define_primitive_fn_2(mod, "div", xint_div);
    define_primitive_fn_2(mod, "mod", xint_mod);
    define_primitive_fn_1(mod, "int-to-float", xint_to_xfloat);

    // float

    define_primitive_fn_2(mod, "fadd", xfloat_add);
    define_primitive_fn_2(mod, "fsub", xfloat_sub);
    define_primitive_fn_2(mod, "fmul", xfloat_mul);
    define_primitive_fn_2(mod, "fdiv", xfloat_div);
    define_primitive_fn_2(mod, "fmod", xfloat_mod);
    define_primitive_fn_1(mod, "float-to-int", xfloat_to_xint);

    // net

    define_primitive_fn(mod, "connect", x_connect);
    define_primitive_fn(mod, "wire-print-net", x_wire_print_net);
    define_primitive_fn(mod, "link", x_link);
    define_primitive_fn(mod, "run", x_run);
    define_primitive_fn(mod, "wire-debug", x_wire_debug);
    define_primitive_fn(mod, "debug", x_debug);
    define_primitive_fn(mod, "define-node", x_define_node);
    define_primitive_fn(mod, "define-rule", x_define_rule);

    // console

    define_primitive_fn_1(mod, "print", x_print);
    define_primitive_fn_0(mod, "newline", x_newline);
    define_primitive_fn(mod, "print-worker", x_print_worker);
    define_primitive_fn(mod, "print-value-stack", x_print_value_stack);
    define_primitive_fn(mod, "print-return-stack", x_print_return_stack);

    // constant

    define_primitive_fn(mod, "define-constant", x_define_constant);

    // function

    define_primitive_fn(mod, "define", x_define_function);
}
