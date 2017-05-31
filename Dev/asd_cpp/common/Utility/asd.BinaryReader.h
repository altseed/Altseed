#pragma once
#include<vector>
#include<fstream>
#include<string>
#include<cstring>
#include<cstdlib>
#include<algorithm>
#include<cstdint>
#include<climits>
#include<cassert>
#include "../asd.common.Base.h"
#include "../Graphics/asd.Graphics.Common.h"
#include "../Graphics/asd.Color.h"
#include "../Math/asd.Vector2DF.h"
#include "../Math/asd.Vector3DF.h"
#include "../Math/asd.Matrix44.h"

#include "../Math/asd.RectI.h"
#include "../Math/asd.RectF.h"

namespace asd
{

class BinaryReader
{
protected:
	std::vector<uint8_t> data;
	std::size_t idx;
public:
	template<typename itrT> void ReadIn(itrT beg, itrT end)
	{
		data.assign(beg, end);
		idx = 0;
	}

	bool ReadFile(std::string const& filename) {
		std::ifstream ifs(filename, std::ios_base::in | std::ios_base::binary);
		if (!ifs){
			return false;
		}
		ifs.seekg(0, ifs.end);
		std::size_t length = ifs.tellg();
		ifs.seekg(0, ifs.beg);
		data.reserve(length);
		ifs.read((char *)data.data(), length);
		return (bool)ifs;
	}

	template<typename T> inline T Get(){ assert(!"The type cannot be serializable."); return T();};
	bool IsEmpty()const{ return data.empty(); }

