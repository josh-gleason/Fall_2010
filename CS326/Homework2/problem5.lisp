(define T
  '(13
    (5
      (1 () ())
      (8 ()
        (9 () ())))
    (22
      (17 () ())
      (25 () ()))))

(define (val T) (car T))
(define (left T) (car (cdr T)))
(define (right T) (car (cdr (cdr T))))

(define (member-bst? V T)
  (cond
    ((null? T) 
      #f)   ; never reached V
    ((eq? V (val T))
      #t)
    ((< V (val T))
      (member-bst? V (left T)))
    (else
      (member-bst? V (right T)))))
      
