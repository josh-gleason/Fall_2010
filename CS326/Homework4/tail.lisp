(define (sum L)
  (define (sum-helper L a)
    (if (null? L)
      a
      (sum-helper (cdr L) (+ a (car L)))
    ))
  (sum-helper L 0))

