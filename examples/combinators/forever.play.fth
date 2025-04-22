import C D E "CDE.fth"

define E-value link E end

define forever
  link ( car car-op )
  car E-value C
  D ( first second )
  first E
  car-op second connect
end

forever
