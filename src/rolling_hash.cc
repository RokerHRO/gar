#include <cstdint>

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <deque>
#include <string>
#include <string_view>


const unsigned LineLengthMin = 80;
const unsigned LineLengthMax = 4096;

const unsigned LineLengthStretch = LineLengthMax - LineLengthMin;
const uint64_t HashStep = UINT64_MAX / LineLengthStretch;


const char* const encoding_value =
//	 0123456789abcdef.123456789abcdef
	"@ABCDEFGHIJKLMNOPQRSTUVWXYZ[2]^/" // 0x .. 1x
	"_!*#$%$'()*+,-./0123456789:;(=)?" // 2x .. 3x
	"@ABCDEFGHIJKLMNOPQRSTUVWXYZ[!]^_" // 4x .. 5x
	"`abcdefghijklmnopqrstuvwxyz{|}~?" // 6x .. 7x
	"`abcdefghijklmnopqrstuvwxyz{|}~." // 8x .. 9x
	"_!0#$%#'()*+,-./0123456789:;1=3?" // Ax .. Bx
	"@ABCDEFGHIJKLMNOPQRSTUVWXYZ[4]^7" // Cx .. Dx
	"`abcdefghijklmnopqrstuvwxyz{|}~!";// Ex .. Fx

char encoding_plane[257] =
//	 0123456789abcdef.123456789abcdef
	"11111111111111111111111111111111" // 0x .. 1x
	"00100010000000000000000000001010" // 2x .. 3x
	"00000000000000000000000000001001" // 4x .. 5x
	"00000000000000000000000000000001" // 6x .. 7x
	"11111111111111111111111111111111" // 8x .. 9x
	"22122212222222222222222222221212" // Ax .. Bx
	"22222222222222222222222222221221" // Cx .. Dx
	"22222222222222222222222222222221";// Ex .. Fx

const char* const encoding_tag =
//	 000111222
//	 012012012
	"`.^,JK|ST" // 00
	"~@ELMNUVW" // 01
	":GHOPQXYZ" // 02
	"'abijkrst" // 10
	"cdelmnuvw" // 11
	"fghopqxyz" // 12
	";129[]FI(" // 20
	"345{}A)=#" // 21
	"657BCD*$%";// 22

constexpr
uint64_t Pow(uint64_t basis, unsigned exponent)
{
	return exponent ? Pow(basis, exponent-1) * basis : 1 ;
}

typedef uint8_t U8;


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


uint64_t break_hash(unsigned length)
{
	if(length<=LineLengthMin) return 0;
	if(length>=LineLengthMax) return ~uint64_t(0);
	
	return (length-LineLengthMin) * HashStep;
}


static const unsigned factors[4] = {27, 9, 3, 1};

std::pair<std::string, std::string> encode_siso93(const std::string& s)
{
	std::string tag, data;
	tag.reserve((s.size()+3)/4);
	data.reserve(s.size());
	
	const size_t max_size = s.size() & ~3ull;
	for(size_t o=0; o<max_size; o+=4)
	{
		const unsigned t = encoding_plane[ U8(s[o  ]) ]*27
		                 + encoding_plane[ U8(s[o+1]) ]*9
		                 + encoding_plane[ U8(s[o+2]) ]*3
		                 + encoding_plane[ U8(s[o+3]) ];
		
		tag += encoding_tag[t];
		data += encoding_value[ U8(s[o  ]) ];
		data += encoding_value[ U8(s[o+1]) ];
		data += encoding_value[ U8(s[o+2]) ];
		data += encoding_value[ U8(s[o+3]) ];
	}
	
	const unsigned* factor = factors;
	if(max_size < s.size())
	{
		unsigned t = 0;
		size_t o = max_size;
		while(o < s.size())
		{
			   t += encoding_plane[ U8(s[o]) ] * (*factor);
			data += encoding_value[ U8(s[o]) ];
			++o;
			++factor;
		}
		tag += encoding_tag[t];
	}
	
	return std::make_pair(std::move(tag), std::move(data));
}

int main()
{
	for(unsigned u=0; u<256; ++u)
	{
		encoding_plane[u] -= '0';
	}

	const std::string es = "Banane";
	
	RollingHash rh(12, "H3V2pgvnSnKmBHRrurim9g=="); // just 16 random bytes base64-encoded.
	
	fprintf(stderr, "# LineLen: %u ... %u\n HashStep: 0x%016llx.\n", LineLengthMin, LineLengthMax, (unsigned long long)HashStep);
	
	for(unsigned u=LineLengthMin-5; u<LineLengthMax+5; ++u)
	{
		fprintf(stderr, "# Len: %u : HashStep: 0x%016llx.\n", u, (unsigned long long)break_hash(u) );
	}
	
	std::string line;
	while(!feof(stdin))
	{
		const int c = getchar();
		if(c == EOF)
			break;
		
		const auto h = rh.add( c );
		line += char(c);
		
		if( h <= break_hash(line.size()) )
		{
			const auto [tag,data] = encode_siso93(line);
			printf("> %s\n%s\n", tag.c_str(), data.c_str());
			fflush(stdout);
			line.clear();
		}
	}
	
}
