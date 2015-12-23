#pragma once

namespace asd
{
	/**
		@brief	子オブジェクトの管理に関する同期の設定を表す値を提供するクラス。
	*/
	class ChildManagementMode
	{
	public:
		typedef int Flags;
		/**
			@brief	何も同期しない。
		*/
		static const Flags Nothing = 0;
		/**
			@brief	レイヤーへの登録状況を同期する。
		*/
		static const Flags RegistrationToLayer = 1;
		/**
			@brief	親オブジェクトが破棄されたとき、子オブジェクトも破棄する。
		*/
		static const Flags Vanishment = 2;
		/**
			@brief	親オブジェクトが更新されないとき、子オブジェクトも更新しない。
		*/
		static const Flags IsUpdated = 4;
		/**
			@brief	親オブジェクトが描画されないとき、子オブジェクトも描画されない。
		*/
		static const Flags IsDrawn = 8;
	};
}