main: main.cc image.h funcs.h
	g++ main.cc `pkg-config opencv --cflags --libs` -o main -g #-O3

clean:
	rm main

.PHONY: clean
