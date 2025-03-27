import zero add1 add "nat.fth"

define one zero add1 end
define two one one add end
define three two one add end
define four two two add end

two two add
two two add
add
