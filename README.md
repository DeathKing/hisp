# HISP

Hisp is interpreter implemented by C, which meanwhile is also a simple copy of MIT-Scheme. The very idea to make such a implemntation is to interpreter the code I worte in Scheme(especially mit-scheme) of [HIT Data Sturcture course](https://github.com/DeathKing/Hit-DataStructure-On-Scheme). But lexically, Hisp also can be considered as:

+ Hit mIt-Scheme inPlementation
+ Hit Iterator Symbolic Processor
+ Hisp Interpreter for Scheme Program

# Standard

Our goal is to make Hisp totally follow the Revised^5 Report on the Algorithmic Language Scheme(aka. R^5RS), but Hisp only suit a subset of that standard. We also try to make Hisp more powerful by implement Scheme Requests for Implementation(aka. SRFI). Some modern programming language features such as *default argument* will be added also.

# Features

## Default parameters

> **This feature hasn't finished yet.**

Parameters with default value can be easily dealed with in Hisp by introduce `[]`, here is a sample:

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

# Contributing

Take it easy, guys, just `(and (fork) (send 'pull-request))`.

# License

MIT License and our lovely HIT License.

