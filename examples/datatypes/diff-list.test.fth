define-node diff front -- back value! end
define-node diff-append target! rest -- result end
define-node diff-open new-back target! -- old-back end

// plug the front of the `target` to the back of `rest`.

define-rule diff diff-append
  ( rest result ) ( front back )
  front diff result connect
  rest diff-open back connect
end

define-rule diff diff-open
  ( new-back old-back ) ( front back )
  back new-back connect
  front old-back connect
end

// import null cons append "list.fth"

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

// test

define-node sole -- value! end

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