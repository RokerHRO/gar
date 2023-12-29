#ifndef GAR_HH_INCLUDED
#define GAR_HH_INCLUDED

#include <string_view>
#include <map>

using FileProcessor = void(*)(std::string_view filename);

// map common well-known file extensions to appropriate FileProcessor
extern const std::map<std::string_view, FileProcessor>  extension2processor;

// map file type (-t commandline option) o appropriate FileProcessor
extern const std::map<std::string_view, FileProcessor>  filetype2processor;


#endif // GAR_HH_INCLUDED
