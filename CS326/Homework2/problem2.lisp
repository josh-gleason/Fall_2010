(define (check x L) 
  (cond 
    ((null? L) #t)
    ((equal? x (car L)) #f )
    (else (check x (cdr L)))))

(define (all-different? L)
  (cond
    ((or (null? L) (equal? (length L) 1)) #t)
    (else
      (if (check (car L) (cdr L))
        (all-different? (cdr L))
        #f ))))

(display (all-different? '(1 2 3 3 4)))
(display (all-different? '(1 2 3 4)))
(display (all-different? '(7 2 3 7 4)))
(display (all-different? '(1 2 3 4 5 5)))
(display (all-different? '(1 1 3 4 5 5)))
(display (all-different? '(5 1 3 4 8 5)))

