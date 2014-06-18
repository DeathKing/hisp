# HISP

Hisp is a simple copy of MIT-Scheme, implemented by C. The very idea to make such a implemnt is to interpreter the code I worte in Scheme(especially mit-scheme) of HIT Data Sturcture course. But, lexically Hisp also can be considered as:

+ Hit mIt-Scheme inPlementation
+ Hit Iterator Symbolic Processor
+ Hisp Interpreter for Scheme Program

# Standard

Our goal is totally obey the R^5RS, but Hisp only suit a subset of that standard. We also try to make Hisp more powerful by achieve SRIF. Some modarn language feature such as *default argument* will be added also.


# Features

Paramter with default value can be easily deal with in Hisp by introduce `[]`, here is a sample:

```scheme
;; in mit-scheme
(define (plus a #!default b)
  (if (eq? b #!default)
    (+ a 1)
    (+ a b)))

;; in hisp
(define (plus a [b 1])
  (+ a b))

;; both will result as:
(plus 5)   ;---> 6
(plus 5 2) ;---> 7 

```

(define (a (b 1))
  (something))

# License

MIT License and our lovely HIT License.

