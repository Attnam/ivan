.POSIX:
.SUFFIXES: .cc
CXX      = c++
CXXFLAGS = -std=c++11 -Wall -Wextra -O3 -g3

all: namegen

namegen: namegen.o example.o
	$(CXX) $(LDFLAGS) -o $@ namegen.o example.o $(LDLIBS)

namegen.o: namegen.cc
example.o: example.cc

clean:
	rm -rf namegen namegen.o example.o

.cc.o:
	$(CXX) -c $(CXXFLAGS) -o $@ $<
