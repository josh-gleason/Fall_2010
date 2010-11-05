(define (flatten L)
  (cond 
    ((null? L)
      L)
    ((list? (car L))
      (append (flatten (car L)) (flatten (cdr L))))
    (else
      (cons (car L) (flatten (cdr L))))))

