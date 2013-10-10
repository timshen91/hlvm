all:
	g++ -Wall -g -std=c++11 compile.cpp -o hlvm

clean:
	rm -f hlvm

.PHONY : all clean
