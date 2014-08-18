#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "../ace.Core.Base_Imp.h"
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

		template<typename TARGET, typename INFO>
		class Resource
		{
			friend class GraphicsResourceContainer;

		private:
			std::map<astring, TARGET*>	m_keyToResource;
			std::map<TARGET*, astring>	m_resourceToKeys;
			std::map <TARGET*, std::shared_ptr < INFO> >	m_reloadInfo;
		public:
			
			TARGET* Get(const achar* key)
			{
				if (m_keyToResource.count(key) > 0)
				{
					return m_keyToResource[key];
				}

				return nullptr;
			}

			void Regist(const achar* key, std::shared_ptr <INFO> info, TARGET* target)
			{
				m_keyToResource[key] = target;
				m_resourceToKeys[target] = key;
				m_reloadInfo[target] = info;
			}

			void Unregist(TARGET* target)
			{
				m_keyToResource.erase(m_resourceToKeys[target]);
				m_resourceToKeys.erase(target);
				m_reloadInfo.erase(target);
			}
		};

	public:

		GraphicsResourceContainer();
		~GraphicsResourceContainer();

		Resource<Model_Imp, ModelReloadInformation>			Models;
		Resource<Font_Imp, FontReloadInformation>			Fonts;
		/**
			@brief	ファイル更新時間を取得する。
			@param	path	パス
		*/
		static time_t GetModifiedTime(const achar* path);

		void Reload();
	};
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

}
