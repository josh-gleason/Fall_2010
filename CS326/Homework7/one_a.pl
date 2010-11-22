isSet([]).
isSet([H|T]) :- not(member(H,T)), isSet(T).
