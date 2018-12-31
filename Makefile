# Nathan Gilbert
# CS 6300 
# Assignment 3 
inference: inference.cpp theorem.cpp theorem.h
	g++ inference.cpp theorem.cpp util.cpp -Wall -Wextra -pedantic -g -lpopt -o inference

clean:
	rm ./inference
