insert(A,[],[A]).       % special case where inserted at end of set
insert(A,[H|B],[H|C]) :- A>H, insert(A,B,C).
insert(A,[H|C],[A|[H|C]]) :- A=<H.
