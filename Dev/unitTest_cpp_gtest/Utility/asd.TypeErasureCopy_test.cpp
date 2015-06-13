#include<gtest/gtest.h>
#include <Utility/asd.TypeErasureCopy.h>
namespace
{

struct S1{
	int a;
	int b;
	char c;
	int d;
};

struct S2{
	int d;
	int e;
	char f;
	int g;
};

}
TEST(Utility, TypeErasureCopy){
	S1 s1 = {1, 2, 3, 4};
	S2 s2;
	asd::TypeErasureCopy<S1, S2>(&s1, &s2);
	EXPECT_EQ(s2.f, 3);
}
