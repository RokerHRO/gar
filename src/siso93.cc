#include "siso93.hh"
#include <cstdint>
#include <stdexcept>

// #define PRINT_DECODING_TABLES 1

#ifdef PRINT_DECODING_TABLES
#include <cstdio>
#include <set>
#endif

using namespace std::string_literals;


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
	"`abcdefghijklmnopqrstuvwxyz{|}~=";// Ex .. Fx

const char encoding_plane[257] =
//	 0123456789abcdef.123456789abcdef
	"\001\001\001\001\001\001\001\001\001\001\001\001\001\001\001\001\001\001\001\001\001\001\001\001\001\001\001\001\001\001\001\001" // 0x .. 1x
	"\000\000\001\000\000\000\001\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\001\000\001\000" // 2x .. 3x
	"\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\001\000\000\001" // 4x .. 5x
	"\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\001" // 6x .. 7x
	"\001\001\001\001\001\001\001\001\001\001\001\001\001\001\001\001\001\001\001\001\001\001\001\001\001\001\001\001\001\001\001\001" // 8x .. 9x
	"\002\002\001\002\002\002\001\002\002\002\002\002\002\002\002\002\002\002\002\002\002\002\002\002\002\002\002\002\001\002\001\002" // Ax .. Bx
	"\002\002\002\002\002\002\002\002\002\002\002\002\002\002\002\002\002\002\002\002\002\002\002\002\002\002\002\002\001\002\002\001" // Cx .. Dx
	"\002\002\002\002\002\002\002\002\002\002\002\002\002\002\002\002\002\002\002\002\002\002\002\002\002\002\002\002\002\002\002\001";// Ex .. Fx

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
	"678BCD*$%";// 22


static const unsigned factors[4] = {27, 9, 3, 1};

#ifdef PRINT_DECODING_TABLES

static int16_t decoding_value[3][256];
static int8_t decoding_tag[256][4];

std::string pp(unsigned c)
{
	char pretty_print[16];
	if(c>=' ' && c<='~')
	{
		snprintf(pretty_print, 15, " '%c'", c);
	}else{
		snprintf(pretty_print, 15, "0x%02X", c);
	}
	return pretty_print;
}

std::string html_escape(unsigned c)
{
	char buffer[16] = { char(c), '\0' };
	if(c<32 || c>126)
	{
		snprintf(buffer, 15, "0x%02X", c);
		return buffer;
	}
	switch(c)
	{
		case '"' : return "&quot;";
		case '<' : return "&lt;";
		case '>' : return "&gt;";
		case '&' : return "&amp;";
	}
	return buffer;
}

void generate_decoding_tables()
{
	printf("// THIS FILE IS GENERATED. DO NOT EDIT IT!\n"
		   "\n"
		   "#include <cstdint>\n"
		   "\n");

	for(unsigned u = 0; u < 256; ++u)
	{
		decoding_value[0][u] = decoding_value[1][u] = decoding_value[2][u] = -1;
		decoding_tag[u][0] = decoding_tag[u][1] = decoding_tag[u][2] = decoding_tag[u][3] = -1;
	}

	// data
	for(unsigned u = 0; u < 256; ++u)
	{
		const unsigned char e = encoding_value[u];
		const unsigned char t = encoding_plane[u];
		decoding_value[t][e] = u;
	}

	// tag
	for(int t0=0; t0<=2; ++t0)
	for(int t1=0; t1<=2; ++t1)
	for(int t2=0; t2<=2; ++t2)
	for(int t3=0; t3<=2; ++t3)
	{
		const unsigned tag = t0*27 + t1*9 + t2*3 + t3;
		const unsigned char tc = encoding_tag[tag];
		decoding_tag[tc][0] = t0;
		decoding_tag[tc][1] = t1;
		decoding_tag[tc][2] = t2;
		decoding_tag[tc][3] = t3;
	}

	printf("static const int16_t decoding_value[3][256] = {\n");
	for(unsigned plane=0; plane<=2; ++plane)
	{
		printf("\t{\n");
		for(unsigned c=0; c<256; ++c)
		{
			printf("%s%3i%s", (c%16==0 ? "\t\t":" "), decoding_value[plane][c], (c==255?" ":","));
			if(c%16 == 15)
			{
				printf(" // %s ... %s\n", pp(c-15).c_str(), pp(c).c_str());
			}
		}
		printf("\t}%s\n", (plane==2 ? "" : ","));
	}
	printf("};\n\n");

	printf("static const int8_t decoding_tag[256][4] = {\n");
	for(unsigned c=0; c<256; ++c)
	{
		printf("%s{%2i, %2i, %2i, %2i}%s", (c%4==0 ? "\t\t":" "), decoding_tag[c][0], decoding_tag[c][1], decoding_tag[c][2], decoding_tag[c][3], (c==255?" ":","));
		if(c%4 == 3)
		{
			printf(" // %s %s %s %s\n", pp(c-3).c_str(), pp(c-2).c_str(), pp(c-1).c_str(), pp(c).c_str());
		}

	}
	printf("};\n\n// END OF GENERATED FILE\n");
}

