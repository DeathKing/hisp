# Terms

## Operations on Environments

### Binding

Description: A association of variables with their corresponding values.

Detail: Hisp simply implement the binding by cons(`hp_cons()` call) the `var` and `val` to form a pair. 

### Frame

Description: A table of bindings.

Detail: Hisp combine the bindings into a list.

### Environment

Description: A sequence of frames.

Detail: Environment is a pair, whose car is a frame standard for current level variable-value bindings and cdr is a parent environment. Ton-environment's cdr is `'()`.


