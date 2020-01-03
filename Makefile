all:
	g++ -std=c++11 -c Blocker.cpp
	g++ -std=c++11 -c main.cpp
	g++ -o out main.o Blocker.o
