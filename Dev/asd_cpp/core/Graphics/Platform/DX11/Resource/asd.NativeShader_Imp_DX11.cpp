
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "asd.NativeShader_Imp_DX11.h"

#include "../asd.Graphics_Imp_DX11.h"

#include "../../../../Log/asd.Log.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
NativeShader_Imp_DX11::NativeShader_Imp_DX11(
	Graphics* graphics, 
	ar::Shader* rhi,
	std::vector<ConstantLayout> vs_uniformLayouts,
	int32_t vs_uniformBufferSize,
	std::vector<TextureLayout> vs_textures,
	std::vector<ConstantLayout> ps_uniformLayouts,
	int32_t ps_uniformBufferSize,
	std::vector<TextureLayout> ps_textures)
	: NativeShader_Imp(graphics)
	, rhi(rhi)
{
	auto g = (Graphics_Imp_DX11*)graphics;

	int32_t index = 0;

	index = 0;
	for (auto& l : vs_uniformLayouts)
	{
		l.InternalIndex = index;
		m_vs_constantLayouts[l.Name] = l;
		constantLayoutsArray.push_back(&(m_vs_constantLayouts[l.Name]));
		index++;
	}

	for (auto& l : ps_uniformLayouts)
	{
		l.InternalIndex = index;
		m_ps_constantLayouts[l.Name] = l;
		constantLayoutsArray.push_back(&(m_ps_constantLayouts[l.Name]));
		index++;
	}

	index = 0;
	for (auto& l : vs_textures)
	{
		l.InternalIndex = index;
		m_vs_textureLayouts[l.Name] = l;
		textureLayoutsArray.push_back(&(m_vs_textureLayouts[l.Name]));
		index++;
	}

	for (auto& l : ps_textures)
	{
		l.InternalIndex = index;
		m_ps_textureLayouts[l.Name] = l;
		textureLayoutsArray.push_back(&(m_ps_textureLayouts[l.Name]));
		index++;
	}


	if (vs_uniformBufferSize > 0)
	{
		vs_uniformBufferSize = (vs_uniformBufferSize / 16 + 1) * 16;
		rhiConstantVB = ar::ConstantBuffer::Create(g->GetRHI());
		rhiConstantVB->Initialize(g->GetRHI(), vs_uniformBufferSize);
		m_vertexConstantBuffer = new uint8_t[vs_uniformBufferSize];

		vs_uniformBufferSize_ = vs_uniformBufferSize;
	}

	if (ps_uniformBufferSize > 0)
	{
		ps_uniformBufferSize = (ps_uniformBufferSize / 16 + 1) * 16;
		rhiConstantPB = ar::ConstantBuffer::Create(g->GetRHI());
		rhiConstantPB->Initialize(g->GetRHI(), ps_uniformBufferSize);
		m_pixelConstantBuffer = new uint8_t[ps_uniformBufferSize];

		ps_uniformBufferSize_ = ps_uniformBufferSize;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
NativeShader_Imp_DX11::~NativeShader_Imp_DX11()
{
	SafeDeleteArray(m_vertexConstantBuffer);
	SafeDeleteArray(m_pixelConstantBuffer);

	asd::SafeDelete(rhi);
	asd::SafeDelete(rhiConstantVB);
	asd::SafeDelete(rhiConstantPB);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

int32_t NativeShader_Imp_DX11::GetConstantBufferID(const char* name)
{
	auto key = std::string(name);

	auto it_vs = m_vs_constantLayouts.find(key);
	auto it_ps = m_ps_constantLayouts.find(key);

	if (it_vs != m_vs_constantLayouts.end())
	{
		return it_vs->second.InternalIndex;
	}

	if (it_ps != m_ps_constantLayouts.end())
	{
		return it_ps->second.InternalIndex;
	}
	return -1;
}

int32_t NativeShader_Imp_DX11::GetTextureID(const char* name)
{
	auto key = std::string(name);
	
	auto it_vs = m_vs_textureLayouts.find(key);
	auto it_ps = m_ps_textureLayouts.find(key);

	if (it_vs != m_vs_textureLayouts.end())
	{
		return it_vs->second.InternalIndex;
	}

	if (it_ps != m_ps_textureLayouts.end())
	{
		return it_ps->second.InternalIndex;
	}
	return -1;
}

void NativeShader_Imp_DX11::SetConstantBuffer(const char* name, const void* data, int32_t size)
{
	auto key = std::string(name);

	auto it_vs = m_vs_constantLayouts.find(key);
	auto it_ps = m_ps_constantLayouts.find(key);

	if (it_vs != m_vs_constantLayouts.end())
	{
		auto size_ = GetBufferSize(it_vs->second.Type, it_vs->second.Count);
		if (size != size_) return;

		memcpy(&(m_vertexConstantBuffer[it_vs->second.Offset]), data, size);
	}

	if (it_ps != m_ps_constantLayouts.end())
	{
		auto size_ = GetBufferSize(it_ps->second.Type, it_ps->second.Count);
		if (size != size_) return;

		memcpy(&(m_pixelConstantBuffer[it_ps->second.Offset]), data, size);
	}
}

void NativeShader_Imp_DX11::SetConstantBuffer(int32_t id, const void* data, int32_t size)
{
	assert(id < (int32_t) constantLayoutsArray.size());
	if (id < 0) return;

	auto& layout = constantLayoutsArray[id];

	if (id < m_vs_constantLayouts.size())
	{
		auto size_ = GetBufferSize(layout->Type, layout->Count);
		if (size != size_) return;

		memcpy(&(m_vertexConstantBuffer[layout->Offset]), data, size);
	}
	else
	{
		auto size_ = GetBufferSize(layout->Type, layout->Count);
		if (size != size_) return;

		memcpy(&(m_pixelConstantBuffer[layout->Offset]), data, size);
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void NativeShader_Imp_DX11::SetTexture(const char* name, Texture* texture, TextureFilterType filterType, TextureWrapType wrapType)
{
	auto key = std::string(name);
	auto g = (Graphics_Imp_DX11*) GetGraphics();

	auto it_vs = m_vs_textureLayouts.find(key);
	auto it_ps = m_ps_textureLayouts.find(key);

	if (it_vs != m_vs_textureLayouts.end())
	{
		NativeShader_Imp::SetTexture(name, texture, filterType, wrapType, (*it_vs).second.Index + 0xff);
	}

	if (it_ps != m_ps_textureLayouts.end())
	{
		NativeShader_Imp::SetTexture(name, texture, filterType, wrapType, (*it_ps).second.Index);
	}
}

void NativeShader_Imp_DX11::SetTexture(int32_t id, Texture* texture, TextureFilterType filterType, TextureWrapType wrapType)
{
	assert(id < (int32_t) textureLayoutsArray.size());
	if (id < 0) return;

	auto& layout = textureLayoutsArray[id];

	if (id < m_vs_textureLayouts.size())
	{
		NativeShader_Imp::SetTexture(layout->Name.c_str(), texture, filterType, wrapType, layout->Index + 0xff);
	}
	else
	{
		NativeShader_Imp::SetTexture(layout->Name.c_str(), texture, filterType, wrapType, layout->Index);
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void NativeShader_Imp_DX11::AssignConstantBuffer()
{
	auto g = (Graphics_Imp_DX11*) GetGraphics();

	if (m_vertexConstantBuffer != nullptr)
	{
		rhiConstantVB->SetData(m_vertexConstantBuffer, vs_uniformBufferSize_, 0);
	}

	if (m_pixelConstantBuffer != nullptr)
	{
		rhiConstantPB->SetData(m_pixelConstantBuffer, ps_uniformBufferSize_, 0);
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
NativeShader_Imp_DX11* NativeShader_Imp_DX11::Create(
	Graphics* graphics,
	const char* vertexShaderText,
	const char* vertexShaderFileName,
	const char* pixelShaderText,
	const char* pixelShaderFileName,
	std::vector <VertexLayout>& layout,
	bool is32Bit,
	std::vector <Macro>& macro,
	Log* log)
{
	auto g = (Graphics_Imp_DX11*)graphics;
	
	ar::ShaderCompilerParameter compilerParam;
	ar::ShaderCompilerResult compilerResult;

	
	// Hash
	std::string macroStr;
	for (auto& macro_ : macro)
	{
		macroStr = macroStr + macro_.Definition;
		macroStr = macroStr + macro_.Name;
	}

	auto hashMacro = CalcHash(macroStr.c_str());
	auto hashVS = CalcHash(vertexShaderText);
	auto hashPS = CalcHash(pixelShaderText);

	char cacheFile[256];

	if (g->GetRHI()->GetDeviceType() == ar::GraphicsDeviceType::DirectX11)
	{
		sprintf(cacheFile, "ShaderCache/dx_%d_%d_%d.ch", hashMacro, hashVS, hashPS);
	}
	else
	{
		sprintf(cacheFile, "ShaderCache/gl_%d_%d_%d.ch", hashMacro, hashVS, hashPS);
	}

	FILE* fp = nullptr;
#ifdef _WIN32
	fopen_s(&fp, cacheFile, "rb");
#else
	fp = fopen(cacheFile, "rb");
#endif
	if (fp == nullptr)
	{
		auto compiler = ar::Compiler::Create(g->GetRHI());

		compilerParam.VertexShaderTexts.push_back(vertexShaderText);
		compilerParam.PixelShaderTexts.push_back(pixelShaderText);

		// DirectX
		if (g->GetRHI()->GetDeviceType() == ar::GraphicsDeviceType::DirectX11)
		{
			const char* device = "DIRECTX";
			const char* one = "1";
			{
				ar::ShaderMacro m(device, one);
				compilerParam.Macros.push_back(m);
			}
		}
		else if (g->GetRHI()->GetDeviceType() == ar::GraphicsDeviceType::OpenGL)
		{
			compilerParam.OpenGLVersion = ar::OpenGLVersionType::OpenGL33;
			const char* device = "OPENGL";
			const char* one = "1";
			{
				ar::ShaderMacro m(device, one);
				compilerParam.Macros.push_back(m);
			}
		}
		
		for (auto& macro_ : macro)
		{
			ar::ShaderMacro m(macro_.Name, macro_.Definition);
			compilerParam.Macros.push_back(m);
		}

		if (!compiler->Compile(compilerResult, compilerParam))
		{
			log->WriteHeading("シェーダーコンパイル失敗");
			log->WriteLine(vertexShaderFileName);
			log->WriteLine(pixelShaderFileName);
			log->WriteLine(ToAString(compilerResult.ErrorMessage.c_str()).c_str());

			asd::SafeDelete(compiler);
		}

		asd::SafeDelete(compiler);

		// 書き込み
		if (g->GetRHI()->GetDeviceType() == ar::GraphicsDeviceType::DirectX11)
		{
#ifdef _WIN32
			fopen_s(&fp, cacheFile, "wb");
#else
			fp = fopen(cacheFile, "wb");
#endif
			if (fp != nullptr)
			{
				auto vs_size = compilerResult.VertexShaderBuffer.size();
				auto ps_size = compilerResult.PixelShaderBuffer.size();

				fwrite(&vs_size, sizeof(int32_t), 1, fp);
				fwrite(&ps_size, sizeof(int32_t), 1, fp);
				fwrite(compilerResult.VertexShaderBuffer.data(), 1, vs_size, fp);
				fwrite(compilerResult.PixelShaderBuffer.data(), 1, ps_size, fp);

				fclose(fp);
			}
		}
	}
	else
	{
		// 読み込み
		int32_t vs_size = 0;
		int32_t ps_size = 0;

		fread(&vs_size, sizeof(int32_t), 1, fp);
		fread(&ps_size, sizeof(int32_t), 1, fp);
		
		compilerResult.VertexShaderBuffer.resize(vs_size);
		compilerResult.PixelShaderBuffer.resize(ps_size);
		fread(compilerResult.VertexShaderBuffer.data(), 1, vs_size, fp);
		fread(compilerResult.PixelShaderBuffer.data(), 1, ps_size, fp);
		fclose(fp);
	}

	auto rhi = ar::Shader::Create(g->GetRHI());

	std::vector<ar::VertexLayout> layout_;
	for (auto& l : layout)
	{
		ar::VertexLayout l_;

		l_.Name = l.Name;
		l_.LayoutFormat = (ar::VertexLayoutFormat)l.LayoutFormat;

		layout_.push_back(l_);
	}

	if (rhi->Initialize(g->GetRHI(), compilerResult, layout_, is32Bit))
	{
		int32_t vs_uniformBufferSize = rhi->GetVertexConstantBufferSize();
		std::vector<ConstantLayout> vcls;
		std::vector<TextureLayout> vtls;

		int32_t ps_uniformBufferSize = rhi->GetPixelConstantBufferSize();
		std::vector<ConstantLayout> pcls;
		std::vector<TextureLayout> ptls;

		for (auto& l : rhi->GetVertexConstantLayouts())
		{
			ConstantLayout cl;

			cl.Name = l.first;
			cl.Count = l.second.Count;
			cl.InternalIndex = l.second.Index;
			cl.Offset = l.second.Offset;
			cl.Type = (ConstantBufferFormat)l.second.Type;

			vcls.push_back(cl);
		}

		for (auto& l : rhi->GetVertexTextureLayouts())
		{
			TextureLayout tl;
			tl.Name = l.first;
			tl.Index = l.second.Index;
			vtls.push_back(tl);
		}

		for (auto& l : rhi->GetPixelConstantLayouts())
		{
			ConstantLayout cl;

			cl.Name = l.first;
			cl.Count = l.second.Count;
			cl.InternalIndex = l.second.Index;
			cl.Offset = l.second.Offset;
			cl.Type = (ConstantBufferFormat)l.second.Type;

			pcls.push_back(cl);
		}

		for (auto& l : rhi->GetPixelTextureLayouts())
		{
			TextureLayout tl;
			tl.Name = l.first;
			tl.Index = l.second.Index;
			ptls.push_back(tl);
		}


		return new NativeShader_Imp_DX11(
			g,
			rhi,
			vcls,
			vs_uniformBufferSize,
			vtls,
			pcls,
			ps_uniformBufferSize,
			ptls);
	}

	asd::SafeDelete(rhi);
	return nullptr;
}

NativeShader_Imp* NativeShader_Imp_DX11::Create(
	Graphics* graphics,
	const uint8_t* vertexShader,
	int32_t vertexShaderSize,
	const uint8_t* pixelShader,
	int32_t pixelShaderSize,
	std::vector <VertexLayout>& layout,
	bool is32Bit)
{
	auto g = (Graphics_Imp_DX11*)graphics;

	ar::ShaderCompilerParameter compilerParam;
	ar::ShaderCompilerResult compilerResult;

	compilerResult.VertexShaderBuffer.resize(vertexShaderSize);
	memcpy(compilerResult.VertexShaderBuffer.data(), vertexShader, vertexShaderSize);

	compilerResult.PixelShaderBuffer.resize(pixelShaderSize);
	memcpy(compilerResult.PixelShaderBuffer.data(), pixelShader, pixelShaderSize);

	auto rhi = ar::Shader::Create(g->GetRHI());

	std::vector<ar::VertexLayout> layout_;
	for (auto& l : layout)
	{
		ar::VertexLayout l_;

		l_.Name = l.Name;
		l_.LayoutFormat = (ar::VertexLayoutFormat)l.LayoutFormat;

		layout_.push_back(l_);
	}

	if (rhi->Initialize(g->GetRHI(), compilerResult, layout_, is32Bit))
	{
		int32_t vs_uniformBufferSize = rhi->GetVertexConstantBufferSize();
		std::vector<ConstantLayout> vcls;
		std::vector<TextureLayout> vtls;

		int32_t ps_uniformBufferSize = rhi->GetPixelConstantBufferSize();
		std::vector<ConstantLayout> pcls;
		std::vector<TextureLayout> ptls;

		for (auto& l : rhi->GetVertexConstantLayouts())
		{
			ConstantLayout cl;

			cl.Name = l.first;
			cl.Count = l.second.Count;
			cl.InternalIndex = l.second.Index;
			cl.Offset = l.second.Offset;
			cl.Type = (ConstantBufferFormat)l.second.Type;

			vcls.push_back(cl);
		}

		for (auto& l : rhi->GetVertexTextureLayouts())
		{
			TextureLayout tl;
			tl.Name = l.first;
			tl.Index = l.second.Index;
			vtls.push_back(tl);
		}

		for (auto& l : rhi->GetPixelConstantLayouts())
		{
			ConstantLayout cl;

			cl.Name = l.first;
			cl.Count = l.second.Count;
			cl.InternalIndex = l.second.Index;
			cl.Offset = l.second.Offset;
			cl.Type = (ConstantBufferFormat)l.second.Type;

			pcls.push_back(cl);
		}

		for (auto& l : rhi->GetPixelTextureLayouts())
		{
			TextureLayout tl;
			tl.Name = l.first;
			tl.Index = l.second.Index;
			ptls.push_back(tl);
		}


		return new NativeShader_Imp_DX11(
			g,
			rhi,
			vcls,
			vs_uniformBufferSize,
			vtls,
			pcls,
			ps_uniformBufferSize,
			ptls);
	}

	asd::SafeDelete(rhi);
	return nullptr;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}