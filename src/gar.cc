#include "gar.hh"

#include "bin_processor.hh"
#include "zip_processor.hh"

// map common well-known file extensions to appropriate FileProcessor
const std::map<std::string_view, FileProcessor>  extension2processor =
	{
		{"jar", process_zip}, // Java Archive
		{"zip", process_zip}, // guess what
		{"apk", process_zip}, // Android package
		{"epub", process_zip}, // ePUB
		
		{"odt", process_zip}, // OpenDocument "text"
		{"ods", process_zip}, // spreadsheet
		{"odp", process_zip}, // presentation
		{"odg", process_zip}, // drawing / "graphics"
		{"odf", process_zip}, // formula
		{"odc", process_zip}, // chart
		{"odi", process_zip}, // image
		{"odm", process_zip}, // "master"
		{"odb", process_zip}, // database
		
		{"ott", process_zip}, // OpenDocument templates:
		{"ots", process_zip},
		{"otp", process_zip},
		{"otg", process_zip},
		{"otc", process_zip},
		{"otf", process_zip},
		{"oti", process_zip},
		{"oth", process_zip},
		
		{"docx", process_zip}, // MS Word
		{"docm", process_zip},
		{"dotx", process_zip},
		{"dotm", process_zip},
		
		{"pptx", process_zip}, // MS PowerPoint
		{"pptm", process_zip},
		{"potx", process_zip},
		{"potm", process_zip},
		{"ppam", process_zip},
		{"ppsx", process_zip},
		{"ppsm", process_zip},
		{"sldx", process_zip},
		{"sldm", process_zip},
		{"thmx", process_zip},

		{"xlsx", process_zip}, // MS Excel
		{"xlsm", process_zip},
		{"xltx", process_zip},
		{"xltm", process_zip},

	}

// map file type (-t commandline option) o appropriate FileProcessor
const std::map<std::string_view, FileProcessor>  filetype2processor;
