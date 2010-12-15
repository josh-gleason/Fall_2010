main: main.cc image.h funcs.h
	g++ main.cc `pkg-config opencv --cflags --libs` -o main -g -Wall -O3

countzeros: countzeros.cc image.h funcs.h
	g++ countzeros.cc `pkg-config opencv --cflags --libs` -o countzeros -g -Wall -O3

clean:
	rm main

.PHONY: clean
