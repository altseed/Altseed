﻿
#include <Base.h>

/**
@brief	文字列を回転させて表示するサンプル。
*/
void TextObject2D_Rotate()
{
	// Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("TextObject2D_Rotate").c_str(), 640, 480, asd::EngineOption());

	{
		// フォントと文字列描画オブジェクトの設定を行う。
		auto edgeFont = asd::Engine::GetGraphics()->CreateFont(asd::ToAString("Data/Font/Font1.aff").c_str());
		auto edgeObj = std::make_shared<asd::TextObject2D>();
		edgeObj->SetFont(edgeFont);
		edgeObj->SetPosition(asd::Vector2DF(100, 100));

		//回転角と描画する文字列を設定する。
		edgeObj->SetAngle(30);
		edgeObj->SetText(asd::ToAString("文字列の回転描画").c_str());

		// 文字描画オブジェクトのインスタンスをエンジンへ追加する。
		asd::Engine::AddObject2D(edgeObj);
	}

	// Altseedのウインドウが閉じられていないか確認する。
	while (asd::Engine::DoEvents())
	{
		// Altseedを更新する。
		asd::Engine::Update();
	}

	// Altseedを終了する。
	asd::Engine::Terminate();
}