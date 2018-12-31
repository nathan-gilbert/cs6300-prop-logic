# Propositional Logic

## To Run:

```sh
% ./inference [args...]
```

## Problem Definition

The goal of this assignment was to develop a simple inference engine for a
subset of propositional logic. In this subset of propositional logic there is
not a negation operation and the or operation was only implicitly defined. The
inference methods to be employed by this assignment were forward and backwards
chaining. These methods would make inferences from a knowledge base that
consisted of asserted facts and implications (essentially Horn clauses.)
Forward chaining works by deriving all facts possible by the knowledge base,
backward chaining works by starting from the goal and working through it’s
premises to see if it is derivable from the given axioms.  This is an
interesting problem for computer scientists because it is a solid introduction
to methods that model reasoning as computation. This is one of the key problems
that face researchers in artificial intelligence. Therefore, it is not
unreasonable to start where humans have previously developed formalizations of
reasoning, notably, in this case, the propositional logic.
