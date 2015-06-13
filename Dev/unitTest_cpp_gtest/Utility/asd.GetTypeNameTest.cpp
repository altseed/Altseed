#include<Utility/asd.GetTypeName.h>
#include<gtest/gtest.h>

class Class
{
public:
	int i;
};

TEST(Utility, GetTypeNameTest)
{
	EXPECT_TRUE(asd::GetTypeName<int>() == std::string("int"));
	auto f = 42.0f;
	EXPECT_TRUE(asd::GetTypeName<decltype(f)>() == std::string("float"));

	/*
	environment-depended
	Class c;
	EXPECT_TRUE(asd::GetTypeName<decltype(c)>() == std::string("Class")) << "expected: Class, actual:" << asd::GetTypeName<decltype(c)>();

	*/
}
/* // 
TEST(GetTypeNameTest, ComplexTypeTest)
{
	EXPECT_TRUE(asd::GetTypeName<std::vector<int> >() == std::string("std::vector<int>"));
}
*/
