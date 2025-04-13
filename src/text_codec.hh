#ifndef GAR_BIN_CODEC_HH
#define GAR_BIN_CODEC_HH

#include "codec.hh"

class BinCodec : public Codec
{
public:
	virtual ~BinCodec() = default;
	
	virtual std::string encode(std::string_view input);
	virtual std::string decode(std::string_view input);
	
	static BinCodec* getDefaultCodec();
};

const std::initializer_list<CodecPair>& getBinCodecs();

#endif // GAR_BIN_CODEC_HH
