
#include "asd.NativeShader_Imp.h"
#include "asd.CubemapTexture.h"
#include "asd.Texture2D_Imp.h"

#include "../../Log/asd.Log.h"

// DixHash32.h

// 32bitハッシュクラス
//  製作者: IKD
//  HP    : http://marupeke296.com

// 入力文字列から32bitのハッシュ値を求めるクラスです。
// CRC32を使用しています。
//
//  参照: http://burtleburtle.net/bob/c/crc.c (パブリックドメインコード）
//      : http://burtleburtle.net/bob/hash/doobs.html （CRC Hashing）

// バージョン
//
// v1.00
//  ・初出
//
// v1.01
//  ・CRC配列の参照オーバーランがあったバグを修正
//
// v1.02
//  ・整数値をハッシュ化する際に前回の文字数が参照されていたバグを修正
//

#ifndef IKD_DIX_DIXHASH32_H
#define IKD_DIX_DIXHASH32_H

namespace Dix
{
class Hash32
{
	unsigned val; // ハッシュ値

public:
	Hash32();
	Hash32(const char* str);
	Hash32(const char* str, unsigned len);
	Hash32(unsigned uiVal);
	Hash32(int uiVal);
	~Hash32();
	unsigned get() const { return val; }

	// 文字列チェック
	bool operator==(const char* str) const;

	// 比較
	bool operator==(const Hash32& src) const { return (val == src.val); }
	bool operator==(const unsigned& uiVal) const;
	bool operator==(const int& iVal) const;
	bool operator!=(const Hash32& src) const { return (val != src.val); }
	bool operator!=(const unsigned& uiVal) const;
	bool operator!=(const int& iVal) const;
	bool operator<(const Hash32& src) const { return (val < src.val); }
	bool operator>(const Hash32& src) const { return (val > src.val); }
	bool operator<(const char* str) const;
	bool operator>(const char* str) const;
	bool operator<(const unsigned& uiVal) const;
	bool operator>(const unsigned& uiVal) const;
	bool operator<(const int& iVal) const;
	bool operator>(const int& iVal) const;

	// 値取得
	operator unsigned() const { return val; }
};
} // namespace Dix

#endif

// DixHash32.cpp
#include <stdio.h>
#include <string.h>

namespace
{
// ハッシュテーブル
static const unsigned crctab[256] = {
	0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3, 0x0edb8832, 0x79dcb8a4, 0xe0d5e91e,
	0x97d2d988, 0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb,
	0xf4d4b551, 0x83d385c7, 0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5, 0x3b6e20c8,
	0x4c69105e, 0xd56041e4, 0xa2677172, 0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940,
	0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59, 0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423, 0xcfba9599,
	0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924, 0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 0x76dc4190, 0x01db7106,
	0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433, 0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb,
	0x086d3d2d, 0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e, 0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457,
	0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65, 0x4db26158, 0x3ab551ce, 0xa3bc0074,
	0xd4bb30e2, 0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0, 0x44042d73, 0x33031de5,
	0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f, 0x5edef90e,
	0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
	0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27,
	0x7d079eb1, 0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0,
	0x10da7a5a, 0x67dd4acc, 0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252, 0xd1bb67f1,
	0xa6bc5767, 0x3fb506dd, 0x48b2364b, 0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79,
	0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236, 0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92,
	0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d, 0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
	0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38, 0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4,
	0xf1d4e242, 0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777, 0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c,
	0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2, 0xa7672661, 0xd06016f7, 0x4969474d,
	0x3e6e77db, 0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9, 0xbdbdf21c, 0xcabac28a,
	0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94, 0xb40bbe37,
	0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d,
};

// ハッシュ値算出
unsigned calcCRCHash(const char* str, unsigned len)
{
	unsigned hash = len;
	for (unsigned i = 0; i < len; i++)
		hash = (hash >> 8) ^ crctab[(unsigned char)((hash & 0xff) ^ str[i])];
	return hash;
}

// 符合無し整数値からハッシュ算出
unsigned calcCRCHash(unsigned int uiVal)
{
	char c[16] = {};
	sprintf(c, "%u", uiVal);
	return calcCRCHash(c, (unsigned)strlen(c));
}

// 符合付き整数値からハッシュ算出
unsigned calcCRCHash(int iVal)
{
	char c[16] = {};
	sprintf(c, "%d", iVal);
	return calcCRCHash(c, (unsigned)strlen(c));
}
} // namespace

