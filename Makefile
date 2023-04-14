all: add-nbo

add-nbo: main.o
	g++ -o add-nbo main.o

main.o: main.cpp
	g++ -std=c++17 -O2 -c -o main.o main.cpp

clean: 
	rm -f *.o
	rm -f add-nbo
