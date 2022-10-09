clean:
	rm *.o

all:
	g++ -c -I../include l01lib.cpp l02lib.cpp -Wall -Wpedantic -Wextra -Ofast
	ar rcs libcvlib.a l01lib.o l02lib.o

install:
	cp libcvlib.a ../lib
