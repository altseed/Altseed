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
	
	class GraphicsResourceFile;
	class GraphicsResourceFileObserver;
	class GraphicsResourceContainer;

	class GraphicsResourceFile
	{
	private:
		time_t	m_modifiedTime;
		astring	m_path;
		GraphicsResourceContainer*	m_container;
		std::set<GraphicsResourceFileObserver*>	m_observers;
	public:
		GraphicsResourceFile(GraphicsResourceContainer* container, const achar* path, GraphicsResourceFileObserver* observer);
		virtual ~GraphicsResourceFile();

		void Reload(bool fourced);
		void Attach(GraphicsResourceFileObserver* observer);
		void Detach(GraphicsResourceFileObserver* observer);

		const achar* GetPath(){ return m_path.c_str(); }
	};

	class GraphicsResourceFileObserver
	{
	public:
		GraphicsResourceFileObserver(){}
		virtual ~GraphicsResourceFileObserver(){}
		virtual void Update(GraphicsResourceFile* file) = 0;
	};
	
	class GraphicsTexture2DFileObserver
		: public GraphicsResourceFileObserver
	{
	private:
		Texture2D_Imp*		m_texture;

	public:
		GraphicsTexture2DFileObserver(Texture2D_Imp* texture);
		virtual ~GraphicsTexture2DFileObserver();
		void Update(GraphicsResourceFile* file) override;
	};

	/**
		@brief	描画リソースを格納するクラス
	*/
	class GraphicsResourceContainer
	{
	private:
		std::map<astring, Texture2D_Imp*>	m_keyToTexture2Ds;
		std::map<Texture2D_Imp*,astring>	m_texture2DToKeys;
		std::map < Texture2D_Imp*, std::shared_ptr < Texture2DReloadInformation> >	m_texture2DReloadInfo;

		std::set<GraphicsResourceFile*>		m_resourceFiles;

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
		static time_t GetModifiedTime(const achar* path);

		void Reload();

		void AddResourceFile(GraphicsResourceFile* file);
		void RemoveResourceFile(GraphicsResourceFile* file);

	};
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

}
