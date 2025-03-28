define-node t -- value! end
define-node f -- value! end

define-node bool-erase target! -- end

define-rule t bool-erase end
define-rule f bool-erase end

define-node conj x! y -- z end
define-node conj-t y! -- z end

define-rule t conj swap conj-t connect end
define-rule f conj f connect bool-erase end

define-rule t conj-t t connect end
define-rule f conj-t f connect end

define-node disj x! y -- z end
define-node disj-f y! -- z end

define-rule t disj t connect bool-erase end
define-rule f disj swap disj-f connect end

define-rule t disj-f t connect end
define-rule f disj-f f connect end

t t conj
f t conj conj
t t disj
f t disj disj
disj

inspect-run