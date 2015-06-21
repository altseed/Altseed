
#include <stdio.h>
#include <string.h>
#include "SampleGuide.h"
using namespace std;

void DrawLine()
{
	cout << "----------------------------------------" << endl;
}

void SampleGuide::Show()
{
	while (true)
	{
#if _WIN32
		system("cls");
#else
		system("clear");
#endif
		ShowTitles();
		int number = InputSampleNumber();

		if (number == m_exitIndex)
		{
			break;
		}

		bool run = EnsureToRun(number);
		if (run)
		{
			m_samples[number].func();
#if _WIN32
			system("pause");
#endif
		}
	}
}

void SampleGuide::ShowTitles()
{
	cout << "実行するサンプルプログラムの番号を入力してください。" << endl;
	DrawLine();
	for (int i = 0; i < m_exitIndex; i++)
	{
		cout << "[" << i << "] " << m_samples[i].title.c_str() << endl;
	}
	cout << "[" << m_exitIndex << "] 終了" << endl;
	DrawLine();
}

int SampleGuide::InputSampleNumber()
{
	const int DISABLE = -1;

	while (true)
	{
		int result = DISABLE;
		cout << "> " << flush;
		cin >> result;

		if (cin.fail())
		{
			cin.clear();
			cin.ignore( 100, '\n' );
			continue;
		}

		if (0 <= result && result <= m_exitIndex)
		{
			DrawLine();
			return result;
		}
	}
}

bool SampleGuide::EnsureToRun(int number)
{
	char responce[8];
	while (true)
	{
		cout << m_samples[number].title.c_str() << endl;
		cout << "実行しますか？(y/n) > " << flush;
		cin >> responce;

		if (strcmp(responce, "y") == 0)
		{
			return true;
		}
		else if (strcmp(responce, "n") == 0)
		{
			return false;
		}
	}
}


