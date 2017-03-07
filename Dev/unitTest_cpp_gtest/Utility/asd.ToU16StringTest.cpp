#include<gtest/gtest.h>
#include<Utility/asd.ToU16String.h>
#include<string>

TEST(Utility, ToU16StringTest) {
	using namespace std::string_literals;
	ASSERT_EQ(asd::ToU16String(42), u"42"s);
	ASSERT_EQ(asd::ToU16String(31337L), u"31337"s);
	ASSERT_EQ(asd::ToU16String(3.14).substr(0, 4), u"3.14"s); // compare only first 4 chars because we don't know the number of zeros trailing
	ASSERT_EQ(asd::ToU16String(3.14f).substr(0, 4), u"3.14"s);

}