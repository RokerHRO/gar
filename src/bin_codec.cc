#include "bin_codec.hh"
#include <string>

BinCodec::BinCodec(unsigned min_line_length, unsigned max_line_length)
: hash{}
, breaker{min_line_length, max_line_length}
{}


std::string BinCodec::encode(std::string_view input)
{

}


std::string BinCodec::decode(std::string_view input)
{

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
