#include "siso93.hh"
#include <cstdint>


namespace
{

typedef uint8_t U8;

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


static const unsigned factors[4] = {27, 9, 3, 1};

} // end of anonymous namespace


std::pair<std::string, std::string> encode_siso93(std::string_view s)
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
