
#pragma once

#include "ace.MassModel.h"

#include <Graphics/3D/ace.MassModel_IO.h>

namespace ace
{
	class MassModel_Imp
		: public MassModel
		, public ReferenceObject
	{
	public:
		struct Material
		{
			std::shared_ptr<Texture2D>	ColorTexture;
			std::shared_ptr<Texture2D>	NormalTexture;
			std::shared_ptr<Texture2D>	SpecularTexture;
			std::shared_ptr<Texture2D>	SmoothnessTexture;

			std::shared_ptr<Material3D>	Material_;

			Material();
			~Material();
		};

	private:
		int32_t					frameSkip = 1;
		std::vector<int32_t>	frameCount;

		std::map<astring, int32_t>	animationClips;

		std::shared_ptr<VertexBuffer_Imp>	m_vertexBuffer;
		std::shared_ptr<IndexBuffer_Imp>	m_indexBuffer;

		std::shared_ptr<Texture2D>		m_animationTexture;

		Material				material;

	public:
		MassModel_Imp();
		virtual ~MassModel_Imp();

		void SetMaterial(Material3D* material) override;

		std::shared_ptr<VertexBuffer_Imp> GetVertexBuffer() { return m_vertexBuffer; }
		std::shared_ptr<IndexBuffer_Imp> GetIndexBuffer() { return m_indexBuffer; }
		std::shared_ptr<Texture2D> GetAnimationTexture() { return m_animationTexture; }
		Material& GetMaterial() { return material; }

		bool Load(Graphics_Imp* g, MassModel_IO& io);

		int32_t GetFrameSkip() { return frameSkip; }
		int32_t GetFrameCount(int32_t index) { return frameCount[index]; }
		int32_t GetClipIndex(const achar* name)
		{
			auto key = astring(name);
			auto it = animationClips.find(key);

			if (it != animationClips.end())
			{
				return (*it).second;
			}
			return -1;
		}

		// IReferenceを継承したデバイスオブジェクト向け定義
#if !SWIG
	public:
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif
	};
}