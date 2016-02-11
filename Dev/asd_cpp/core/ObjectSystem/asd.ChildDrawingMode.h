#pragma once

namespace asd
{
	/**
		@brief	子オブジェクトの描画に関する同期の設定を表すフラグを提供するクラス。
		*/
	class ChildDrawingMode
	{
	public:
		typedef int Flags;

		enum : Flags
		{
			/**
				@brief	何も同期しない。
			*/
			Nothing = 0,
			/**
				@brief	描画の際に親の色を子に乗算する。
			*/
			Color = 1,
			/**
				@brief	描画の際に親の描画優先度を子の描画優先度に足す。
			*/
			DrawingPriority = 2,
		};
	};
}
