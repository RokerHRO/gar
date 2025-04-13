#include "text_codec.hh"
#include <string>

std::string TextCodec::encode(std::string_view input)
{
	
}


std::string TextCodec::decode(std::string_view input)
{

}


TextCodec* TextCodec::getDefaultCodec()
{
	static TextCodec process_text;
	return &process_text;
}


// checks whether input data consists of UTF-8 text lines (each terminated by CRLF)
bool TextCodec::isText(std::string_view input)
{
	enum Status { ASCII, U4, U3, U2, CR };
	
	Status status = ASCII;
	for(char c : input)
	{
		const unsigned uc = (unsigned char)c;
		switch(status)
		{
			case ASCII:
			{
				switch(uc)
				{
					case '\r' : status = CR; continue;
					case '\t' : [[fallthrough]];
					case '\n' : [[fallthrough]];
					case ' ' ... '\x7f' : continue;
					case 0xC2 ... 0xDF  : status = U2; continue; // start of 2-octet UTF-8 sequence
					case 0xE0 ... 0xEF  : status = U3; continue; // start of 3-octet UTF-8 sequence
					case 0xF0 ... 0xF4  : status = U4; continue; // start of 4-octet UTF-8 sequence
					default: return false;
				}
			}
			case U4:
			{
				if(uc>=0x80 && uc<=0xBF)
				{
					status = U3;
					continue;
				}else{
					return false; // illegal UTF-8
				}
			}
			case U3:
			{
				if(uc>=0x80 && uc<=0xBF)
				{
					status = U2;
					continue;
				}else{
					return false; // illegal UTF-8
				}
			}
			case U2:
			{
				if(uc>=0x80 && uc<=0xBF)
				{
					status = ASCII;
					continue;
				}else{
					return false; // illegal UTF-8
				}
			}
			case CR:
				if(c == '\n')
					continue;
				else
					return false; // \r not followed by \n -> not text file!
		}
	}
	
	return status == ASCII;
}


const std::initializer_list< CodecPair >& getTextCodecs()
{
	static auto codec_text = TextCodec::getDefaultCodec();

	static const std::initializer_list<CodecPair> data =
	{
		{"txt",  codec_text}, // plain text
		{"text", codec_text}, // plain text
		{"c",    codec_text}, // C source
		{"h",    codec_text}, // C header
		{"cc",   codec_text}, // C++ source
		{"hh",   codec_text}, // C++ header
		{"cpp",  codec_text}, // C++ source
		{"hpp",  codec_text}, // C++ header
		{"cxx",  codec_text}, // C++ source
		{"hxx",  codec_text}, // C++ header
		{"c++",  codec_text}, // C++ source
		{"h++",  codec_text}, // C++ header
		
		// everything else in alphabetical order:
		{"bas",  codec_text}, // Basic source
		{"c#",   codec_text}, // C# source
		{"cs",   codec_text}, // C# source
		{"java", codec_text}, // Java source
		{"js",   codec_text}, // JavaScript source
		{"pas",  codec_text}, // Pascal source
		{"php",  codec_text}, // PHP source
		{"pl",   codec_text}, // Perl source
		{"py",   codec_text}, // Python source
		{"rb",   codec_text}, // Ruby source
	};
	
	return data;
}
