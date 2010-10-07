all: main phase

main:main.cc image.h funcs.h
	g++ -g -Wall main.cc `pkg-config opencv --cflags --libs` -o main -O3 

phase:phase.cc image.h funcs.h
	g++ -g -Wall phase.cc `pkg-config opencv --cflags --libs` -o phase -O3

clean:
	rm main phase

.PHONY: clean all

