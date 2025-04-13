#ifndef GAR_ZIP_CODEC_HH
#define GAR_ZIP_CODEC_HH

#include "codec.hh"

class ZipCodec : public Codec
{
public:
	virtual ~ZipCodec() = default;
	
	virtual std::string encode(std::string_view input);
	virtual std::string decode(std::string_view input);

	static ZipCodec* getDefaultCodec();
};

const std::initializer_list<CodecPair>& getZipCodecs();

#endif // GAR_ZIP_CODEC_HH
