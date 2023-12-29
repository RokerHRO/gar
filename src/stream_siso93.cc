#include "stream_siso93.hh"
#include "rolling_hash.hh"
#include "siso93.hh"

Siso93::Siso93(unsigned line_length_min, unsigned line_length_max)
:rh{ new RollingHash(line_length_min, line_length_max, 12, "H3V2pgvnSnKmBHRrurim9g==") } // just 16 random bytes base64-encoded.
{
}

Siso93::~Siso93()
{
	delete rh;
}


void Siso93::encode(std::istream& in, std::ostream& out)
{
	std::string line;
	while(!feof(stdin))
	{
		const int c = getchar();
		if(c == EOF)
			break;
		
		const auto h = rh.add( c );
		line += char(c);
		
		if( h <= rh.break_hash(line.size()) )
		{
			const auto [tag,data] = encode_siso93(line);
			printf("> %s\n%s\n", tag.c_str(), data.c_str());
			fflush(stdout);
			line.clear();
		}
	}
}


void decode(std::istream& in, std::ostream& out)
{

}
