main: main.cc image.h funcs.h
	g++ main.cc `pkg-config opencv --cflags --libs` -o main -g -Wall -O3 

clean:
	rm main

.PHONY: clean
