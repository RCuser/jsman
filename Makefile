CFLAGS = -I ./include
#LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: jsman

jsman: jsman.cpp ppm.cpp
	g++ $(CFlags) jsman.cpp chrisR.cpp jeremyM.cpp danbiY.cpp albertK.cpp luisA.cpp ppm.cpp /usr/lib/x86_64-linux-gnu/libopenal.so \
	/usr/lib/libalut.so libggfonts.a -Wall -ojsman -lX11 -lGL -lGLU -lm

clean:
	rm -f jsman
	rm -f *.o

