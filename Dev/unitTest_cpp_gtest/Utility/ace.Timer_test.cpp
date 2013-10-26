#include<cstdlib>
#include<gtest/gtest.h>
#include<Utility/ace.Timer.h>
#ifdef _WIN32
#include<Windows.h>
#else
#include <unistd.h>
#endif
TEST(Utility, Timer){
	int64_t prevTime = ace::GetTime();
	
	// wait a half seconds...
#ifdef _WIN32
	Sleep(500);
#else
	usleep(500000);
#endif
	int64_t currentTime = ace::GetTime();
	
	ASSERT_NEAR(currentTime - prevTime, 500000, 200000); // 0.2sec margin
}
