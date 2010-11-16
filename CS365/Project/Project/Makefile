LIBS = -lglut -lGLU -lGL `pkg-config libpng --cflags --libs`

main: main.cpp loadpng.o
	g++ -Wall -o main main.cpp loadpng.o $(LIBS)
	rm loadpng.o

loadpng.o: loadpng.cpp loadpng.h
	g++ -c loadpng.cpp $(LIBS)

clean:
	rm -f main
