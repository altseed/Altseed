
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.NativeShader_Imp_DX11.h"

#include "../ace.Graphics_Imp_DX11.h"

#include "../../../../Log/ace.Log.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
ID3DBlob* NativeShader_Imp_DX11::CompileVertexShader(Graphics_Imp_DX11* g, const char* vertexShaderText, const char* vertexShaderFileName, std::vector <Macro>& macro, Log* log)
{
	ID3DBlob* shader = nullptr;
	ID3DBlob* error = nullptr;

	// DirectX
	const char* device = "DIRECTX";
	const char* one = "1";

	{
		Macro m;
		m.Name = device;
		m.Definition = one;
		macro.push_back(m);
	}

	if (macro.size() > 0)
	{
		Macro m;
		m.Definition = NULL;
		m.Name = NULL;
		macro.push_back(m);
	}

	UINT flag = D3D10_SHADER_PACK_MATRIX_ROW_MAJOR;
#if !_DEBUG
	flag = flag | D3D10_SHADER_OPTIMIZATION_LEVEL3;
#endif

	/* この方法だとVS4.0以上でのコンパイルが要求 */
	auto hr = D3DCompile(
		vertexShaderText,
		strlen(vertexShaderText),
		vertexShaderFileName,
		macro.size() > 0 ? (D3D_SHADER_MACRO*) &macro[0] : NULL,
		NULL,
		"main",
		"vs_4_0",
		flag,
		0,
		&shader,
		&error);

	if (macro.size() > 0)
	{
		macro.pop_back();
		macro.pop_back();
	}

	if (FAILED(hr))
	{
		log->WriteHeading("頂点シェーダーコンパイル失敗");
		log->WriteLine(vertexShaderFileName);

		if (hr == E_OUTOFMEMORY)
		{
			log->WriteLine("E_OUTOFMEMORY");
		}
		else
		{
			log->WriteLine("不明なエラー");
		}

		if (error != NULL)
		{
			log->WriteLine((const char*) error->GetBufferPointer());
			OutputDebugStringA((const char*) error->GetBufferPointer());
			error->Release();
		}

		return nullptr;
	}

	return shader;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
ID3DBlob* NativeShader_Imp_DX11::CompilePixelShader(Graphics_Imp_DX11* g, const char* vertexShaderText, const char* vertexShaderFileName, std::vector <Macro>& macro, Log* log)
{
	ID3DBlob* shader = nullptr;
	ID3DBlob* error = nullptr;

	// DirectX
	const char* device = "DIRECTX";
	const char* one = "1";

	{
		Macro m;
m.Name = device;
m.Definition = one;
macro.push_back(m);
	}

	if (macro.size() > 0)
	{
		Macro m;
		m.Definition = NULL;
		m.Name = NULL;
		macro.push_back(m);
	}

	UINT flag = D3D10_SHADER_PACK_MATRIX_ROW_MAJOR;
#if !_DEBUG
	flag = flag | D3D10_SHADER_OPTIMIZATION_LEVEL3;
#endif

	/* この方法だとPS4.0以上でのコンパイルが要求 */
	auto hr = D3DCompile(
		vertexShaderText,
		strlen(vertexShaderText),
		vertexShaderFileName,
		macro.size() > 0 ? (D3D_SHADER_MACRO*) &macro[0] : NULL,
		NULL,
		"main",
		"ps_4_0",
		flag,
		0,
		&shader,
		&error);

	if (macro.size() > 0)
	{
		macro.pop_back();
		macro.pop_back();
	}

	if (FAILED(hr))
	{
		log->WriteHeading("ピクセルシェーダーコンパイル失敗");
		log->WriteLine(vertexShaderFileName);

		if (hr == E_OUTOFMEMORY)
		{
			log->WriteLine("E_OUTOFMEMORY");
		}
		else
		{
			log->WriteLine("不明なエラー");
		}

		if (error != NULL)
		{
			log->WriteLine((const char*) error->GetBufferPointer());
			OutputDebugStringA((const char*) error->GetBufferPointer());
			error->Release();
		}

		return nullptr;
	}

	return shader;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void NativeShader_Imp_DX11::Reflect(void* buf, int32_t bufSize, std::vector<ConstantLayout>& uniformLayouts, int32_t& uniformBufferSize, std::vector<TextureLayout>& textures)
{
	auto getBufferType = [](D3D11_SHADER_TYPE_DESC typeDesc, eConstantBufferFormat& format, int32_t& elements) -> void
	{
		elements = 1;

		if (typeDesc.Class == D3D_SHADER_VARIABLE_CLASS::D3D10_SVC_SCALAR && typeDesc.Type == D3D_SHADER_VARIABLE_TYPE::D3D10_SVT_FLOAT)
		{
			if (typeDesc.Elements == 0)
			{
				if (typeDesc.Columns == 1) format = eConstantBufferFormat::CONSTANT_BUFFER_FORMAT_FLOAT1;
			}
			else if (typeDesc.Elements > 0)
			{
				if (typeDesc.Columns == 1)
				{
					elements = typeDesc.Elements;
					format = eConstantBufferFormat::CONSTANT_BUFFER_FORMAT_FLOAT1_ARRAY;
				}
			}
		}

		if (typeDesc.Class == D3D_SHADER_VARIABLE_CLASS::D3D_SVC_VECTOR && typeDesc.Type == D3D_SHADER_VARIABLE_TYPE::D3D10_SVT_FLOAT)
		{
			if (typeDesc.Columns == 2) format = eConstantBufferFormat::CONSTANT_BUFFER_FORMAT_FLOAT2;
			if (typeDesc.Columns == 3) format = eConstantBufferFormat::CONSTANT_BUFFER_FORMAT_FLOAT3;
			if (typeDesc.Columns == 4) format = eConstantBufferFormat::CONSTANT_BUFFER_FORMAT_FLOAT4;
		}

		if (typeDesc.Class == D3D_SHADER_VARIABLE_CLASS::D3D_SVC_MATRIX_ROWS && typeDesc.Type == D3D_SHADER_VARIABLE_TYPE::D3D10_SVT_FLOAT)
		{
			if (typeDesc.Rows == 4 && typeDesc.Columns == 4)
			{
				if (typeDesc.Elements == 0) format = eConstantBufferFormat::CONSTANT_BUFFER_FORMAT_MATRIX44;
				if (typeDesc.Elements > 0)
				{
					elements = typeDesc.Elements;
					format = eConstantBufferFormat::CONSTANT_BUFFER_FORMAT_MATRIX44_ARRAY;
				}
			}
		}
	};

	auto getResourceType = [](D3D11_SHADER_INPUT_BIND_DESC bindDesc, eConstantBufferFormat& format, int32_t& bindPoint) -> bool
	{
		if (bindDesc.Type == D3D_SIT_TEXTURE)
		{
			if (bindDesc.Dimension == D3D_SRV_DIMENSION_TEXTURE2D)
			{
				bindPoint = bindDesc.BindPoint;
				return true;
			}
			else if (bindDesc.Dimension == D3D_SRV_DIMENSION_TEXTURECUBE)
			{
				bindPoint = bindDesc.BindPoint;
				return true;
			}
			else
			{
				return false;
			}
		}

		return false;
	};

	int32_t offset = 0;

	ID3D11ShaderReflection*	reflection = nullptr;
	D3DReflect(buf, bufSize, IID_ID3D11ShaderReflection, (void**) &reflection);

	D3D11_SHADER_DESC shaderDesc;
	reflection->GetDesc(&shaderDesc);

	for (int32_t i = 0; i < shaderDesc.BoundResources; i++)
	{
		D3D11_SHADER_INPUT_BIND_DESC bindDesc;
		reflection->GetResourceBindingDesc(i, &bindDesc);

		eConstantBufferFormat format;
		int32_t bindPoint = 0;
		if (!getResourceType(bindDesc, format, bindPoint)) continue;
		auto name = bindDesc.Name;

		TextureLayout layout;
		layout.Name = name;
		layout.ID = bindPoint;
		textures.push_back(layout);
	}

	for (int32_t i = 0; i < shaderDesc.ConstantBuffers; i++)
	{
		auto cbuf = reflection->GetConstantBufferByIndex(i);

		D3D11_SHADER_BUFFER_DESC bufferDesc;
		cbuf->GetDesc(&bufferDesc);
		for (int32_t j = 0; j < bufferDesc.Variables; j++)
		{
			auto variable = cbuf->GetVariableByIndex(j);
			if (variable == nullptr) continue;

			auto type = variable->GetType();

			D3D11_SHADER_TYPE_DESC typeDesc;
			type->GetDesc(&typeDesc);

			D3D11_SHADER_VARIABLE_DESC variableDesc;
			variable->GetDesc(&variableDesc);
			auto name = variableDesc.Name;
			auto size = variableDesc.Size;
			auto startOffset = variableDesc.StartOffset;

			eConstantBufferFormat format;
			int32_t elements = 0;
			getBufferType(typeDesc, format, elements);

			offset = Max(offset, startOffset + size);
			ConstantLayout l;
			l.Name = name;
			l.Count = elements;
			l.Offset = startOffset;
			l.Type = format;

			uniformLayouts.push_back(l);
		}

	}

	SafeRelease(reflection);

	uniformBufferSize = offset;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
NativeShader_Imp_DX11::NativeShader_Imp_DX11(
	Graphics* graphics, 
	ID3D11VertexShader* vertexShader, 
	ID3D11PixelShader* pixelShader, 
	ID3D11InputLayout* layout,
	std::vector<ConstantLayout> vs_uniformLayouts,
	int32_t vs_uniformBufferSize,
	std::vector<TextureLayout> vs_textures,
	std::vector<ConstantLayout> ps_uniformLayouts,
	int32_t ps_uniformBufferSize,
	std::vector<TextureLayout> ps_textures)
	: NativeShader_Imp(graphics)
	, m_vertexShader(vertexShader)
	, m_pixelShader(pixelShader)
	, m_vertexDeclaration(layout)
	, m_constantBufferToVS(nullptr)
	, m_constantBufferToPS(nullptr)
{
	int32_t index = 0;

	index = 0;
	for (auto& l : vs_uniformLayouts)
	{
		l.Index = index;
		m_vs_constantLayouts[l.Name] = l;
		constantLayoutsArray.push_back(&(m_vs_constantLayouts[l.Name]));
		index++;
	}

	for (auto& l : ps_uniformLayouts)
	{
		l.Index = index;
		m_ps_constantLayouts[l.Name] = l;
		constantLayoutsArray.push_back(&(m_ps_constantLayouts[l.Name]));
		index++;
	}

	index = 0;
	for (auto& l : vs_textures)
	{
		l.Index = index;
		m_vs_textureLayouts[l.Name] = l;
		textureLayoutsArray.push_back(&(m_vs_textureLayouts[l.Name]));
		index++;
	}

	for (auto& l : ps_textures)
	{
		l.Index = index;
		m_ps_textureLayouts[l.Name] = l;
		textureLayoutsArray.push_back(&(m_ps_textureLayouts[l.Name]));
		index++;
	}


	if (vs_uniformBufferSize > 0)
	{
		vs_uniformBufferSize = (vs_uniformBufferSize / 16 + 1) * 16;

		SafeRelease(m_constantBufferToVS);
		m_vertexConstantBuffer = new uint8_t[vs_uniformBufferSize];
	
		auto g = (Graphics_Imp_DX11*) GetGraphics();

		D3D11_BUFFER_DESC hBufferDesc;
		hBufferDesc.ByteWidth = vs_uniformBufferSize;
		hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		hBufferDesc.CPUAccessFlags = 0;
		hBufferDesc.MiscFlags = 0;
		hBufferDesc.StructureByteStride = sizeof(float);

		g->GetDevice()->CreateBuffer(&hBufferDesc, NULL, &m_constantBufferToVS);
	}

	if (ps_uniformBufferSize > 0)
	{
		ps_uniformBufferSize = (ps_uniformBufferSize / 16 + 1) * 16;

		SafeRelease(m_constantBufferToPS);
		m_pixelConstantBuffer = new uint8_t[ps_uniformBufferSize];

		auto g = (Graphics_Imp_DX11*) GetGraphics();

		D3D11_BUFFER_DESC hBufferDesc;
		hBufferDesc.ByteWidth = ps_uniformBufferSize;
		hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		hBufferDesc.CPUAccessFlags = 0;
		hBufferDesc.MiscFlags = 0;
		hBufferDesc.StructureByteStride = sizeof(float);

		g->GetDevice()->CreateBuffer(&hBufferDesc, NULL, &m_constantBufferToPS);
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
NativeShader_Imp_DX11::~NativeShader_Imp_DX11()
{
	SafeRelease(m_constantBufferToVS);
	SafeRelease(m_constantBufferToPS);
	SafeRelease(m_vertexShader);
	SafeRelease(m_pixelShader);
	SafeRelease(m_vertexDeclaration);
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
		return it_vs->second.Index;
	}

	if (it_ps != m_ps_constantLayouts.end())
	{
		return it_ps->second.Index;
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
		return it_vs->second.Index;
	}

	if (it_ps != m_ps_textureLayouts.end())
	{
		return it_ps->second.Index;
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
		NativeShader_Imp::SetTexture(name, texture, filterType, wrapType, (*it_vs).second.ID + 0xff);
	}

	if (it_ps != m_ps_textureLayouts.end())
	{
		NativeShader_Imp::SetTexture(name, texture, filterType, wrapType, (*it_ps).second.ID);
	}
}

void NativeShader_Imp_DX11::SetTexture(int32_t id, Texture* texture, TextureFilterType filterType, TextureWrapType wrapType)
{
	assert(id < (int32_t) textureLayoutsArray.size());
	if (id < 0) return;

	auto& layout = textureLayoutsArray[id];

	if (id < m_vs_textureLayouts.size())
	{
		NativeShader_Imp::SetTexture(layout->Name.c_str(), texture, filterType, wrapType, layout->ID + 0xff);
	}
	else
	{
		NativeShader_Imp::SetTexture(layout->Name.c_str(), texture, filterType, wrapType, layout->ID);
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
		g->GetContext()->UpdateSubresource(m_constantBufferToVS, 0, NULL, m_vertexConstantBuffer, 0, 0);
		g->GetContext()->VSSetConstantBuffers(0, 1, &m_constantBufferToVS);
	}

	if (m_pixelConstantBuffer != nullptr)
	{
		g->GetContext()->UpdateSubresource(m_constantBufferToPS, 0, NULL, m_pixelConstantBuffer, 0, 0);
		g->GetContext()->PSSetConstantBuffers(0, 1, &m_constantBufferToPS);
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
	std::vector <Macro>& macro,
	Log* log)
{
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
	sprintf(cacheFile, "ShaderCache/dx_%d_%d_%d.ch", hashMacro, hashVS, hashPS);

	HRESULT hr;
	auto g = (Graphics_Imp_DX11*) graphics;
	ID3D11VertexShader* vs = nullptr;
	ID3D11PixelShader* ps = nullptr;
	ID3D11InputLayout* il = nullptr;
	std::vector< D3D11_INPUT_ELEMENT_DESC> decl;

	std::vector<ConstantLayout> vs_uniformLayouts;
	int32_t vs_uniformBufferSize;
	std::vector<TextureLayout> vs_textures;

	std::vector<ConstantLayout> ps_uniformLayouts;
	int32_t ps_uniformBufferSize;
	std::vector<TextureLayout> ps_textures;

	ID3D10Blob* vertexShader = nullptr;
	ID3D10Blob* pixelShader = nullptr;
	std::vector<uint8_t> vertexShaderBuf;
	std::vector<uint8_t> pixelShaderBuf;

	void* vertexShaderPtr = nullptr;
	int32_t vertexShaderSize = 0;

	void* pixelShaderPtr = nullptr;
	int32_t pixelShaderSize = 0;

	FILE* fp = nullptr;
	fopen_s(&fp, cacheFile, "rb");
	if (fp == nullptr)
	{
		vertexShader = CompileVertexShader(
			(Graphics_Imp_DX11*) graphics,
			vertexShaderText,
			vertexShaderFileName,
			macro,
			log
			);

		pixelShader = CompilePixelShader(
			(Graphics_Imp_DX11*) graphics,
			pixelShaderText,
			pixelShaderFileName,
			macro,
			log
			);

		if (vertexShader == nullptr ||
			pixelShader == nullptr)
		{
			goto End;
		}

		vertexShaderPtr = vertexShader->GetBufferPointer();
		vertexShaderSize = vertexShader->GetBufferSize();

		pixelShaderPtr = pixelShader->GetBufferPointer();
		pixelShaderSize = pixelShader->GetBufferSize();

		// 書き込み
		fopen_s(&fp, cacheFile, "wb");
		if (fp != nullptr)
		{
			fwrite(&vertexShaderSize, sizeof(int32_t), 1, fp);
			fwrite(&pixelShaderSize, sizeof(int32_t), 1, fp);
			fwrite(vertexShaderPtr, 1, vertexShaderSize, fp);
			fwrite(pixelShaderPtr, 1, pixelShaderSize, fp);

			fclose(fp);
		}
	}
	else
	{
		// 読み込み
		fread(&vertexShaderSize, sizeof(int32_t), 1, fp);
		fread(&pixelShaderSize, sizeof(int32_t), 1, fp);
		vertexShaderBuf.resize(vertexShaderSize);
		pixelShaderBuf.resize(pixelShaderSize);
		fread(vertexShaderBuf.data(), 1, vertexShaderSize, fp);
		fread(pixelShaderBuf.data(), 1, pixelShaderSize, fp);
		vertexShaderPtr = vertexShaderBuf.data();
		pixelShaderPtr = pixelShaderBuf.data();
		fclose(fp);
	}

	hr = g->GetDevice()->CreateVertexShader(
		vertexShaderPtr,
		vertexShaderSize,
		NULL,
		&vs
		);

	if (FAILED(hr))
	{
		log->WriteHeading("頂点シェーダー生成失敗");
		log->WriteLine(vertexShaderFileName);
		goto End;
	}

	hr = g->GetDevice()->CreatePixelShader(
		pixelShaderPtr,
		pixelShaderSize,
		NULL,
		&ps
		);

	if (FAILED(hr))
	{
		log->WriteHeading("ピクセルシェーダー生成失敗");
		log->WriteLine(pixelShaderFileName);
		goto End;
	}

	int32_t byteOffset = 0;
	for each ( auto& l in layout)
	{
		auto d = D3D11_INPUT_ELEMENT_DESC();
		ZeroMemory(&d, sizeof(D3D11_INPUT_ELEMENT_DESC));

		if (l.LayoutFormat == LAYOUT_FORMAT_R32G32B32_FLOAT)
		{
			d.SemanticName = l.Name.c_str();
			d.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			d.AlignedByteOffset = byteOffset;
			d.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			byteOffset += sizeof(float) * 3;
		}
		else if (l.LayoutFormat == LAYOUT_FORMAT_R8G8B8A8_UNORM)
		{
			d.SemanticName = l.Name.c_str();
			d.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			d.AlignedByteOffset = byteOffset;
			d.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			byteOffset += sizeof(float) * 1;
		}
		else if (l.LayoutFormat == LAYOUT_FORMAT_R8G8B8A8_UINT)
		{
			d.SemanticName = l.Name.c_str();
			d.Format = DXGI_FORMAT_R8G8B8A8_UINT;
			d.AlignedByteOffset = byteOffset;
			d.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			byteOffset += sizeof(float) * 1;
		}
		else if (l.LayoutFormat == LAYOUT_FORMAT_R32G32_FLOAT)
		{
			d.SemanticName = l.Name.c_str();
			d.Format = DXGI_FORMAT_R32G32_FLOAT;
			d.AlignedByteOffset = byteOffset;
			d.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			byteOffset += sizeof(float) * 2;
		}
		decl.push_back(d);
	}

	Reflect(vertexShaderPtr, vertexShaderSize, vs_uniformLayouts, vs_uniformBufferSize, vs_textures);
	Reflect(pixelShaderPtr, pixelShaderSize, ps_uniformLayouts, ps_uniformBufferSize, ps_textures);

	hr = g->GetDevice()->CreateInputLayout(
		&(decl[0]),
		decl.size(),
		vertexShaderPtr,
		vertexShaderSize,
		&il);

	SafeRelease(vertexShader);
	SafeRelease(pixelShader);

	if (FAILED(hr))
	{
		log->WriteHeading("頂点レイアウト生成失敗");
		log->WriteLine(vertexShaderFileName);
		log->WriteLine(pixelShaderFileName);
		goto End;
	}

	

	return new NativeShader_Imp_DX11(
		g,
		vs,
		ps,
		il,
		vs_uniformLayouts,
		vs_uniformBufferSize,
		vs_textures,
		ps_uniformLayouts,
		ps_uniformBufferSize,
		ps_textures);

End:;
	SafeRelease(vertexShader);
	SafeRelease(pixelShader);
	SafeRelease(vs);
	SafeRelease(ps);
	SafeRelease(il);

	return nullptr;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}