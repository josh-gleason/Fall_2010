intersection([],_,[]).
intersection([H|A],B,[H|C]) :- member(H,B), intersection(A,B,C).
intersection([_|A],B,C) :- intersection(A,B,C).
