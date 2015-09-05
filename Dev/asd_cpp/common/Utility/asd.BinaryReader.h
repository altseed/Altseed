#pragma once
#include<deque>
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
	std::deque<int8_t> data;
public:
	template<typename itrT> void ReadIn(itrT beg, itrT end)
	{
		data.assign(beg, end);
	}

	template<typename T> T Get(){ assert(!"The type cannot be serializable."); return T();};
	bool IsEmpty()const{ return data.empty(); }

	inline std::vector<uint8_t> Get(int32_t size)
	{
		std::vector<uint8_t> v;
		
		for (int i = 0; i < size; i++)
		{
			v.push_back(data.front());
			data.pop_front();
		}

		return v;
	}

};



template<> inline int32_t BinaryReader::Get()
{
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
}

template<> inline int16_t BinaryReader::Get()
{
	int8_t cs[2];
	for (int i = 0; i < 2; i++)
	{
		assert(!data.empty());
		if (data.empty()){
			return static_cast<int16_t>(0);
		}
		cs[i] = data.front();
		data.pop_front();
	}

	return *(static_cast<int16_t*>(static_cast<void*>(cs)));
}

template<> inline uint16_t BinaryReader::Get()
{
	int8_t cs[2];
	for (int i = 0; i < 2; i++)
	{
		assert(!data.empty());
		if (data.empty()){
			return static_cast<uint16_t>(0);
		}
		cs[i] = data.front();
		data.pop_front();
	}

	return *(static_cast<uint16_t*>(static_cast<void*>(cs)));
}


template<> inline int8_t BinaryReader::Get()
{
	int8_t c;
	assert(!data.empty());
	if (data.empty()){
		return static_cast<int8_t>(0);
	}
	c = data.front();
	data.pop_front();

	return c;
}

template<> inline uint8_t BinaryReader::Get()
{
	uint8_t c;
	assert(!data.empty());
	if (data.empty()){
		return static_cast<uint8_t>(0);
	}
	c = data.front();
	data.pop_front();

	return c;
}


template<> inline std::string BinaryReader::Get()
{
	int8_t lenCs[4];
	std::string str = std::string();
	for (int i = 0; i < 4; i++)
	{
		assert(!data.empty());
		if (data.empty()){
			return str;
		}
		lenCs[i] = data.front();
		data.pop_front();
	}

	int32_t const len = *(static_cast<int32_t*>(static_cast<void*>(lenCs)));

	int8_t chr;

	for (int32_t i = 0; i < len; i++)
	{

		assert(!data.empty());
		if (data.empty()){
			return str;
		}
		chr = data.front();
		data.pop_front();

		str.push_back(*(static_cast<char*>(static_cast<void*>(&chr))));

	}
	return str;
}
template<> inline astring BinaryReader::Get()
{
	int8_t lenCs[4];
	astring astr = astring();
	for (int i = 0; i < 4; i++)
	{
		assert(!data.empty());
		if (data.empty()){
			return astr;
		}
		lenCs[i] = data.front();
		data.pop_front();
	}

	int32_t const len =  *(static_cast<int32_t*>(static_cast<void*>(lenCs)));

	int8_t charCs[2];

	for (int32_t i = 0; i < len; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			assert(!data.empty());
			if (data.empty()){
				return astr;
			}
			charCs[j] = data.front();
			data.pop_front();
		}

		astr.push_back(*(static_cast<achar*>(static_cast<void*>(charCs))));

	}
	return astr;
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
	int8_t cs[4];
	for (int i = 0; i < 4; i++)
	{
		assert(!data.empty());
		if (data.empty()){
			return static_cast<float>(0);
		}
		cs[i] = data.front();
		data.pop_front();
	}

	return *(static_cast<float*>(static_cast<void*>(cs)));
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