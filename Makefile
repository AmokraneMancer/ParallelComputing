
SRC = $(shell find . -name "*.cpp")
BIN = $(SRC:.cpp=.out)

all: $(BIN)

%.out: %.cpp
	mpic++ -std=c++11 -Wall -Wextra -O2 -lboost_mpi -lboost_serialization -o $@ $< 
	@echo "to run: mpirun -n 4 $@"

.PHONY: clean

clean:
	rm $(BIN)
