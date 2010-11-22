%1 a
isSet([]).
isSet([H|T]) :- not(member(H,T)), isSet(T).

%1 b
subset([],_).
subset([H|T],B) :- member(H,B), subset(T,B).

%1 c
union([],B,B).
union([H|A],B,C) :- member(H,B), union(A,B,C).
union([H|A],B,[H|C]) :- union(A,B,C).

%1 d
intersection([],_,[]).
intersection([H|A],B,[H|C]) :- member(H,B), intersection(A,B,C).
intersection([_|A],B,C) :- intersection(A,B,C).

%2
tally(_,[],0).
tally(A,[A|B],N) :- tally(A,B,O), N is O+1. % add 1 to returned value O
tally(A,[_|B],N) :- tally(A,B,N).

%3
subst(_,_,[],[]).
subst(A,B,[A|T],[B|R]) :- subst(A,B,T,R).
subst(A,B,[C|T],[C|R]) :- subst(A,B,T,R).

%4
insert(A,[],[A]).       % special case where inserted at end of set
insert(A,[H|B],[H|C]) :- A>H, insert(A,B,C).
insert(A,[H|C],[A|[H|C]]) :- A=<H.

%5 (Extra Credit)
flatten([],[]).
flatten([[]|A],B) :- flatten(A,B). % in case [] are in the list
flatten([A|X],[A|Y]) :- atomic(A), flatten(X,Y).
flatten([A|T],B) :- flatten(A,X), flatten(T,Y), append(X,Y,B).
