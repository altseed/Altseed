#include<Utility/ace.GetTypeName.h>
#include<gtest/gtest.h>

class Class
{
public:
	int i;
};

TEST(Utility, GetTypeNameTest)
{
	EXPECT_TRUE(ace::GetTypeName<int>() == std::string("int"));
	auto f = 42.0f;
	EXPECT_TRUE(ace::GetTypeName<decltype(f)>() == std::string("float"));

	/*
	environment-depended
	Class c;
	EXPECT_TRUE(ace::GetTypeName<decltype(c)>() == std::string("Class")) << "expected: Class, actual:" << ace::GetTypeName<decltype(c)>();

	*/
}
/* // 
TEST(GetTypeNameTest, ComplexTypeTest)
{
	EXPECT_TRUE(ace::GetTypeName<std::vector<int> >() == std::string("std::vector<int>"));
}
*/
