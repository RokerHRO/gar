#include "codec.hh"
#include "bin_codec.hh"
#include "zip_codec.hh"


// map common well-known file extensions to appropriate Codec
const std::map<std::string_view, Codec*>  extension2codec =
	CodecList{} +
	getZipCodecs() +
	getBinCodecs();


// map file type (-t commandline option) o appropriate Codec
const std::map<std::string_view, Codec*>  filetype2codec =
	{ 
		{ "zip", ZipCodec::getDefaultCodec() },
		{ "bin", BinCodec::getDefaultCodec() },
	};
