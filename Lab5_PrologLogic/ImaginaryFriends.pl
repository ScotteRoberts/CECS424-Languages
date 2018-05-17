:- use_module(library(clpfd)).

animal(bear).
animal(moose).
animal(seal).
animal(zebra).

adventure(circus).
adventure(band).
adventure(spaceship).
adventure(train).

solve :-
    animal(JoanneAnimal), animal(LouAnimal), animal(RalphAnimal), animal(WinnieAnimal),
	all_different([JoanneAnimal, LouAnimal, RalphAnimal, WinnieAnimal]),
    
    adventure(JoanneAdventure), adventure(LouAdventure),
    adventure(RalphAdventure), adventure(WinnieAdventure),
	all_different([JoanneAdventure, LouAdventure, RalphAdventure, WinnieAdventure]),
    
    Triples = [	[joanne, JoanneAnimal, JoanneAdventure],
                [lou, LouAnimal, LouAdventure],
                [ralph, RalphAnimal, RalphAdventure],
                [winnie, WinnieAnimal, WinnieAdventure]	],
    
    /** First Rule
     * The seal (who isn't the creation of either Joanne or Lou) neither rode to the moon
     * in a spaceship nor took a trip around the world on a magic train.
     */
    \+ member([joanne, seal, _], Triples),
    \+ member([lou, seal, _], Triples),
    \+ member([_, seal, spaceship], Triples),
    \+ member([_, seal, train], Triples),
    
    /** Second rule
     * Joanne's imaginary friend (who isn't the grizzly bear) went to the circus.
     */
    \+ member([joanne, bear, _], Triples),
    member([joanne, _, circus], Triples),
    
    /** Third Rule
     * Winnie's imaginary friend is a zebra.
     */
    member([winnie, zebra, _], Triples),
    
    /** Fourth Rule
     * The grizzly bear didn't board spaceship to the moon.
     */
    \+ member([_, bear, spaceship], Triples),

	% Print Results
	tell(joanne, JoanneAnimal, JoanneAdventure),
	tell(lou, LouAnimal, LouAdventure),
    tell(ralph, RalphAnimal, RalphAdventure),
    tell(winnie, WinnieAnimal, WinnieAdventure).
    
    
% Succeeds if all elements of the argument list are bound and different.
% Fails if any elements are unbound or equal to some other element.
all_different([H | T]) :- member(H, T), !, fail.
all_different([_ | T]) :- all_different(T).
all_different([_]).

tell(X, Y, Z) :-
    write(X), write('\'s imaginary friend is the '), write(Y),
    write(' and they went on a '), write(Z), write(' adventure.'), nl.

