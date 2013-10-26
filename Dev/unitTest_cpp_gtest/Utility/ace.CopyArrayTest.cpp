#include<gtest/gtest.h>
#include<Utility/ace.CopyArray.h>

TEST(Utility, CopyArray){
	int ar1[] = { 1, 2, 3 };
	int ar2[] = { 4, 5, 6 };
	double ar3 [] = { 10.0, 11.0, 12.0 };
	int *buf1 = new int[3];
	buf1[0] = 7;
	buf1[1] = 8;
	buf1[2] = 9;
	int *buf2 = new int[3];
	double* buf3 = new double[3];
	ace::CopyArray(ar2, ar1, 3 * sizeof(int)); /* array-to-array copy  */
	EXPECT_EQ(ar2[1], 2);
	ace::CopyArray(buf2, ar1, 3 * sizeof(int)); /* array-to-buffer copy  */
	EXPECT_EQ(buf2[1], 2);
	ace::CopyArray(ar2, buf1, 3 * sizeof(int)); /* buffer-to-array copy  */
	EXPECT_EQ(ar2[1], 8);
	ace::CopyArray(ar2, buf1, 3 * sizeof(int)); /* buffer-to-buffer copy  */
	EXPECT_EQ(ar2[1], 8);
	ace::CopyArray(buf3, ar3, 3 * sizeof(double)); /* buffer-to-buffer copy  */
	EXPECT_EQ(buf3[1], 11.0);

	delete[] buf1;
	delete[] buf2;
	delete[] buf3;
}
TEST(UtilityDeathTest, ArrayCopyAccessViolationTest){
	int ar1 [] = { 1, 2, 3 };
	int ar2 [] = { 4, 5, 6 };
	EXPECT_DEATH(ace::CopyArray(ar2, ar1, /**/5/**/ * sizeof(int)), ".*"); // tring to copy, but this code is in danger of buffer overflow
}
