subset([],_).
subset([H|T],B) :- member(H,B), subset(T,B).
