.PHONY: all test clean

CXX=g++ -g -std=c++17 -O3 -Wall -fopenmp

all: gar

test: test_siso93
	./test_siso93

clean:
	rm -rvf  decoding_tables.cc *.o gar generate_decoding_tables test_siso93

%.o: %.cc %.hh Makefile
	$(CXX) -c -o $@ $<

siso93.o: siso93.cc siso93.hh decoding_tables.cc Makefile
	$(CXX) -c -o $@ $<

decoding_tables.cc : generate_decoding_tables
	./generate_decoding_tables > $@

gar: siso93.o rolling_hash.o codec.o bin_codec.o text_codec.o zip_codec.o
	$(CXX) -o $@ $^ -lpthread

test_siso93: test_siso93.o siso93.o
	$(CXX) -o $@ $^ -lgtest -lgtest_main -lpthread

generate_decoding_tables: siso93.cc siso93.hh
	$(CXX) -DPRINT_DECODING_TABLES -o $@ $<
