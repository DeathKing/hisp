![Hisp Bear](https://raw.githubusercontent.com/DeathKing/Hisp/dev/src/res/hisp2.png)

> **WARNING!**  
> This is a refactoring version of my final project of Software Development course, and it can not work until a β-version released.

Hisp is a Scheme interpreter implemented by C, which meanwhile is also a **simple** copy of MIT-Scheme. The very idea to make such a implementation is to interpreter the code I worte in Scheme(especially mit-scheme) for [HIT Data Sturcture course](https://github.com/DeathKing/Hit-DataStructure-On-Scheme). But lexically, Hisp also can be considered as:

+ Hit mIt-Scheme inPlementation
+ Hit Iterator Symbolic Processor
+ Hisp Interpreter for Scheme Program

# Standard

Our goal is to make Hisp totally follow the Revised^5 Report on the Algorithmic Language Scheme(aka. R^5RS), but Hisp only suit a subset of that standard now (for more detail information, please check [Issue 1](https://github.com/DeathKing/Hisp/issues/1). We also try to make Hisp more powerful by implement Scheme Requests for Implementation(aka. SRFI). Some modern programming language features such as *default argument* will be added also.

# Features

> **WARNING!**  
> These features haven't finished yet.

## Default parameters

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
# Reference

There are some useful resources when building a compiler or interpreter, which I stolen lots of ideas from.

1. Sussman G, Abelson H, Sussman J. Structure and interpretation of computer programs[J]. The Massachusetts Institute of Technology, 1985, 10.
2. [Ruby Programming Language](https://github.com/ruby/ruby)
3. Ruby Hacking Guide
  1. [Japanese Version](http://loveruby.net/ja/rhg/)
  2. [Chinese Version](http://axgle.github.io/rhg/)
  3. [English Version](http://ruby-hacking-guide.github.io/)
4. Pat Shaughnessy, Ruby Under a Microscope: An Illustrated Guide to Ruby Internals[M], No Starch Press, US, 2013, 11.
5. [《Lua源码欣赏》, 云风](http://www.codingnow.com/temp/readinglua.pdf)

# Contributing

Take it easy, guys, just `(and (fork) (send 'pull-request))`.

# Credit & License

My friend [_奥利奥有力量](http://weibo.com/u/1922805257) crafted the lovely bear logo, which everyone can freely distrubite once you gave her a credit. 

The whole Hisp project followed the MIT License and our lovely HIT License.

