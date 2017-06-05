
#pragma once

#include "../asd.Graphics_Imp.h"

namespace asd {

class EffekseerFile :
	public Effekseer::FileInterface
{
	Graphics_Imp*	graphics = nullptr;

public:
	EffekseerFile(Graphics_Imp* graphics);

	virtual ~EffekseerFile();

	Effekseer::FileReader* OpenRead(const EFK_CHAR* path);

	Effekseer::FileWriter* OpenWrite(const EFK_CHAR* path);
};

class EffectTextureLoader
	: public ::Effekseer::TextureLoader
{
protected:
	Graphics_Imp*				m_graphics = nullptr;
	Effekseer::TextureLoader*	originalTextureLoader = nullptr;

	struct Cache
	{
		bool IsDDS;
		int32_t Count;
		Effekseer::TextureData* Ptr;
		int32_t Width;
		int32_t Height;
	};
	std::map<astring, Cache>		m_caches;
	std::map<Effekseer::TextureData*, astring>		dataToKey;

public:

	EffectTextureLoader(Graphics_Imp* graphics, Effekseer::TextureLoader* originalTextureLoader);
	virtual ~EffectTextureLoader();

	Effekseer::TextureData* Load(const EFK_CHAR* path, Effekseer::TextureType textureType) override;
	void Unload(Effekseer::TextureData* data) override;
};

class EffectModelLoader
	: public ::Effekseer::ModelLoader
{
	Graphics_Imp*	m_graphics = nullptr;
	Effekseer::ModelLoader*	originalLoader = nullptr;

	struct Cache
	{
		int32_t Count;
		void* Ptr;
	};
	std::map<astring, Cache>		m_caches;
	std::map<void*, astring>		dataToKey;

public:
	EffectModelLoader(Graphics_Imp* graphics, Effekseer::ModelLoader* loader);
	virtual ~EffectModelLoader();

	void* Load(const EFK_CHAR* path) override;
	void Unload(void* data) override;
};

class EffectDistortingCallback
	: public ::EffekseerRenderer::DistortingCallback
{
public:
	bool	IsEnabled = false;
};

class EffekseerHelper
{
private:

public:
	static EffekseerRenderer::Renderer* CreateRenderer(Graphics* graphics, int32_t spriteCount);

	static Effekseer::TextureLoader* CreateTextureLoader(Graphics* graphics, Effekseer::FileInterface* fileInterface);

	static Effekseer::ModelLoader* CreateModelLoader(Graphics* graphics, Effekseer::FileInterface* fileInterface);

};

}