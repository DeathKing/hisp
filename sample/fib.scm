;;; HISP Sample file - 
;;;
;;; Author: Dinesta Xy. Fisper
;;; License: MIT


(define fib
    (lambda (x)
		(if (or (= x 0) (= x 1)) 
            1
			(+ (fib (- x 1)) (fib (- x 2))))))

(display (fib 6))
