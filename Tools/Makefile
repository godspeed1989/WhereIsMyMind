CC = g++
CXXFLAGS = -Wall -g
LIBS = -lcxxtools -lzim
RM = rm -rf
TARGETS = dir_iterator createzim_test

all: $(TARGETS)

dir_iterator: dir_iterator.cpp
	$(CC) $(CXXFLAGS) $< $(LIBS) -o $@

createzim_test: createzim_test.cpp
	$(CC) $(CXXFLAGS) $< $(LIBS) -o $@

clean:
	$(RM) *.o $(TARGETS)
