#include "zip_codec.hh"
#include <string>

std::string ZipCodec::encode(std::string_view filename)
{

}


std::string ZipCodec::decode(std::string_view filename)
{

}


ZipCodec* ZipCodec::getDefaultCodec()
{
	static ZipCodec process_Zip;
	return &process_Zip;
}


const std::initializer_list< CodecPair >& getZipCodecs()
{
	static auto codec_zip = ZipCodec::getDefaultCodec();

	static const std::initializer_list<CodecPair> data =
	{
		{"jar", codec_zip}, // Java Archive
		{"zip", codec_zip}, // guess what
		{"apk", codec_zip}, // Android package
		{"epub", codec_zip}, // ePUB
		
		{"odt", codec_zip}, // OpenDocument "text"
		{"ods", codec_zip}, // spreadsheet
		{"odp", codec_zip}, // presentation
		{"odg", codec_zip}, // drawing / "graphics"
		{"odf", codec_zip}, // formula
		{"odc", codec_zip}, // chart
		{"odi", codec_zip}, // image
		{"odm", codec_zip}, // "master"
		{"odb", codec_zip}, // database
		
		{"ott", codec_zip}, // OpenDocument templates:
		{"ots", codec_zip},
		{"otp", codec_zip},
		{"otg", codec_zip},
		{"otc", codec_zip},
		{"otf", codec_zip},
		{"oti", codec_zip},
		{"oth", codec_zip},
		
		{"docx", codec_zip}, // MS Word
		{"docm", codec_zip},
		{"dotx", codec_zip},
		{"dotm", codec_zip},
		
		{"pptx", codec_zip}, // MS PowerPoint
		{"pptm", codec_zip},
		{"potx", codec_zip},
		{"potm", codec_zip},
		{"ppam", codec_zip},
		{"ppsx", codec_zip},
		{"ppsm", codec_zip},
		{"sldx", codec_zip},
		{"sldm", codec_zip},
		{"thmx", codec_zip},

		{"xlsx", codec_zip}, // MS Excel
		{"xlsm", codec_zip},
		{"xltx", codec_zip},
		{"xltm", codec_zip},
	};
	
	return data;
}
