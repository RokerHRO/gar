#include "bin_codec.hh"
#include "siso93.hh"

static
void append_siso93(std::string& output, const std::string& line)
{
	const auto [tag,data] = encode_siso93(line);
	output += "> ";
	output += tag;
	output += '\n';
	output += data;
	output += '\n';
}


BinCodec::BinCodec(unsigned min_line_length, unsigned max_line_length)
: hash{12, "H3V2pgvnSnKmBHRrurim9g=="} // just 16 random bytes base64-encoded.
, breaker{min_line_length, max_line_length}
{}


std::string BinCodec::encode(std::string_view input)
{
	std::string ret;
	ret.reserve(input.size() * 1.1);
	
	std::string line;
	for(char c : input)
	{
		const auto h = hash.add( c );
		line += char(c);
		
		if( h <= breaker.break_hash(line.size()) )
		{
//			line_lengths += line.length();
//			++lines;
			append_siso93(ret, line);
			line.clear();
		}
	}
	
	if(!line.empty())
	{
		append_siso93(ret, line);
	}
	
	return ret;
}


std::string BinCodec::decode(std::string_view input)
{
	std::string ret;
	
	
	return ret;
}


BinCodec* BinCodec::getDefaultCodec()
{
	static BinCodec process_bin{64, 1024};
	return &process_bin;
}


const std::initializer_list< CodecPair >& getBinCodecs()
{
	static auto codec_bin = BinCodec::getDefaultCodec();

	static const std::initializer_list<CodecPair> data =
	{
		{"",    codec_bin},
		{"png", codec_bin},
		{"jpg", codec_bin},
	};
	
	return data;
}
