#ifndef GAR_STREAM_SISO93_HH
#define GAR_STREAM_SISO93_HH

#include "stream_hase.hh"

class RollingHash;

class Siso93 : public StreamBase
{
public:
	Siso93(unsigned line_length_min, unsigned line_length_max);
	Siso93(const Siso93&) = delete;
	~Siso93();
	
	void encode(std::istream& in, std::ostream& out);
	void decode(std::istream& in, std::ostream& out);
private:
	RollingHash* rh;
};

#endif // GAR_STREAM_SISO93_HH
