#include<gtest/gtest.h>
#include<asd.common.Base.h>
#include<string>

// TODO: asd.common.Base_test.hへ移行

TEST(Utility, ToU16StringTest) {
#if _WIN32
	using namespace std::string_literals;
	ASSERT_EQ(asd::ToU16String(42), u"42"s);
	ASSERT_EQ(asd::ToU16String(31337L), u"31337"s);
	ASSERT_EQ(asd::ToU16String(3.14).substr(0, 4), u"3.14"s); // compare only first 4 chars because we don't know the number of zeros trailing
	ASSERT_EQ(asd::ToU16String(3.14f).substr(0, 4), u"3.14"s);

#elif __APPLE__
	using namespace std::string_literals;
	ASSERT_EQ(asd::ToU16String(42), u"42"s);
	ASSERT_EQ(asd::ToU16String(31337L), u"31337"s);
	ASSERT_EQ(asd::ToU16String(3.14).substr(0, 4), u"3.14"s); // compare only first 4 chars because we don't know the number of zeros trailing
	ASSERT_EQ(asd::ToU16String(3.14f).substr(0, 4), u"3.14"s);

#else
	ASSERT_EQ(asd::ToU16String(42), u"42");
	ASSERT_EQ(asd::ToU16String(31337L), u"31337");
	ASSERT_EQ(asd::ToU16String(3.14).substr(0, 4), u"3.14"); // compare only first 4 chars because we don't know the number of zeros trailing
	ASSERT_EQ(asd::ToU16String(3.14f).substr(0, 4), u"3.14");

#endif
   }
