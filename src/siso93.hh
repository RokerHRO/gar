#ifndef GAR_SISO93_HH
#define GAR_SISO93_HH

#include <string_view>
#include <string>

std::pair<std::string, std::string> encode_siso93(std::string_view s);

std::string decode_siso93(std::string_view tag, std::string_view data);

#endif // GAR_SISO93_HH