void check_tables()
{
	std::set<char> used_chars;
	
	FILE* html = fopen("tables.html", "w");
	fprintf(html, "<!DOCTYPE html>\n"
		"<html><head><meta charset=\"utf-8\">"
		"<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
		"<style>.err { border:thick solid red; }\n"
		".p0 { background-color: #fff; }\n"
		".p1 { background-color: #ec8; }\n"
		".p2 { background-color: #8be; }\n"
		"</style>"
		"<title>Siso93 generated encoding tables</title></head>\n"
		"<body><h2>Data bytes</h2><table border=\"1\">\n"
		"<tr><th>\\</th>"
		);
	for(unsigned low=0; low<16; ++low)
		fprintf(html, "<th>…%X</th>", low);
	
	std::set< std::pair<char, int> > data_set;
	for(unsigned u=0; u<256; ++u)
	{
		const char* css_class = "";
		if(u%16 ==0)
		{
			fprintf(html,"</tr>\n<tr><th>%X…</th>", u/16);
		}
		const char value = encoding_value[u];
		used_chars.insert(value);
		const int  plane = encoding_plane[u];
		auto [iter, b] = data_set.insert(std::make_pair(value, plane));
		if(b==false)
		{
		 //   throw std::logic_error("encodig_value/plane table invalid at entry #" + std::to_string(u));
			printf("// ERROR at entry 0x%02X!\n", u);
			css_class = "err";
		}
		fprintf(html, "<td class=\"%s p%u\">%s</td>", css_class, plane, html_escape(value).c_str());
	}
	fprintf(html, "</tr></table><p>");
	fprintf(html, "<h3>Unused combinations</h3><table border=\"1\"><tr>");
	for(const char c : used_chars)
	for(int plane = 0; plane<3; ++plane)
	{
		if(data_set.count( std::make_pair(c, plane) )==0 )
		{
			fprintf(html, "<td class=\"p%u\">&nbsp;%s&nbsp;</td>", plane, html_escape(c).c_str());
		}
	}
	fprintf(html, "</tr></table><p>");
	
	fprintf(html, "<h2>Tag bytes</h2><table border=\"1\">\n");
	std::set<char> tag_set;
	for(unsigned u=0; u<3*3*3*3; ++u)
	{
		const char* css_class = "";
		const char tag = encoding_tag[u];
		auto [iter, b] = tag_set.insert(tag);
		if(b==false)
		{
			css_class = "err";
//			throw std::logic_error("encoding_tag table invalid at entry #" + std::to_string(u));
		}
		fprintf(html, "<tr><td>0x%02X</td><td>%i %i %i %i</td><td class=\"%s\">&nbsp;%s&nbsp;</td></tr>", u, u/27, (u/9)%3, (u/3)%3, u%3, css_class, html_escape(tag).c_str());
	}
	fprintf(html, "</table>\n<h3>Unused tag bytes</h3><table border=\"1\"><tr>");
	for(const char c : used_chars)
	{
		if(tag_set.count(c)==0)
		{
			fprintf(html, "<td>&nbsp;%s&nbsp;</td>", html_escape(c).c_str());
		}
	}
	
	fprintf(html, "</tr></table></body></html>\n");
	
	fclose(html);
}
#else

#include "decoding_tables.cc"

#endif // PRINT_DECODING_TABLES

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
	
	if(max_size < s.size())
	{
		const unsigned* factor = factors;
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

std::string decode_siso93(std::string_view tag, std::string_view data)
{
	if(tag.size()>=2 && tag[0]=='>' && tag[1]==' ')
	{
		tag.remove_prefix(2); // skip the "> " prefix
	}
	
	if((data.size()+3)/4 != tag.size())
	{
		throw std::runtime_error("Data and tag size don't match!");
	}
	
	std::string ret;
	ret.reserve(data.size());
	const size_t max_full_quad = data.size()/4;
	for(size_t q = 0; q<max_full_quad; ++q)
	{
		const int8_t (&dtag)[4] = decoding_tag[ uint8_t(tag[q]) ];
		if(dtag[0]<0)
			throw std::runtime_error("Illegal tag byte '"s + tag[q] + "' at postition " + std::to_string(q));
		
		const size_t q4 = q*4;
		const uint8_t c0 = data[q4];
		const int16_t i0 = decoding_value[dtag[0]][c0];
		
		const uint8_t c1 = data[q4+1];
		const int16_t i1 = decoding_value[dtag[1]][c1];
		
		const uint8_t c2 = data[q4+2];
		const int16_t i2 = decoding_value[dtag[2]][c2];
		
		const uint8_t c3 = data[q4+3];
		const int16_t i3 = decoding_value[dtag[3]][c3];
		
		if(i0<0 || i1<0 || i2<0 || i3<0)
			throw std::runtime_error("Illegal data byte(s) at position " + std::to_string(q4));
		
		ret += char(i0);
		ret += char(i1);
		ret += char(i2);
		ret += char(i3);
	}
	
	const size_t q4 = max_full_quad*4;
	if(q4 < data.size())
	{
		const int8_t (&dtag)[4] = decoding_tag[ uint8_t(tag.back()) ];
		if(dtag[0]<0)
			throw std::runtime_error("Illegal tag byte '"s + tag.back() + "' at last postition");
		
		for(size_t q=0; q4+q < data.size(); ++q)
		{
			const uint8_t c0 = data.at(q4+q);
			const int16_t i0 = decoding_value[dtag[q]][c0];
			if(i0<0)
				throw std::runtime_error("Illegal data byte at position " + std::to_string(q4+q));
			
			ret += char(i0);
		}
	}
	
	return ret;
}

#ifdef PRINT_DECODING_TABLES

int main()
{
	check_tables();
	generate_decoding_tables();
}

#endif
