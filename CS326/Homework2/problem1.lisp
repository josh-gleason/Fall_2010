(define (subst x y L)
  (cond 
    ((null? L) L)
    ((equal? (car L) x)
      (append (list y) (subst x y (cdr L))))
    (else
      (append (list (car L)) (subst x y (cdr L))))))

(subst 'c 'k '(c o c o n u t))

