
#include "../ace.Graphics_Imp.h"
#include "ace.ShaderCache.h"
#include "ace.NativeShader_Imp.h"

namespace ace
{
	struct ShaderCacheReferenceDeleter
	{
		void operator ()(NativeShader_Imp* p)
		{
			if (p->GetRef() == 1)
			{
				auto g = (Graphics_Imp*)p->GetGraphics();
				g->GetShaderCache()->RemoveShader(p);
			}
			SafeRelease(p);
		}
	};

	ShaderCache::ShaderCache(Graphics_Imp* graphics)
		: m_graphics(graphics)
	{
	}

	ShaderCache::~ShaderCache()
	{

	}

	std::shared_ptr<NativeShader_Imp> ShaderCache::CreateFromCode(
		const achar* key,
		const char* vertexShaderCode,
		const char* pixelShaderCode,
		std::vector <VertexLayout>& layout,
		std::vector <Macro>& macro)
	{
		NativeShader_Imp* shader = nullptr;

		auto it = m_shaders.find(key);
		if (it != m_shaders.end())
		{
			shader = it->second;
			SafeAddRef(shader);
		}
		else
		{
			shader = m_graphics->CreateShader_Imp_(
				vertexShaderCode,
				ToUtf8String(key).c_str(),
				pixelShaderCode,
				ToUtf8String(key).c_str(),
				layout,
				macro);

			m_shaders[key] = shader;
			shader->SetKey(key);
		}
		
		if (shader == nullptr) return nullptr;

		return std::shared_ptr<NativeShader_Imp>(shader, ShaderCacheReferenceDeleter());
	}

	void ShaderCache::RemoveShader(NativeShader_Imp* shader)
	{
		assert(shader != nullptr);
		assert(shader->GetRef() == 1);
		m_shaders.erase(shader->GetKey());
	}
}