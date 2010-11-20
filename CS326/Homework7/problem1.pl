isSet(X) :-
	X=[]; X=[H|T], not(member(H,T)), isSet(T).

subset(X,Y) :-
	X=[]; X=[H|T], member(H,Y), subset(T,Y).
