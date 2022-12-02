.SILENT: l04 all run buildrun

clean:
	rm l04 

l04:
	g++ -o l04 -I../include -L../lib  l04.cpp  -lcvlib -Wall -Wpedantic -Wextra -Ofast

all:
	make clean
	make l04

run:
	echo "--- Running Code ---"
	./l04
	echo "--- Converting to JPG ---"
	convert l04*.ppm l04*.jpg

buildrun:
	make all
	make run
	