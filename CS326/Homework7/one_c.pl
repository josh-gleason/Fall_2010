union([],B,B).
union([H|A],B,C) :- member(H,B), union(A,B,C).
union([H|A],B,[H|C]) :- union(A,B,C).
