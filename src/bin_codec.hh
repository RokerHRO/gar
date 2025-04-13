#ifndef GAR_BIN_CODEC_HH
#define GAR_BIN_CODEC_HH

#include "codec.hh"
#include "rolling_hash.hh"
#include "line_breaker.hh"


class BinCodec : public Codec
{
public:
	BinCodec(unsigned min_line_length, unsigned max_line_length);
	virtual ~BinCodec() = default;
	
	virtual std::string encode(std::string_view input);
	virtual std::string decode(std::string_view input);
	
	static BinCodec* getDefaultCodec();

private:
	RollingHash hash;
	LineBreaker breaker;
};

const std::initializer_list<CodecPair>& getBinCodecs();

#endif // GAR_BIN_CODEC_HH
