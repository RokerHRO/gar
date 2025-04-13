#include "codec.hh"
#include "bin_codec.hh"
#include "text_codec.hh"
#include "zip_codec.hh"


// map common well-known file extensions to appropriate Codec
const std::map<std::string_view, Codec*>  extension2codec =
	CodecList{} +
	getTextCodecs() +
	getZipCodecs() +
	getBinCodecs();


// map file type (-t commandline option) o appropriate Codec
const std::map<std::string_view, Codec*>  filetype2codec =
	{ 
		{ "bin", BinCodec::getDefaultCodec() },
		{ "txt", TextCodec::getDefaultCodec() },
		{ "zip", ZipCodec::getDefaultCodec() },
	};


Codec* getCodecByExtension(std::string_view extension)
{
	auto f = extension2codec.find(extension);
	if (f == extension2codec.end())
	{
		f = extension2codec.find("");
	}
	
	return f->second;
}
