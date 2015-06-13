#pragma once
#include <list>
#include "../asd.ReferenceObject.h"

namespace asd
{
	/**
		@brief	プログラムの実行状況(計算時間や、使っているプロセッサ)を記録するクラス。
	*/
	class Profiler : public ReferenceObject
	{
	protected:
		Profiler(){}
		virtual ~Profiler(){}

	public:
		/**
			@brief	計算の開始時間と、使用するプロセッサ番号を記録する。
			@param	id		プロファイリングを識別するID。Start関数は、IDの一致するEnd関数と対応する。
		*/
		virtual void Start(int id) = 0;

		/**
			@brief	計算の終了時間を記録し、プロファイリング結果を蓄積する。
			@param	id		プロファイリングを識別するID。End関数は、IDの一致するStart関数と対応する。
		*/
		virtual void End(int id) = 0;
	};
}