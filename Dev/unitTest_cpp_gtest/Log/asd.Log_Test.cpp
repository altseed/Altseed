#include <iostream>
#include <Log/asd.Log_Imp.h>
#include<gtest/gtest.h>

// Dev/unitTest_cpp/msvc/log_test.html に出力されます
void log_test()
{
	asd::Log* log = asd::Log_Imp::Create(u"log_test.htmu", u"テスト");

	ASSERT_FALSE( log == nullptr );

	log->ChangeColumn();
	log->Write(u"文字列");
	log->WriteLine(u"文字列＋改行");
	log->WriteLine(u"<文字列>");
	log->WriteHeading(u"ヘッダ");
	log->WriteLineStrongly(u"強調文字列");
	log->WriteHorizontalRule();
	log->BeginTable();
	log->Write(u"セル１");
	log->ChangeColumn();
	log->Write(u"セル２(ChangeColumn)");
	log->ChangeRow();
	log->Write(u"セル３(ChangeRow)");
	log->EndTable();

	log->SetOutputLevel(asd::LogLevel::Warning);
	log->WriteLine(u"表示されるログ(critical)", asd::LogLevel::Critical);
	log->WriteLine(u"表示されないログ(information)", asd::LogLevel::Information);
	delete log;
}

TEST(Log, LogOutput)
{
	log_test();
}
