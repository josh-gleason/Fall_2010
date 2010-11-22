flatten([],[]).
flatten([[]|A],B) :- flatten(A,B). % in case [] are in the list
flatten([A|X],[A|Y]) :- atomic(A), flatten(X,Y).
flatten([A|T],B) :- flatten(A,X), flatten(T,Y), append(X,Y,B).
