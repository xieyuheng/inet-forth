#include "index.h"

void
import_prelude(mod_t *mod) {
    // bool

    define(mod, "false", xfalse);
    define(mod, "true", xtrue);
    define_primitive_fn_1(mod, "not", xbool_not);
    define_primitive_fn_2(mod, "and", xbool_and);
    define_primitive_fn_2(mod, "or", xbool_or);

    // value

    define_primitive_fn_2(mod, "eq", x_eq);
    define_primitive_fn(mod, "dup", 1, 2, x_dup);
    define_primitive_fn(mod, "swap", 2, 2, x_swap);
    define_primitive_fn(mod, "drop", 1, 0, x_drop);
    define_primitive_fn(mod, "rot", 3, 3, x_rot);
    define_primitive_fn(mod, "over", 3, 3, x_over);
    define_primitive_fn(mod, "tuck", 3, 3, x_tuck);

    // testing

    define_primitive_fn(mod, "ok", 1, 0, x_ok);

    // int

    define_primitive_fn_2(mod, "iadd", xint_add);
    define_primitive_fn_2(mod, "isub", xint_sub);
    define_primitive_fn_2(mod, "imul", xint_mul);
    define_primitive_fn_2(mod, "idiv", xint_div);
    define_primitive_fn_2(mod, "imod", xint_mod);
    define_primitive_fn_1(mod, "int-to-float", xint_to_xfloat);

    // float

    define_primitive_fn_2(mod, "fadd", xfloat_add);
    define_primitive_fn_2(mod, "fsub", xfloat_sub);
    define_primitive_fn_2(mod, "fmul", xfloat_mul);
    define_primitive_fn_2(mod, "fdiv", xfloat_div);
    define_primitive_fn_2(mod, "fmod", xfloat_mod);
    define_primitive_fn_1(mod, "float-to-int", xfloat_to_xint);

    // net

    define_primitive_fn(mod, "connect", 2, 0, x_connect);
    define_primitive_fn(mod, "link", 0, 2, x_link);
    define_primitive_fn(mod, "run", 0, 0, x_run);
    define_primitive_fn(mod, "inspect-run", 0, 0, x_inspect_run);
    define_primitive_fn(mod, "define-node", 0, 0, x_define_node);
    define_primitive_fn(mod, "define-rule", 0, 0, x_define_rule);

    // console

    define_primitive_fn_1(mod, "print", x_print);
    define_primitive_fn_0(mod, "newline", x_newline);
    define_primitive_fn(mod, "print-worker", 0, 0, x_print_worker);
    define_primitive_fn(mod, "print-value-stack", 0, 0, x_print_value_stack);
    define_primitive_fn(mod, "print-return-stack", 0, 0, x_print_return_stack);

    // function

    define_primitive_fn(mod, "define", 0, 0, x_define_function);
    define_primitive_fn(mod, "import", 0, 0, x_import);
}
