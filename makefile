concoord: concoord.o
	g++ -o concoord concoord.cpp

install:
	cp concoord /usr/bin
