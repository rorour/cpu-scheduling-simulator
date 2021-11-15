omain: main.cpp data_structs.cpp data_structs.h
	g++ -o main main.cpp data_structs.cpp -std=c++11

clean:
	rm -f *.o
	rm -f main
	rm -f data_structs