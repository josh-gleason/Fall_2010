all: equalize spec median

equalize: equalize.cc generic.h
	g++ -Wall -g `pkg-config opencv --cflags --libs` equalize.cc -o equalize

spec: spec.cc generic.h
	g++ -Wall -g `pkg-config opencv --cflags --libs` spec.cc -o spec

median: median.cc generic.h
	g++ -Wall -g `pkg-config opencv --cflags --libs` median.cc -o median

clean:
	rm equalize spec median

.PHONY: all clean
