.SILENT: l05 all run buildrun

clean:
	rm l05 

l05:
	g++ -o l05 -I../include -L../lib  l04.cpp  -lcvlib -Wall -Wpedantic -Wextra -Ofast

all:
	make clean
	make l05

run:
	echo "--- Running Code ---"
	./l05
	echo "--- Converting to JPG ---"
	convert l05*.ppm l05*.jpg

buildrun:
	make all
	make run
	