namespace Dix
{
Hash32::Hash32() : val() {}
Hash32::Hash32(const char* str) : val(str ? calcCRCHash(str, (unsigned)strlen(str)) : 0) {}
Hash32::Hash32(const char* str, unsigned len) : val((str && len) ? calcCRCHash(str, len) : 0) {}
Hash32::Hash32(unsigned int uiVal) : val(calcCRCHash(uiVal)) {}
Hash32::Hash32(int iVal) : val(calcCRCHash(iVal)) {}

Hash32::~Hash32() {}

// 文字列チェック
bool Hash32::operator==(const char* str) const { return (val == calcCRCHash(str, (unsigned)strlen(str))); }
bool Hash32::operator==(const unsigned& uiVal) const { return (val == calcCRCHash(uiVal)); }
bool Hash32::operator!=(const unsigned& uiVal) const { return (val != calcCRCHash(uiVal)); }
bool Hash32::operator==(const int& iVal) const { return (val == calcCRCHash(iVal)); }
bool Hash32::operator!=(const int& iVal) const { return (val != calcCRCHash(iVal)); }

// 大小比較（辞書作成時に必要）
bool Hash32::operator<(const char* str) const { return (val < calcCRCHash(str, (unsigned)strlen(str))); }
bool Hash32::operator>(const char* str) const { return (val > calcCRCHash(str, (unsigned)strlen(str))); }
bool Hash32::operator<(const unsigned& uiVal) const { return (val < calcCRCHash(uiVal)); }
bool Hash32::operator>(const unsigned& uiVal) const { return (val > calcCRCHash(uiVal)); }
bool Hash32::operator<(const int& iVal) const { return (val < calcCRCHash(iVal)); }
bool Hash32::operator>(const int& iVal) const { return (val > calcCRCHash(iVal)); }
} // namespace Dix

