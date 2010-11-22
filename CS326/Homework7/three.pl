subst(_,_,[],[]).
subst(A,B,[A|T],[B|R]) :- subst(A,B,T,R).
subst(A,B,[C|T],[C|R]) :- subst(A,B,T,R).
