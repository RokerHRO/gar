.PHONY: all clean

all: gar

clean:
	rm -rvf  *.o gar

%.o: %.cc %.hh
	$(CXX) -g -std=c++17 -O3 -Wall -c -o $@ $<


gar: siso93.o rolling_hash.o
	$(CXX) -g -std=c++17 -O3 -Wall -o $@ $^


