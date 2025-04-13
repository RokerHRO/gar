#ifndef GAR_ROLLING_HASH_HH
#define GAR_ROLLING_HASH_HH

#include <cstdint>
#include <deque>
#include <string>
#include <string_view>


static constexpr
uint64_t Pow(uint64_t basis, unsigned exponent)
{
	return exponent ? Pow(basis, exponent-1) * basis : 1 ;
}


struct RollingHash
{
public:
	RollingHash(unsigned length=8)
	: MaxExpon{length}
	, Power{ Pow(257, MaxExpon) }
	, buffer{}
	, h{0}
	{ }
	
	RollingHash(unsigned length, const std::string_view& sv)
	: RollingHash(length)
	{
		for(char c : sv)
		{
			add(c);
		}
	}
	
	uint64_t add(char c)
	{
		if(buffer.size()>MaxExpon)
		{
			const uint64_t minus = (uint8_t(buffer[0]) + 1u) * Power;
			h -= minus;
			buffer.pop_front();
		}
		buffer.push_back(c);

		h = (h*257u + uint8_t(c) + 1u);
		return xrm(h);
	}
	
	uint64_t add(std::string_view sv)
	{
		for(char c:sv)
			add(c);
		
		return xrm(h);
	}
	
	constexpr
	uint64_t hash() const { return xrm(h); }
	
	std::string string() const
	{
		std::string s;
		for(char c : buffer)
		{
			s += c;
		}
		return s;
	}
	
private:
	const unsigned MaxExpon;
	const uint64_t Power;
	std::deque<uint8_t> buffer;
	uint64_t h;
	
	static constexpr
	uint64_t xrm(uint64_t u)
	{
		u ^= 0x727C9716FFB764D5; // 2**64 / sqrt(5)
		u  = (u>>41) | (u<<(64-41)); // bitrotate right by 41 bit.
		u *= 0x3ABAFD5229726D8D;  // 2**64 / sqrt(19)
		
		u ^= 0x2F89BACC7D77102C; // 2**64 / sqrt(29)
		u  = (u>>27) | (u<<(64-27)); // bitrotate right by 27 bit.
		u *= 0x2284DF580CA9B57B; // 2**64 / sqrt(55)
		
		return u;
	}
};

#endif // GAR_ROLLING_HASH_HH
