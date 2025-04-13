#include "bin_codec.hh"
#include <string>

std::string BinCodec::encode(std::string_view filename)
{

}


std::string BinCodec::decode(std::string_view filename)
{

}


BinCodec* BinCodec::getDefaultCodec()
{
	static BinCodec process_bin;
	return &process_bin;
}


const std::initializer_list< CodecPair >& getBinCodecs()
{
	static auto codec_bin = BinCodec::getDefaultCodec();

	static const std::initializer_list<CodecPair> data =
	{
		{"png", codec_bin},
		{"jpg", codec_bin},
	};
	
	return data;
}
