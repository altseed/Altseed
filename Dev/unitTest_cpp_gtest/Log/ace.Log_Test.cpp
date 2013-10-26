#include <iostream>
#include <Log/ace.Log_Imp.h>
#include<gtest/gtest.h>

// Dev/unitTest_cpp/msvc/log_test.html に出力されます
void log_test()
{
	ace::Log* log = ace::Log_Imp::Create(L"log_test.html", L"テスト");

	ASSERT_FALSE( log == nullptr );

	log->ChangeColumn();
	log->Write(L"文字列");
	log->WriteLine(L"文字列＋改行");
	log->WriteLine(L"<文字列>");
	log->WriteHeading(L"ヘッダ");
	log->WriteLineStrongly(L"強調文字列");
	log->WriteHorizontalRule();
	log->BeginTable();
	log->Write(L"セル１");
	log->ChangeColumn();
	log->Write(L"セル２(ChangeColumn)");
	log->ChangeRow();
	log->Write(L"セル３(ChangeRow)");
	log->EndTable();

	log->SetOutputLevel(ace::Log::LOG_WARNING);
	log->WriteLine(L"表示されるログ(critical)", ace::Log::LOG_CRITICAL);
	log->WriteLine(L"表示されないログ(information)", ace::Log::LOG_INFORMATION);
	delete log;
}

TEST(Log, LogOutput)
{
	log_test();
}
