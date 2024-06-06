#include <gtest/gtest.h>
#include "siso93.hh"
#include <vector>

namespace
{
	struct TestTriple
	{
		std::string input;
		std::string output_tag;
		std::string output_data;
	};
	
	std::ostream& operator<<(std::ostream& o, const TestTriple& tp)
	{
		return o << "input=«" << tp.input << "», output=«" << tp.output_tag << "»|«" << tp.output_data << "»";
	}

const char nullo[5] = {0,0,0,0,0};

const std::vector<TestTriple> testValues =
	{
		{ ""                   , "", ""       },  // always start with the simple case ;-)
		{ "123"                , "`", "123"   },  // some ASCII digits. Still easy.
		{ "\n\\\b"             , "l", "J!H"   },  // backslash escapes for ASCII and control chars
		{ "\u001F"             , "'", "/"  },  // C compiler knows \x##, but JSON does not
		{ "\u007F"             , "'", "?"  },  // C compiler knows \x##, but JSON does not
		
		{ "äöü"           , "%3", "C$C6C1"  },  // German umlauts from Unicode block "Latin-1 Supplement"
		{ "Москва"        , "=#%", "P|P3QaP:P2P0"},  // some Cyrillic
		{ "\"\xF0\x9f\x92\xA3\"", "p3", "*p.r#*"  }, // Unicode Bomb <U+1F4A3>, an example for char outside of BMP
		
		{ std::string(nullo, nullo+1) , "'", "@"  },  // 1 NUL byte
		{ std::string(nullo, nullo+2) , "c", "@@"  },  // 2 NUL bytes
		{ std::string(nullo, nullo+3) , "l", "@@@"  },  // 3 NUL bytes
		{ std::string(nullo, nullo+4) , "m", "@@@@"  },  // 4 NUL bytes
		{ std::string(nullo, nullo+5) , "m'", "@@@@@"  }  // 5 NUL bytes
	};
}

class Siso93Test : public ::testing::TestWithParam<TestTriple>
{
	// intentionally left blank
};


INSTANTIATE_TEST_CASE_P(Siso93TestInstance, Siso93Test, testing::ValuesIn(testValues) );


TEST_P( Siso93Test, Encoder )
{
	const auto param = GetParam();
	const auto out = encode_siso93(param.input);
	
	EXPECT_EQ(param.output_tag, out.first);
	EXPECT_EQ(param.output_data, out.second);
}

TEST_P( Siso93Test, Decoder )
{
	const auto param = GetParam();
	const auto out = encode_siso93(param.input);
	const auto dec = decode_siso93(out.first, out.second);
	
	EXPECT_EQ(param.input, dec);
}
