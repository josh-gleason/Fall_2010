; Assignment #2
; Author: Joshua Gleason
; Class : CS326
; Date  : 9/21/2010

(define T '(13 (5 (1 () ()) (8 () (9 () ()))) (22 (17 () ()) (25 () ()))))

;Question 1
(define (subst x y L)
  (cond 
    ((null? L) L)
    ((equal? (car L) x)
      (append (list y) (subst x y (cdr L))))
    (else
      (append (list (car L)) (subst x y (cdr L))))))

;Question 2
(define (exists? x L) ; check if x is an element of L
  (cond 
    ((null? L) #f)
    ((equal? x (car L)) #t )
    (else (exists? x (cdr L)))))

(define (all-different? L)
  (cond
    ((or (null? L) (equal? (length L) 1)) #t)
    (else
      (if (exists? (car L) (cdr L))
        #f
        (all-different? (cdr L))))))

;Tree node functions for #3 and the extra credit
(define (val T) (car T))
(define (left T) (car (cdr T)))
(define (right T) (car (cdr (cdr T))))

;Question 3
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
      0)  ; not a leaf
    ((and (null? (left T)) (null? (right T)))
      1)  ; this is a leaf
    (else
      (+
        (n-leaves (left T))
        (n-leaves (right T))))))

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

;Question 4
(define (flatten L)
  (cond 
    ((null? L)
      L)
    ((list? (car L))
      (append (flatten (car L)) (flatten (cdr L))))
    (else
      (cons (car L) (flatten (cdr L))))))

;Question 5 (Extra Credit)
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

