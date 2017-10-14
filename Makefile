
SOURCE=main.cpp bmpIO.cpp
all:	
	g++ -g -Wall -o main $(SOURCE)
clean: 
	rm -f  main out*.bmp

