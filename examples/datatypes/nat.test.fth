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

define one zero add1 end
define two one one add end
define three two one add end
define four two two add end

two two add
two two add
add

wire-print-net
run
wire-print-net

// to define `mul`, we first need `nat-erase` and `nat-dup`

define-node nat-erase target! -- end

define-rule zero nat-erase end

define-rule add1 nat-erase
  ( prev )
  prev nat-erase
end

define-node nat-dup target! -- first second end

define-rule zero nat-dup
  ( first second )
  zero first connect
  zero second connect
end

define-rule add1 nat-dup
  ( first second ) ( prev )
  prev nat-dup
  ( prev-first prev-second )
  prev-second add1 second connect
  prev-first add1 first connect
end

define-node mul target! mulend -- result end

define-rule zero mul
  ( mulend result )
  mulend nat-erase
  zero result connect
end

define-rule add1 mul
  ( mulend result ) ( prev )
  mulend nat-dup
  ( mulend-first mulend-second )
  prev mulend-second swap mul
  mulend-first add result connect
end

two two mul

wire-print-net
run
wire-print-net

// to define `nat-max`, we need `nat-max-add1`

define-node nat-max first! second -- result end
define-node nat-max-add1 first second! -- result end

define-rule zero nat-max
  ( second result )
  second result connect
end

define-rule add1 nat-max
  ( second result ) ( prev )
  prev second nat-max-add1 result connect
end

define-rule zero nat-max-add1
  ( first result )
  first add1 result connect
end

define-rule add1 nat-max-add1
  ( first result ) ( prev )
  first prev nat-max
  add1 result connect
end

one two nat-max

wire-print-net
run
wire-print-net