	inline std::vector<uint8_t> Get(int32_t size)
	{
		
		if (idx + size > data.size()) { return std::vector<uint8_t>(); }
		std::vector<uint8_t> v(size);
		/*
		for (int i = 0; i < size; i++)
		{
			v.push_back(data.front());
			data.pop_front();
		}
		*/
		std::memcpy(v.data(), data.data() + idx, size);
		idx += size;
		return v;
	}

};



template<> inline int32_t BinaryReader::Get()
{
	/*
	int8_t cs[4];
	for (int i = 0; i < 4; i++)
	{
		assert(!data.empty());
		if (data.empty()){
			return static_cast<int32_t>(0);
		}
		cs[i] = data.front();
		data.pop_front();
	}

	return *(static_cast<int32_t*>(static_cast<void*>(cs)));
	*/
	size_t const size = sizeof(int32_t);
	if (idx + size > data.size()) { assert(!"Lack of buffer (int32_t)."); return int32_t(); }
	int32_t v = 0;
	std::memcpy(&v, data.data() + idx, size);
	idx += size;
	return v;

}

template<> inline int16_t BinaryReader::Get()
{
	size_t const size = sizeof(int16_t);
	if (idx + size > data.size()) { assert(!"Lack of buffer (int16_t)."); return int16_t(); }
	int16_t v = 0;
	std::memcpy(&v, data.data() + idx, size);
	idx += size;
	return v;
}

template<> inline uint16_t BinaryReader::Get()
{
	size_t const size = sizeof(uint16_t);
	if (idx + size > data.size()) { assert(!"Lack of buffer (uint16_t)."); return uint16_t(); }
	uint16_t v = 0;
	std::memcpy(&v, data.data() + idx, size);
	idx += size;
	return v;
}


template<> inline int8_t BinaryReader::Get()
{
	size_t const size = sizeof(int8_t);
	if (idx + size > data.size()) { assert(!"Lack of buffer (int8_t)."); return int8_t(); }
	int8_t v = 0;
	std::memcpy(&v, data.data() + idx, size);
	idx += size;
	return v;
}

template<> inline uint8_t BinaryReader::Get()
{
	size_t const size = sizeof(uint8_t);
	if (idx + size > data.size()) { assert(!"Lack of buffer (uint8_t)."); return uint8_t(); }
	uint8_t v = 0;
	std::memcpy(&v, data.data() + idx, size);
	idx += size;
	return v;
}


template<> inline std::string BinaryReader::Get()
{
	int32_t const len = Get<int32_t>();
	size_t const size = sizeof(char);

	if (idx + size * len > data.size()) { assert(!"Lack of buffer (string)."); return std::string(); }

	std::string v = std::string((char const*)(data.data() + idx), (size_t)len);
	idx += size * len;
	return v;
}
template<> inline astring BinaryReader::Get()
{
	int32_t const len = Get<int32_t>();
	size_t const size = sizeof(achar);

	if (idx + size * len > data.size()) { assert(!"Lack of buffer (astring)."); return asd::astring(); }

	asd::astring v = asd::astring((achar const*)(data.data() + idx), (size_t)len);
	idx += size * len;
	return v;
}

//template<> inline asd::achar* BinaryReader::Get()
//{
//	int8_t lenCs[4];
//	for (int i = 0; i < 4; i++)
//	{
//		assert(!data.empty());
//		if (data.empty()){
//			return nullptr;
//		}
//		lenCs[i] = data.front();
//		data.pop_front();
//	}
//
//	int32_t const len = *(static_cast<int32_t*>(static_cast<void*>(lenCs)));
//
//	asd::achar *achs = static_cast<asd::achar*>(calloc(len + 1, sizeof(asd::achar)));
//	int8_t charCs[2];
//
//	for (int32_t i = 0; i < len; i++)
//	{
//		for (int j = 0; j < 2; j++)
//		{
//			assert(!data.empty());
//			if (data.empty()){
//				return achs;
//			}
//			charCs[j] = data.front();
//			data.pop_front();
//		}
//
//		achs[i] = *(static_cast<asd::achar*>(static_cast<void*>(charCs)));
//
//	}
//	return achs;
//}

template<> inline float BinaryReader::Get()
{
	size_t const size = sizeof(float);
	if (idx + size > data.size()) { assert(!"Lack of buffer (float)."); return float(); }
	float v = 0.0f;
	std::memcpy(&v, data.data() + idx, size);
	idx += size;
	return v;
}

template<> inline bool BinaryReader::Get()
{
	size_t const size = sizeof(bool);
	if (idx + size > data.size()) { assert(!"Lack of buffer (bool)."); return bool(); }
	bool v = false;
	std::memcpy(&v, data.data() + idx, size);
	idx += size;
	return v;
}


template<> inline Vector3DF BinaryReader::Get()
{
	Vector3DF v;
	v.X = Get<float>();
	v.Y = Get<float>();
	v.Z = Get<float>();

	return v;

}

template<> inline Vector2DF BinaryReader::Get()
{
	Vector2DF v;
	v.X = Get<float>();
	v.Y = Get<float>();

	return v;
}

template<> inline Color BinaryReader::Get()
{
	Color c;
	c.R = Get<uint8_t>();
	c.G = Get<uint8_t>();
	c.B = Get<uint8_t>();
	c.A = Get<uint8_t>();

	return c;
}

template<> inline RectI BinaryReader::Get()
{
	RectI v;
	v.X = Get<int32_t>();
	v.Y = Get<int32_t>();
	v.Width = Get<int32_t>();
	v.Height = Get<int32_t>();
	return v;
}

template<> inline RectF BinaryReader::Get()
{
	RectF v;
	v.X = Get<float>();
	v.Y = Get<float>();
	v.Width = Get<float>();
	v.Height = Get<float>();
	return v;
}

template<> inline FCurveKeyframe BinaryReader::Get()
{
	FCurveKeyframe keyframe;

	keyframe.KeyValue = Get<Vector2DF>();
	keyframe.LeftHandle = Get<Vector2DF>();
	keyframe.RightHandle = Get<Vector2DF>();
	keyframe.Interpolation = (InterpolationType)Get<int32_t>();

	return keyframe;
}

template<> inline RotationOrder BinaryReader::Get()
{
	RotationOrder rotationOrder;

	rotationOrder = (RotationOrder)Get<int32_t>();

	return rotationOrder;
}

template<> inline Matrix44 BinaryReader::Get()
{
	Matrix44 m;
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			m.Values[j][i] = Get<float>();
		}
	}

	return m;

}

} 
