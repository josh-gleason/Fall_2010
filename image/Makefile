all: main phase

main:main.cc image.h funcs.h
	g++ -g -Wall main.cc `pkg-config opencv --cflags --libs` -o main 

phase:phase.cc image.h funcs.h
	g++ -g -Wall phase.cc `pkg-config opencv --cflags --libs` -o phase

clean:
	rm main phase

.PHONY: clean all

