#ifndef GAR_CODEC_HH
#define GAR_CODEC_HH

#include <string_view>
#include <map>

class Codec
{
public:
	virtual ~Codec() = default;
	
	virtual std::string encode(std::string_view input) = 0;
	virtual std::string decode(std::string_view input) = 0;
};


using CodecList = std::map <std::string_view, Codec*>;
using CodecPair = std::pair<std::string_view, Codec*>;

// map common well-known file extensions to appropriate Codec
extern const CodecList  extension2codec;

// map file type (-t commandline option) o appropriate Codec
extern const CodecList  filetype2codec;


Codec* getCodecByExtension(std::string_view extension);


inline
CodecList operator+(CodecList head, const std::initializer_list<CodecPair>& tail)
{
	head.insert(tail.begin(), tail.end());
	return std::move(head);
}


#endif // GAR_CODEC_HH
