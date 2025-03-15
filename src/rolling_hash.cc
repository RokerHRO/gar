#include "siso93.hh"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <deque>
#include <string>
#include <string_view>

#include <unistd.h> // for getopt()



static constexpr
uint64_t Pow(uint64_t basis, unsigned exponent)
{
	return exponent ? Pow(basis, exponent-1) * basis : 1 ;
}

static constexpr
uint64_t sqr(uint32_t u)
{
	return uint64_t{u}*u;
//	return u;
}


struct RollingHash
{
public:
	RollingHash(unsigned length=8)
	: MaxExpon{length}
	, Power{ Pow(257, MaxExpon) }
	, buffer{}
	, h{0}
	{ }
	
	RollingHash(unsigned length, const std::string_view& sv)
	: RollingHash(length)
	{
		for(char c : sv)
		{
			add(c);
		}
	}
	
	uint64_t add(char c)
	{
		if(buffer.size()>MaxExpon)
		{
			const uint64_t minus = (uint8_t(buffer[0]) + 1u) * Power;
			h -= minus;
			buffer.pop_front();
		}
		buffer.push_back(c);

		h = (h*257u + uint8_t(c) + 1u);
		return xrm(h);
	}
	
	uint64_t add(std::string_view sv)
	{
		for(char c:sv)
			add(c);
		
		return xrm(h);
	}
	
	constexpr
	uint64_t hash() const { return xrm(h); }
	
	std::string string() const
	{
		std::string s;
		for(char c : buffer)
		{
			s += c;
		}
		return s;
	}
	
private:
	const unsigned MaxExpon;
	const uint64_t Power;
	std::deque<uint8_t> buffer;
	uint64_t h;
	
	static constexpr
	uint64_t xrm(uint64_t u)
	{
		u ^= 0x727C9716FFB764D5; // 2**64 / sqrt(5)
		u  = (u>>41) | (u<<(64-41)); // bitrotate right by 41 bit.
		u *= 0x3ABAFD5229726D8D;  // 2**64 / sqrt(19)
		
		u ^= 0x2F89BACC7D77102C; // 2**64 / sqrt(29)
		u  = (u>>27) | (u<<(64-27)); // bitrotate right by 27 bit.
		u *= 0x2284DF580CA9B57B; // 2**64 / sqrt(55)
		
		return u;
	}
};


class LineBreaker
{
public:
	LineBreaker(unsigned min_length, unsigned max_length)
	: LineLengthMin(min_length)
	, LineLengthMax(max_length)
	, HashStep{UINT64_MAX / sqr(LineLengthMax - LineLengthMin)}
	{
		fprintf(stderr, "# LineLen: %u ... %u\n HashStep: 0x%016llx.\n", LineLengthMin, LineLengthMax, (unsigned long long)HashStep);
		
		for(unsigned u=LineLengthMin-5; u<LineLengthMax+5; ++u)
		{
			fprintf(stderr, "# Len: %u : HashStep: 0x%016llx.\n", u, (unsigned long long)break_hash(u) );
		}
	}

	uint64_t break_hash(unsigned length) const noexcept
	{
		if(length<=LineLengthMin) return 0;
		if(length>=LineLengthMax) return ~uint64_t(0);
		
		return sqr(length-LineLengthMin) * HashStep;
	}

private:
	const unsigned LineLengthMin;
	const unsigned LineLengthMax;
	const uint64_t HashStep;
};


int main(int argc, char** argv)
{
	uint64_t line_lengths = 0;
	uint64_t lines = 0;
	
	int opt = -42;
	unsigned min_len = 40;
	unsigned max_len = 256;
	
	while( (opt = getopt(argc, argv, "s:l:")) != -1)
	{
		switch(opt)
		{
			case 's' : min_len = atoi(optarg); break;
			case 'l' : max_len = atoi(optarg); break;
			default:
				fprintf(stderr, "Usage: %s [-s min_length] [-l max_length]\n", argv[0]);
				return 1;
		}
	}
	
	if(min_len<8) min_len=8;
	if(min_len>4096) min_len=4096;
	
	if(max_len<min_len) max_len = min_len;
	if(max_len>65536) max_len = 65536;
	
	LineBreaker lb(min_len, max_len);

	RollingHash rh(12, "H3V2pgvnSnKmBHRrurim9g=="); // just 16 random bytes base64-encoded.
/*	
*/
	std::string line;
	while(!feof(stdin))
	{
		const int c = getchar();
		if(c == EOF)
			break;
		
		const auto h = rh.add( c );
		line += char(c);
		
		if( h <= lb.break_hash(line.size()) )
		{
			line_lengths += line.length();
			++lines;
			const auto [tag,data] = encode_siso93(line);
			printf("> %s\n%s\n", tag.c_str(), data.c_str());
			fflush(stdout);
			line.clear();
		}
	}
	
	fprintf(stderr, "Average line length: %f bytes.\n", double(line_lengths)/lines );
}
