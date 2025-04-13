#ifndef GAR_LINE_BREAKER_HH
#define GAR_LINE_BREAKER_HH

#include <cstdint>


static constexpr
uint64_t sqr(uint32_t u)
{
	return uint64_t{u}*u;
//	return u;
}


class LineBreaker
{
public:
	LineBreaker(unsigned min_length, unsigned max_length)
	: LineLengthMin(min_length)
	, LineLengthMax(max_length)
	, HashStep{UINT64_MAX / sqr(LineLengthMax - LineLengthMin)}
	{
//		fprintf(stderr, "# LineLen: %u ... %u\n HashStep: 0x%016llx.\n", LineLengthMin, LineLengthMax, (unsigned long long)HashStep);
		
		for(unsigned u=LineLengthMin-5; u<LineLengthMax+5; ++u)
		{
//			fprintf(stderr, "# Len: %u : HashStep: 0x%016llx.\n", u, (unsigned long long)break_hash(u) );
		}
	}

	uint64_t break_hash(unsigned length) const noexcept
	{
		if(length<=LineLengthMin) return 0;
		if(length>=LineLengthMax) return ~uint64_t(0);
		
		return sqr(length-LineLengthMin) * HashStep;
	}

private:
	const unsigned LineLengthMin;
	const unsigned LineLengthMax;
	const uint64_t HashStep;
};

#endif // GAR_LINE_BREAKER_HH