namespace asd
{

int32_t NativeShader_Imp::GetBufferSize(ConstantBufferFormat type, int32_t count)
{
	if (type == ConstantBufferFormat::Float1)
	{
		return sizeof(float) * 1 * count;
	}

	if (type == ConstantBufferFormat::Float2)
	{
		return sizeof(float) * 2 * count;
	}

	if (type == ConstantBufferFormat::Float3)
	{
		return sizeof(float) * 3 * count;
	}

	if (type == ConstantBufferFormat::Float4)
	{
		return sizeof(float) * 4 * count;
	}

	if (type == ConstantBufferFormat::Matrix44)
	{
		return sizeof(float) * 16 * count;
	}

	if (type == ConstantBufferFormat::Float4_ARRAY)
	{
		return sizeof(float) * 4 * count;
	}

	if (type == ConstantBufferFormat::Matrix44_ARRAY)
	{
		return sizeof(float) * 16 * count;
	}

	return 0;
}

uint32_t NativeShader_Imp::CalcHash(const char* text)
{
	Dix::Hash32 hash(text);
	return hash.get();
}

NativeShader_Imp::NativeShader_Imp(Graphics* graphics,
								   ar::Shader* rhi,
								   std::vector<ConstantLayout> vs_uniformLayouts,
								   int32_t vs_uniformBufferSize,
								   std::vector<TextureLayout> vs_textures,
								   std::vector<ConstantLayout> ps_uniformLayouts,
								   int32_t ps_uniformBufferSize,
								   std::vector<TextureLayout> ps_textures)
	: DeviceObject(graphics), m_vertexConstantBuffer(nullptr), m_pixelConstantBuffer(nullptr), rhi(rhi)
{
	for (int32_t i = 0; i < Graphics_Imp::MaxTextureCount; i++)
	{
		m_textureSlots[i] = nullptr;
		textureFilterTypes[i] = TextureFilterType::Nearest;
		textureWrapTypes[i] = TextureWrapType::Clamp;
	}

	auto g = (Graphics_Imp*)graphics;

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

NativeShader_Imp::~NativeShader_Imp()
{
	SafeDeleteArray(m_vertexConstantBuffer);
	SafeDeleteArray(m_pixelConstantBuffer);

	for (int32_t i = 0; i < Graphics_Imp::MaxTextureCount; i++)
	{
		auto t = (Texture2D*)m_textureSlots[i];
		SafeRelease(t);
	}

	asd::SafeDelete(rhi);
	asd::SafeDelete(rhiConstantVB);
	asd::SafeDelete(rhiConstantPB);
}

void NativeShader_Imp::SetFloat(const char* name, const float& value) { SetConstantBuffer(name, &value, sizeof(float)); }

void NativeShader_Imp::SetVector2DF(const char* name, const Vector2DF& value) { SetConstantBuffer(name, &value, sizeof(Vector2DF)); }

void NativeShader_Imp::SetVector3DF(const char* name, const Vector3DF& value) { SetConstantBuffer(name, &value, sizeof(Vector3DF)); }

void NativeShader_Imp::SetVector4DF(const char* name, const Vector4DF& value) { SetConstantBuffer(name, &value, sizeof(Vector4DF)); }

void NativeShader_Imp::SetMatrix44(const char* name, const Matrix44& value) { SetConstantBuffer(name, &value, sizeof(Matrix44)); }

void NativeShader_Imp::SetFloat(int32_t id, const float& value) { SetConstantBuffer(id, &value, sizeof(float)); }

void NativeShader_Imp::SetVector2DF(int32_t id, const Vector2DF& value) { SetConstantBuffer(id, &value, sizeof(Vector2DF)); }

void NativeShader_Imp::SetVector3DF(int32_t id, const Vector3DF& value) { SetConstantBuffer(id, &value, sizeof(Vector3DF)); }

void NativeShader_Imp::SetVector4DF(int32_t id, const Vector4DF& value) { SetConstantBuffer(id, &value, sizeof(Vector4DF)); }

void NativeShader_Imp::SetVector4DFArray(int32_t id, Vector4DF* value, int32_t count)
{
	SetConstantBuffer(id, value, sizeof(Vector4DF) * count);
}

void NativeShader_Imp::SetMatrix44(int32_t id, const Matrix44& value) { SetConstantBuffer(id, &value, sizeof(Matrix44)); }

void NativeShader_Imp::SetMatrix44Array(int32_t id, Matrix44* value, int32_t count)
{
	SetConstantBuffer(id, value, sizeof(Matrix44) * count);
}

void NativeShader_Imp::SetTexture(const char* name, Texture* texture, TextureFilterType filterType, TextureWrapType wrapType, int32_t index)
{
	{
		SafeAddRef(texture);

		BindingTexture bt;
		bt.TexturePtr = CreateSharedPtrWithReleaseDLL(texture);
		bt.FilterType = filterType;
		bt.WrapType = wrapType;
		bt.Name = name;

		bindingTextures[index] = bt;
	}

	if (index >= Graphics_Imp::MaxTextureCount)
		return;

	SafeAddRef(texture);

	{
		auto t = m_textureSlots[index];
		SafeRelease(t);
	}

	{
		auto t = texture;
		m_textureSlots[index] = t;
		m_textureNames[index] = name;
		textureFilterTypes[index] = filterType;
		textureWrapTypes[index] = wrapType;
	}
}

bool NativeShader_Imp::GetTexture(char*& name, Texture*& texture, TextureFilterType& filterType, TextureWrapType& wrapType, int32_t index)
{
	if (index >= Graphics_Imp::MaxTextureCount)
		return false;
	if (m_textureSlots[index] == nullptr)
		return false;

	name = (char*)m_textureNames[index].c_str();
	texture = m_textureSlots[index];
	filterType = textureFilterTypes[index];
	wrapType = textureWrapTypes[index];
	return true;
}

void NativeShader_Imp::SetConstantValues(ShaderConstantValue* constantValues, int32_t constantValueCount)
{
	// テクスチャを全てnullにする。
	bindingTextures.clear();

	for (auto i = 0; i < constantValueCount; i++)
	{
		auto& value = constantValues[i];

		if (value.ValueType == ShaderVariableType::Float)
		{
			SetFloat(value.ID, value.Data.Float4[0]);
		}
		else if (value.ValueType == ShaderVariableType::Vector2DF)
		{
			SetVector2DF(value.ID, Vector2DF(value.Data.Float4[0], value.Data.Float4[1]));
		}
		else if (value.ValueType == ShaderVariableType::Vector3DF)
		{
			SetVector3DF(value.ID, Vector3DF(value.Data.Float4[0], value.Data.Float4[1], value.Data.Float4[2]));
		}
		else if (value.ValueType == ShaderVariableType::Vector4DF)
		{
			SetVector4DF(value.ID, Vector4DF(value.Data.Float4[0], value.Data.Float4[1], value.Data.Float4[2], value.Data.Float4[3]));
		}
		else if (value.ValueType == ShaderVariableType::Vector4DF_Array)
		{
			SetVector4DFArray(value.ID, value.Data.Vector4DFArray.Ptr, value.Data.Vector4DFArray.Count);
		}
		else if (value.ValueType == ShaderVariableType::Matrix44)
		{
			Matrix44 mat;
			memcpy(&mat, value.Data.Mat44, sizeof(Matrix44));
			SetMatrix44(value.ID, mat);
		}
		else if (value.ValueType == ShaderVariableType::Matrix44_Array)
		{
			SetMatrix44Array(value.ID, value.Data.Mat44Array.Ptr, value.Data.Mat44Array.Count);
		}
		else if (value.ValueType == ShaderVariableType::Texture2D)
		{
			SetTexture(value.ID, value.Data.Texture2DPtr.Ptr, value.Data.Texture2DPtr.FilterType, value.Data.Texture2DPtr.WrapType);
		}
		else if (value.ValueType == ShaderVariableType::CubemapTexture)
		{
			SetTexture(
				value.ID, value.Data.CubemapTexturePtr.Ptr, value.Data.CubemapTexturePtr.FilterType, value.Data.CubemapTexturePtr.WrapType);
		}
	}
}

int32_t NativeShader_Imp::GetConstantBufferID(const char* name)
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

int32_t NativeShader_Imp::GetTextureID(const char* name)
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

void NativeShader_Imp::SetConstantBuffer(const char* name, const void* data, int32_t size)
{
	auto key = std::string(name);

	auto it_vs = m_vs_constantLayouts.find(key);
	auto it_ps = m_ps_constantLayouts.find(key);

	if (it_vs != m_vs_constantLayouts.end())
	{
		auto size_ = GetBufferSize(it_vs->second.Type, it_vs->second.Count);
		if (size != size_)
			return;

		memcpy(&(m_vertexConstantBuffer[it_vs->second.Offset]), data, size);
	}

	if (it_ps != m_ps_constantLayouts.end())
	{
		auto size_ = GetBufferSize(it_ps->second.Type, it_ps->second.Count);
		if (size != size_)
			return;

		memcpy(&(m_pixelConstantBuffer[it_ps->second.Offset]), data, size);
	}
}

void NativeShader_Imp::SetConstantBuffer(int32_t id, const void* data, int32_t size)
{
	assert(id < (int32_t)constantLayoutsArray.size());
	if (id < 0)
		return;

	auto& layout = constantLayoutsArray[id];

	if (id < m_vs_constantLayouts.size())
	{
		auto size_ = GetBufferSize(layout->Type, layout->Count);
		if (size != size_)
			return;

		memcpy(&(m_vertexConstantBuffer[layout->Offset]), data, size);
	}
	else
	{
		auto size_ = GetBufferSize(layout->Type, layout->Count);
		if (size != size_)
			return;

		memcpy(&(m_pixelConstantBuffer[layout->Offset]), data, size);
	}
}

void NativeShader_Imp::SetTexture(const char* name, Texture* texture, TextureFilterType filterType, TextureWrapType wrapType)
{
	auto key = std::string(name);
	auto g = (Graphics_Imp*)GetGraphics();

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

void NativeShader_Imp::SetTexture(int32_t id, Texture* texture, TextureFilterType filterType, TextureWrapType wrapType)
{
	assert(id < (int32_t)textureLayoutsArray.size());
	if (id < 0)
		return;

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

void NativeShader_Imp::AssignConstantBuffer()
{
	auto g = (Graphics_Imp*)GetGraphics();

	if (m_vertexConstantBuffer != nullptr)
	{
		rhiConstantVB->SetData(m_vertexConstantBuffer, vs_uniformBufferSize_, 0);
	}

	if (m_pixelConstantBuffer != nullptr)
	{
		rhiConstantPB->SetData(m_pixelConstantBuffer, ps_uniformBufferSize_, 0);
	}
}

NativeShader_Imp* NativeShader_Imp::Create(Graphics* graphics,
										   const char* vertexShaderText,
										   const char* vertexShaderFileName,
										   const char* pixelShaderText,
										   const char* pixelShaderFileName,
										   std::vector<VertexLayout>& layout,
										   bool is32Bit,
										   std::vector<Macro>& macro,
										   Log* log)
{
	auto g = (Graphics_Imp*)graphics;

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

		return new NativeShader_Imp(g, rhi, vcls, vs_uniformBufferSize, vtls, pcls, ps_uniformBufferSize, ptls);
	}

	asd::SafeDelete(rhi);
	return nullptr;
}

NativeShader_Imp* NativeShader_Imp::Create(Graphics* graphics,
										   const uint8_t* vertexShader,
										   int32_t vertexShaderSize,
										   const uint8_t* pixelShader,
										   int32_t pixelShaderSize,
										   std::vector<VertexLayout>& layout,
										   bool is32Bit)
{
	auto g = (Graphics_Imp*)graphics;

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

		return new NativeShader_Imp(g, rhi, vcls, vs_uniformBufferSize, vtls, pcls, ps_uniformBufferSize, ptls);
	}

	asd::SafeDelete(rhi);
	return nullptr;
}

} // namespace asd