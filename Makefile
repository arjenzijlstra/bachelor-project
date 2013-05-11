
# use g++ 4.7 together with C++11 as standard
#CC = g++-4.7
CC = g++
CFLAGS = --std=c++0x -Wall -pedantic -fopenmp -O2 -g
LDFLAGS = -D_GLIBCXX_PARALLEL -fopenmp

SOURCES = $(wildcard *.cpp */*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

EXECUTABLE = project

all: $(EXECUTABLE)
#	make -C dedekind

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o "$@"

%.o: %.cpp
	$(CC) $(CFLAGS) -c "$<" -o "$@"

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)

make.dep: $(SOURCES)
	$(CC) $(CFLAGS) -MM $^ > "$@"

include make.dep
