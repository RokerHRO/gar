#ifndef GAR_TEXT_CODEC_HH
#define GAR_TEXT_CODEC_HH

#include "codec.hh"

class TextCodec : public Codec
{
public:
	virtual ~TextCodec() = default;
	
	virtual std::string encode(std::string_view input);
	virtual std::string decode(std::string_view input);
	
	static TextCodec* getDefaultCodec();
	
	// checks whether input data consists of UTF-8 text lines (terminated by CRLF)
	// Nota Bene: not all illegal UTF-8 sequences are detected, yet,
	//            e.g. overlong sequences or codes > U+10FFFF.
	static bool isText(std::string_view input);
};

const std::initializer_list<CodecPair>& getTextCodecs();

#endif // GAR_TEXT_CODEC_HH
