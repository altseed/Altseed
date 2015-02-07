
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
			std::shared_ptr<Texture2D>	MetalnessTexture;
			std::shared_ptr<Texture2D>	SmoothnessTexture;

			std::shared_ptr<Material3D>	Material_;

			Material();
			~Material();
		};

	private:
		std::vector<int32_t>	frameCount;
		std::vector<bool>		loopingMode;

		std::vector<astring>		AnimationClipNames;
		std::map<astring, int32_t>	animationClips;

		std::shared_ptr<VertexBuffer_Imp>	m_vertexBuffer;
		std::shared_ptr<IndexBuffer_Imp>	m_indexBuffer;

		std::shared_ptr<Texture2D>		m_animationTexture;

		Material				material;

	public:
		MassModel_Imp();
		virtual ~MassModel_Imp();

		int32_t GetAnimationCount() const { return AnimationClipNames.size(); }

		const achar* GetAnimationName(int32_t index) const override;

		float GetAnimationLength(const achar* name) const override;

		bool GetIsAnimationLoopingMode(const achar* name) const override;
		void SetIsAnimationLoopingMode(const achar* name, bool isLoopingMode) override;

		void SetMaterial(Material3D* material) override;

		std::shared_ptr<VertexBuffer_Imp> GetVertexBuffer() { return m_vertexBuffer; }
		std::shared_ptr<IndexBuffer_Imp> GetIndexBuffer() { return m_indexBuffer; }
		std::shared_ptr<Texture2D> GetAnimationTexture() { return m_animationTexture; }
		Material& GetMaterial() { return material; }

		bool Load(Graphics_Imp* g, MassModel_IO& io);

		bool GetIsLoopingMode(int32_t index) { return loopingMode[index]; }
		int32_t GetFrameCount(int32_t index) { return frameCount[index]; }
		int32_t GetClipIndex(const achar* name) const
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