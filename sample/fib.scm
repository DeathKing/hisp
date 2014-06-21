;;; HISP Sample file - Fibonacci Sequence
;;;
;;; Author: Dinesta Xy. Fisper
;;; License: MIT
;;;
;;; This is a classic example of recursive function call,
;;; considered following sequence:
;;;
;;;         / 1                 (n = 1 or n = 2)
;;; f_n   = |
;;;         \ f_{n-1} + f_{n-2} (n >= 3)
;;;
;;; This file shows how to compute f_n by define a procedure fib.

(define (fib n)
	(if (or (= x 1) (= x 2))
		1
		(+ (fib (- x 1)) (fib (- x 2)))))

(display (fib 6))
