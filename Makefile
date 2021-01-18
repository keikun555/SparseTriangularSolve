HFILES=$(shell ls -1 *.h include/*.h)
OFILES=matrix_operations.o
CC=g++
CC_FLAGS=-fopenmp -Wall -Werror -g -o3 -I ./include -std=c++11

# main program
main: $(HFILES) $(OFILES) main.o
	$(CC) $(CC_FLAGS) -o $@ main.o $(OFILES)

# How to compile a .o file from a .cpp file.
%.o: %.cpp $(HFILES)
	$(CC) $(CC_FLAGS) -c -o $@ $<

# Deletes the temporary files we have built.
.PHONY: clean all
clean:
	rm -f *.o
	rm -f main