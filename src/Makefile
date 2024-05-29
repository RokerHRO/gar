.PHONY: all clean

CXX=g++ -g -std=c++17 -O3 -Wall

all: gar

clean:
	rm -rvf  *.o gar

%.o: %.cc %.hh
	$(CXX) -c -o $@ $<

siso93.o: siso93.cc siso93.hh decoding_tables.cc
	$(CXX) -c -o $@ $<

decoding_tables.cc : generate_decoding_tables
	./generate_decoding_tables > $@

gar: siso93.o rolling_hash.o
	$(CXX) -o $@ $^


generate_decoding_tables: siso93.cc siso93.hh
	$(CXX) -DPRINT_DECODING_TABLES -o $@ $<
