
#include "PCH/asd.UnitTestCpp.PCH.h"

#ifdef __llvm__
#pragma clang diagnostic ignored "-Wc++11-narrowing"
#endif
#ifdef _WIN32
#pragma warning( disable: 4309 )
#endif

using namespace std;

TEST(CharSet, Utf16ToUtf8)
{
	// "UTF16文字列"
	int16_t source [] = { 0x55, 0x54, 0x46, 0x31, 0x36, 0x6587, 0x5b57, 0x5217, 0x00 };

	std::vector<int8_t> actual;
	asd::Utf16ToUtf8(actual, source);

	int8_t expected [] = {
		0x55, 0x54, 0x46, 0x31, 0x36,
		0xe6, 0x96, 0x87, 0xe5, 0xad, 0x97, 0xe5, 0x88, 0x97, 0x00 };

	ASSERT_EQ(
		basic_string<int8_t>(expected),
		basic_string<int8_t>(actual.data()));
}

TEST(CharSet, Utf8ToUtf16)
{
	// "UTF8文字列"
	int8_t source [] = {
		0x55, 0x54, 0x46, 0x38,
		0xe6, 0x96, 0x87, 0xe5, 0xad, 0x97, 0xe5, 0x88, 0x97, 0x00 };

	std::vector<int16_t> actual;
	asd::Utf8ToUtf16(actual, source);

	int16_t expected [] = { 0x55, 0x54, 0x46, 0x38, 0x6587, 0x5b57, 0x5217, 0 };

	ASSERT_EQ(
		basic_string<int16_t>(expected),
		basic_string<int16_t>(actual.data()));
}

TEST(CharSet, ToWide)
{
	auto expected = wstring(L"文字列のWide化");

	wstring actual = asd::ToWide("文字列のWide化");

	ASSERT_EQ(expected, actual);
}

TEST(CharSet, ToAString_wchar)
{
#if _WIN32
	auto actual = asd::ToAString(L"文字列のAString化");

	// windowsでは achar は wchar_t なので
	ASSERT_EQ(
		asd::astring(L"文字列のAString化"),
		actual);
#else
	auto actual = asd::ToAString(L"文字列のAString化");

	// *nixでは achar は char16_t
	ASSERT_EQ(
		asd::astring(u"文字列のAString化"),
		actual);
#endif
}

TEST(CharSet, ToAString_char)
{
#if _WIN32
	auto actual = asd::ToAString("文字列のAString化");

	// windowsでは achar は wchar_t なので
	ASSERT_EQ(
		asd::astring(L"文字列のAString化"),
		actual);
#else
	auto actual = asd::ToAString("文字列のAString化");

	// *nixでは achar は char16_t
	ASSERT_EQ(
		asd::astring(u"文字列のAString化"),
		actual);
#endif
}

TEST(CharSet, ToUtf8String)
{
	asd::astring str = asd::ToAString("文字列のUTF8化");
	auto actual = asd::ToUtf8String(str.c_str());

	int8_t expected [] = {
		0xe6, 0x96, 0x87, 0xe5, 0xad, 0x97, 0xe5, 0x88, 0x97,
		0xe3, 0x81, 0xae,
		0x55, 0x54, 0x46, 0x38,
		0xe5, 0x8c, 0x96,
		0x00
	};

	ASSERT_EQ(
		basic_string<char>((const char*)expected),
		actual);
}


void ReplaceAll_paramTest(
	const wchar_t* source,
	const wchar_t* from,
	const wchar_t* to,
	const wchar_t* expected)
{
	auto expectedString = asd::ToAString(expected);
	auto actual = asd::ReplaceAll(
		asd::ToAString(source),
		asd::ToAString(from).c_str(),
		asd::ToAString(to).c_str());

	ASSERT_EQ(expectedString, actual);
}

TEST(CharSet, ReplaceAll)
{
	{
		auto source = L"ReplaceAll関数";
		auto from = L"l";
		auto to = L"*";
		auto expected = L"Rep*aceA**関数";

		ReplaceAll_paramTest(source, from, to, expected);
	}

	{
		auto source = L"ReplaceAll関数";
		auto from = L"関";
		auto to = L"#";
		auto expected = L"ReplaceAll#数";

		ReplaceAll_paramTest(source, from, to, expected);
	}
}
