.SILENT: l02 all run buildrun

clean:
	rm l02 

l02:
	g++ -o l02 -I../include -L../lib  l02.cpp  -lcvlib -Wall -Wpedantic -Wextra -Ofast

all:
	make l02

run:
	echo "--- Running Code ---"
	./l02
	echo "--- Converting to JPG ---"
	convert l02.ppm l02.jpg
	convert Images/out1.ppm Images/out1.jpg
	convert Images/out2.ppm Images/out2.jpg
	convert Images/out3.ppm Images/out3.jpg
	convert Images/out4.ppm Images/out4.jpg
	convert Images/out5.ppm Images/out5.jpg
	convert Images/out6.ppm Images/out6.jpg
	cd ..

buildrun:
	make all
	make run
	