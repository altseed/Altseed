
#include "../PCH/asd.UnitTestCpp.PCH.h"

#include <Window/asd.Window_Imp.h>

//#define _PRINTLOG 1

TEST(Window, Empty)
{
#ifdef _PRINTLOG
	printf("-Create\n");
#endif

	auto window = asd::Window_Imp::Create(640, 480, asd::ToAString(L"Test Window").c_str(), NULL, asd::WindowPositionType::Default, asd::GraphicsDeviceType::Default, asd::ColorSpaceType::LinearSpace, false);
#ifdef _PRINTLOG
	if( window != nullptr )
	{
		printf("-Create Succeed.\n");
	}
	else
	{
		printf("-Create Failed.\n");
	}
#endif
	ASSERT_TRUE(window != nullptr);

	int counter = 0;

	while( window->DoEvent() )
	{
		if( counter == 10 )
		{
			window->SetTitle( asd::ToAString(L"0.1 minites has passed.").c_str() );
#ifdef _PRINTLOG
	printf("-SetTitle 0\n");
#endif
		}

		if( counter == 20 )
		{
			window->SetTitle( asd::ToAString(L"0.2秒経過.").c_str() );
#ifdef _PRINTLOG
	printf("-SetTitle 1\n");
#endif
		}

		if( counter == 30 )
		{
			window->Close();
#ifdef _PRINTLOG
	printf("-Close\n");
#endif
		}

		glfwSwapBuffers((GLFWwindow *)(window->GetWindow()->GetNativeWindow()));
		asd::Sleep(1);
		counter++;
	}

	window->Release();
#ifdef _PRINTLOG
	printf("-delete\n");
#endif
}
