all:
	g++ -Wall -g -std=c++11 main.cpp -o hlvm

clean:
	rm -f hlvm

.PHONY : all clean
