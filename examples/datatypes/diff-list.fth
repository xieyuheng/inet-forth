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
