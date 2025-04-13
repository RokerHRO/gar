#include "gar.hh"
#include <string>


std::string slurp(const std::string_view filename)
{
    std::ifstream input(filename.c_str(), std::ios_base::binary);
    if(!input)
    {
        throw std::runtime_error("Cannot read file \"" + filename + "\"! ");
    }
    
    std::stringstream sstr;
    sstr << input.rdbuf();
    return sstr.str();
}
