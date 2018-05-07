parent(tom, bob).
parent(bob, pat).
parent(bob, ada).
parent(pat, jim).
parent(ada, michelle).

male(tom).
male(bob).
male(jim).
male(pat).
female(ada).
female(michelle).

sibling(X, Y) :- parent(Z, X), parent(Z, Y), X \= Y.
brother(X, Y) :- male(X), sibling(X, Y).
aunt(X, Y) :- female(X), parent(Z, Y), sibling(X, Z).
cousin(X, Y) :- parent(Z, X), parent(W, Y), sibling(Z, W).

ancestor(X, Y) :- parent(X, Y).
ancestor(X, Y) :- parent(Z, Y), ancestor(X, Z).


third([_,_,X|_], X). % Simple in-line pattern matching.

next([_|X], X).

my_last([X], X).
my_last([_|T], Y) :- my_last(T, Y).

prefix([_|_], []).
prefix([X|A], [Y|B]):- X = Y, prefix(A, B).

each_one([A|_],A).
each_one([_|B],X):- each_one(B,X).
/** <examples>
?- each_one([1,2,3,4], X).
*/