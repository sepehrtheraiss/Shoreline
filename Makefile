all:
	g++ -c Blocker.cpp
	g++ -c main.cpp
	g++ -o out main.o Blocker.o
