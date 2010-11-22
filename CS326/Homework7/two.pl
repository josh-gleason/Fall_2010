tally(_,[],0).
tally(A,[A|B],N) :- tally(A,B,O), N is O+1.
tally(A,[_|B],N) :- tally(A,B,N).
