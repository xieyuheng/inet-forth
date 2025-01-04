define-node t -- value! end
define-node f -- value! end

define-node bool-erase target! -- end

define-rule t bool-erase end
define-rule f bool-erase end

define-node conj x! y -- z end
define-node conj-t y! -- z end

define-rule t conj ( y z ) y conj-t z connect end
define-rule f conj ( y z ) y bool-erase f z connect end

define-rule t conj-t ( z ) z t connect end
define-rule f conj-t ( z ) z f connect end

define-node disj x! y -- z end
define-node disj-f y! -- z end

define-rule t disj ( y z ) y bool-erase t z connect end
define-rule f disj ( y z ) y disj-f z connect end

define-rule t disj-f ( z ) t z connect end
define-rule f disj-f ( z ) f z connect end

t t conj
f t conj conj
t t disj
f t disj disj
disj

wire-print-net
run
wire-print-net

// debug
