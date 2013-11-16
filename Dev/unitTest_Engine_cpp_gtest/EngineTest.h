#pragma once
#include <ace.h>

class EngineTest
{
private:
	int m_exitTime;
	bool m_isOpenGLMode;
	ace::astring m_title;

protected:
	/**
		@brief	オーバーライドして、メインループより前のアサーションや初期化を記述できる。
	*/
	virtual void OnStart();

	/**
		@brief	オーバーライドして、メインループ内のアサーションなどを記述できる。
	*/
	virtual void OnUpdating();

	/**
		@brief	オーバーライドして、メインループ内のアサーションなどを記述できる。
	*/
	virtual void OnUpdated();

	/**
		@brief	オーバーライドして、メインループの後のアサーションや終了処理を記述できる。
	*/
	virtual void OnFinish();

public:
	EngineTest(ace::astring title, bool isOpenGLMode, int exitTime);
	void Run();
};