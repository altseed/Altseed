#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "../../ace.Core.Base_Imp.h"
#include "ace.Graphics.h"

#include <time.h>

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	/**
		@brief	描画リソースを格納するクラス
	*/
	class GraphicsResourceContainer
	{
	private:
		std::map<astring, Texture2D_Imp*>	m_keyToTexture2Ds;
		std::map<Texture2D_Imp*,astring>	m_texture2DToKeys;
		std::map < Texture2D_Imp*, std::shared_ptr < Texture2DReloadInformation> >	m_texture2DReloadInfo;

	public:

		GraphicsResourceContainer();
		~GraphicsResourceContainer();

		Texture2D_Imp* GetTexture2D(const achar* key);
		void RegistTexture2D(const achar* key, std::shared_ptr <Texture2DReloadInformation> info, Texture2D_Imp* texture);
		void UnregistTexture2D(Texture2D_Imp* texture);

		/**
			@brief	ファイル更新時間を取得する。
			@param	path	パス
		*/
		time_t GetModifiedTime(const achar* path);

		void Reload();
	};
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

}
