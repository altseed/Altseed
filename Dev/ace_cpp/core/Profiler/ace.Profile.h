#pragma once
#include <memory>

namespace ace
{
	/**
		@brief	１回分のプロファイリングデータを表すクラス。
	*/
	class Performance
	{
	private:
		int m_startTime;
		int m_endTime;
		int m_processorNumber;

	public:
		Performance();

		/**
			@brief	このプロファイリングが開始された時間を取得する。
		*/
		int GetStartTime() const;

		/**
			@brief	このプロファイリングが終了した時間を取得する。
		*/
		int GetEndTime() const;

		/**
			@brief	このプロファイリングを開始したときに使用していたプロセッサを取得する。
		*/
		int GetProcessorNumber() const;

		/**
			@brief	このプロファイリングが開始された時間を設定する。
		*/
		void SetStartTime(int value);

		/**
			@brief	このプロファイリングが終了した時間を設定する。
		*/
		void SetEndTime(int value);

		/**
			@brief	このプロファイリングを開始したときに使用していたプロセッサを設定する。
		*/
		void SetProcessorNumber(int value);
	};

	/**
		@brief	１つのIDに対するプロファイルを管理するクラス。
	*/
	class Profile
	{
	public:
		typedef std::shared_ptr<Performance> PerfPtr;

	private:
		int m_id;
		PerfPtr m_last;
		PerfPtr m_current;

	public:
		Profile(int id);

		/**
			@brief	記録する対象のプロファイルデータを取得する。
		*/
		PerfPtr GetCurrent();

		/**
			@brief	現在記録している対象のプロファイルデータの編集を終了し、保持する。
		*/
		void Archive();

		int GetID() const;
		
#if !SWIG
		const PerfPtr GetLastLog();
#endif
	};

}