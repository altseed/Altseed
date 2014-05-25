
#pragma once

#include "PCH/ace.UnitTestEngineCpp.PCH.h"

class EngineTest
{
public:
	const int32_t WindowWidth = 640;
	const int32_t WindowHeight = 480;

private:
	const ace::astring directory;

	int m_exitTime;
	int m_currentTime;
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
	int32_t GetExitTime() { return m_exitTime; }
	int32_t GetTime() { return m_currentTime; }

};

extern void AssertMemoryDoesntLeak();

/**
	@brief	EngineTestクラスを継承したテスト クラスによるテストを実行する。
	@param	T				実行するテストクラスの型。引数に、OpenGLモードかどうかを表す真偽値を１つだけとるコンストラクタが必要である。
	@param	isOpenGLMode	OpenGLモードかどうか。
*/
template<class T>
extern void RunTest(bool isOpenGLMode)
{
	T(isOpenGLMode).Run();
	AssertMemoryDoesntLeak();
}