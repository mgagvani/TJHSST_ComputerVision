.SILENT: l06 all run buildrun

clean:
	rm l06 

l06:
	g++ -o l06 -I../include -L../lib  l06.cpp  -lcvlib -Wall -Wpedantic -Wextra -Ofast

all:
	rm l06
	make l06

run:
	echo "--- Running Code ---"
	./l06
	echo "--- Converting to JPG ---"
	convert l06.ppm l06.jpg

buildrun:
	make all
	make run
	