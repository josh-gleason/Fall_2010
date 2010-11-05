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

;a
(define (n-nodes T)
  (cond 
    ((null? T)
      0)
    (else
      (+
        1
        (n-nodes (left T))
        (n-nodes (right T))))))

;b
(define (n-leaves T)
  (cond 
    ((null? T)
      1)  ; a leaf
    (else
      (+
        (n-nodes (left T))
        (n-nodes (right T))))))

;c
(define (height T)
  (cond
    ((null? T)
      0)
    (else
      (+ 1 (max (height (left T)) (height (right T)))))))

;d
(define (postorder T)
  (cond
    ((null? T)
      T)
    (else  
      (append (postorder (left T)) (postorder (right T)) (list (val T))))))

