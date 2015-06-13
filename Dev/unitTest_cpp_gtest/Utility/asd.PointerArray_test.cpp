#include<cstdlib>
#include<gtest/gtest.h>
#include<Utility/asd.PointerArray.h>

TEST(Utility, PointerArray){
	int* ip = (int*)malloc(sizeof(int) * 4);
	ASSERT_TRUE(ip != nullptr);
	auto ar = asd::PointerArray<int>(ip, 4);
	ar[0] = 1;
	ar[1] = 2;
	ar[2] = 3;
	ar[3] = 4;

	for(int i = 0; i < 4; i++) {
		EXPECT_EQ(ar[i],  i + 1);
	}
	free(ip);
}
TEST(UtilityDeathTest, PointerArrayAccessViolationTest){
	int* ip = (int*)malloc(sizeof(int) * 4);
	ASSERT_TRUE(ip != nullptr);
	auto ar = asd::PointerArray<int>(ip, 4);
	EXPECT_DEATH(ar[4] == 5, ".*"); // must fail
	free(ip);
}