.SILENT: sample all run buildrun

clean:
	rm sample 

sample:
	g++ -o sample -I../include -L../lib  sample.cpp  -lcvlib -Wall -Wpedantic -Wextra -Ofast

all:
	make sample

run:
	echo "--- Running Code ---"
	./sample
	echo "--- Converting to JPG ---"
	convert sample.ppm sample.jpg

buildrun:
	make all
	make run
	