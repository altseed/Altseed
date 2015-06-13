
#pragma once

#include "asd.CorePostEffect.h"

namespace asd
{
	class CorePostEffect_Imp
		: public CorePostEffect
		, public ReferenceObject
	{
		friend class ObjectSystemFactory_Imp;

	private:
		
		std::vector<std::shared_ptr<Material2DCommand>>	m_commands;

		CorePostEffect_Imp(Graphics* graphics);
		virtual ~CorePostEffect_Imp();
	public:

		void DrawOnTexture2DWithMaterial(RenderTexture2D* target, Material2D* material);

		std::vector<std::shared_ptr<Material2DCommand>>& GetCommands() { return m_commands; }
		void ClearCommands(){ m_commands.clear(); }
#if !SWIG
	public:
		const char* GetGrayScaleShader(GraphicsDeviceType graphicsDeviceType) const override;
		const char* GetLightBloomShader(GraphicsDeviceType graphicsDeviceType) const override;
		const char* GetSepiaShader(GraphicsDeviceType graphicsDeviceType) const override;
		const char* GetGaussianBlurShader(GraphicsDeviceType graphicsDeviceType) const override;
		const char* GetDownsampleShader(GraphicsDeviceType graphicsDeviceType) const override;

		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif
	};
}