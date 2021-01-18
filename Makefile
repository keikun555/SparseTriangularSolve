HFILES=$(shell ls -1 *.h include/*.h)
OFILES=matrix_operations.o
CC=g++
CC_FLAGS=-fopenmp -Werror -g -o3 -I ./boost_1_66_0

# main program
main: $(HFILES) $(OFILES) main.o
	$(CC) $(CC_FLAGS) -o $@ main.o $(OFILES)

# How to compile a .o file from a .cpp file.
%.o: %.cpp $(HFILES)
	$(CC) $(CC_FLAGS) -c -o $@ $<

# boost requirement (used for graphs)
boost:
  git submodule update --init --recursive

# Deletes the temporary files we have built.
.PHONY: clean all
clean:
	rm -f *.o
	rm -f main