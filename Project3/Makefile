.SILENT: l03 all run buildrun

clean:
	rm l03 

l03:
	g++ -o l03 -I../include -L../lib  l03.cpp  -lcvlib -Wall -Wpedantic -Wextra -Ofast -std=c++11

all:
	make l03

run:
	echo "--- Running Code ---"
	./l03
	echo "--- Converting to JPG ---"
	convert l03.ppm l03.jpg

buildrun:
	make all
	make run
	