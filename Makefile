rebuild:
	rm -f main *.o
	g++ -c SHA2/SHA2.cpp -o main.o
	g++ -o main main.o
