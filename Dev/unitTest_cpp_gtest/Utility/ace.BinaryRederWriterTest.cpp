#include<gtest/gtest.h>
#include<fstream>
#include<vector>
#include<cassert>
#include<cstdint>
#include<ace.common.Base.h>
#include<Utility/ace.BinaryReader.h>
#include<Utility/ace.BinaryWriter.h>


class BinaryReaderWriterTest : public ::testing::Test
{
protected:
	std::ifstream ifs;
	ace::BinaryWriter writer;
	ace::BinaryReader reader;
	std::vector<int8_t> dat;
	BinaryReaderWriterTest() : ifs("binaryTest.dat", std::ios_base::in | std::ios_base::binary)
	{

	}

	virtual ~BinaryReaderWriterTest()
	{

	}


	virtual void SetUp()
	{

	}

	virtual void TearDown()
	{

	}
	void ReadIn()
	{
		char ch;
		while (ifs.get(ch))
		{
			dat.push_back(*(static_cast<int8_t*>(static_cast<void*>(&ch))));
		}
		ifs.close();
		ASSERT_FALSE(dat.size() <= 1);
		reader.ReadIn(dat.begin(), dat.end());
	}
	void WriteOut()
	{
		ASSERT_TRUE(writer.WriteOut("binaryTest.dat", false));

	}


	// ここで宣言されるオブジェクトは，テストケース内の全てのテストで利用できます．
};


TEST_F(BinaryReaderWriterTest, int8Test)
{
	std::vector<int8_t> orig = { 1, 2, 3, 42 };
	for (auto const e : orig)
	{
		writer.Push(e);
	}
	WriteOut();
	ReadIn();

	for (auto const e : orig)
	{
		ASSERT_FALSE(reader.IsEmpty());
		EXPECT_TRUE(e == reader.Get<int8_t>());
	}
}

TEST_F(BinaryReaderWriterTest, int16Test)
{
	std::vector<int16_t> orig = { 0xACE, 0x1234, 0x7777, 0x1, 0x4649 };
	for (auto const e : orig)
	{
		writer.Push(e);
	}
	WriteOut();
	ReadIn();

	for (auto const e : orig)
	{
		ASSERT_FALSE(reader.IsEmpty());
		EXPECT_TRUE(e == reader.Get<int16_t>());
	}
}

TEST_F(BinaryReaderWriterTest, int32Test)
{
	std::vector<int32_t> orig = { 0x12345678, 0x77777777, 0x31337, 42 };
	for (auto const e : orig)
	{
		writer.Push(e);
	}
	WriteOut();
	ReadIn();

	for (auto const e : orig)
	{
		ASSERT_FALSE(reader.IsEmpty());
		EXPECT_TRUE(e == reader.Get<int32_t>());
	}
}

TEST_F(BinaryReaderWriterTest, stdstringTest)
{
	std::vector<std::string> orig = { "This", "is", "a", "test." };
	for (auto const& e : orig)
	{
		writer.Push(e);
	}
	WriteOut();
	ReadIn();

	for (auto const e : orig)
	{
		ASSERT_FALSE(reader.IsEmpty());
		EXPECT_TRUE(e == reader.Get<std::string>());
	}
}



TEST_F(BinaryReaderWriterTest, acharptrTest)
{
	std::vector<ace::achar const*> orig = { ace::ToAString(L"これも").c_str(), ace::ToAString(L"テスト").c_str(), ace::ToAString(L"です").c_str() };

	for (auto const e : orig)
	{
		writer.Push(e);
	}
	WriteOut();
	ReadIn();
	for (auto const e : orig)
	{
		ASSERT_FALSE(reader.IsEmpty());
		EXPECT_EQ(ace::ToUtf8String(e), ace::ToUtf8String(reader.Get<ace::achar*>()));
	}

}

TEST_F(BinaryReaderWriterTest, astringTest)
{
	std::vector<ace::astring> orig = { ace::ToAString(L"これは"), ace::ToAString(L"テスト"), ace::ToAString(L"です") };
	for (auto const& e : orig)
	{
		writer.Push(e);
	}
	WriteOut();
	ReadIn();

	for (auto const e : orig)
	{
		ASSERT_FALSE(reader.IsEmpty());
		EXPECT_TRUE(e == reader.Get<ace::astring>());
	}
}

TEST_F(BinaryReaderWriterTest, mixedTypeTest)
{
	int32_t i = 0x1234ABCD;
	int16_t sh = 0x1234;
	int8_t c = 0x12;

	ace::astring astr(ace::ToAString(L"これはテストです"));
	std::string str("This is a test");
	ace::achar* achs;

	writer.Reserve(42);

	EXPECT_TRUE(writer.Get().capacity() >= 42);

	writer.Push(i);
	writer.Push(sh);
	writer.Push(c);
	writer.Push(astr);
	writer.Push(astr.c_str());
	writer.Push(str);

	WriteOut();
	ReadIn();



	i = reader.Get<int32_t>();
	EXPECT_TRUE(i == 0x1234ABCD);


	sh = reader.Get<int16_t>();
	EXPECT_TRUE(sh == 0x1234);


	c = reader.Get<int8_t>();
	EXPECT_TRUE(c == 0x12);


	astr = reader.Get<ace::astring>();
	EXPECT_TRUE(astr == ace::ToAString(L"これはテストです"));


	achs = reader.Get<ace::achar*>();
	EXPECT_TRUE(ace::astring(achs) == ace::ToAString(L"これはテストです"));




	str = reader.Get<std::string>();
	EXPECT_TRUE(str == std::string("This is a test"));


}