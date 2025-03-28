import diff diff-append "diff-list.fth"
import null cons "list.fth"
import sole "trivial.fth"

define sole-diff-list
  link ( front front-op )
  front diff ( back value )
  back sole cons sole cons
  front-op connect
  value
end

sole-diff-list
sole-diff-list
diff-append

inspect-run