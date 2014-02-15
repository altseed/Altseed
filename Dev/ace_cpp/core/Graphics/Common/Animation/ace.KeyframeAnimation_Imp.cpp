
#include "ace.KeyframeAnimation_Imp.h"

#include <cmath>

namespace ace
{
	/**
		@brief	文字列分割
		@note
		http://shnya.jp/blog/?p=195 のコードを改造
	*/
	static std::vector<astring> split(const astring &str, const astring &delim)
	{
		std::vector<astring> res;
		size_t current = 0, found, delimlen = delim.size();
		while ((found = str.find(delim, current)) != astring::npos)
		{
			res.push_back(astring(str, current, found - current));
			current = found + delimlen;
		}
		res.push_back(astring(str, current, str.size() - current));
		return res;
	}

	static const double DEF_NaN = std::numeric_limits<double>::quiet_NaN();

#if _WIN32
	static const double M_PI = 3.14159265358979;
#endif

	KeyframeAnimation_Imp::KeyframeAnimation_Imp()
		: m_targetType(ANIMATION_CURVE_TARGET_TYPE_NONE)
		, m_targetAxis(ANIMATION_CURVE_TARGET_AXIS_NONE)
	{
	
	}

	KeyframeAnimation_Imp::~KeyframeAnimation_Imp()
	{
	
	}

	const achar* KeyframeAnimation_Imp::GetName()
	{
		return m_name.c_str();
	}

	void KeyframeAnimation_Imp::SetName(const achar* name)
	{
		m_name = name;

		auto strs = split(astring(name), ToAString("."));

		// ボーン向け設定の取得
		m_targetName = astring();
		m_targetType = ANIMATION_CURVE_TARGET_TYPE_NONE;
		m_targetAxis = ANIMATION_CURVE_TARGET_AXIS_NONE;

		if (strs.size() < 3) return;
		
		for (size_t i = 0; i < strs.size() - 2; i++)
		{
			m_targetName += strs[i];

			if (i != strs.size() - 3)
			{
				m_targetName += ToAString(".");
			}
		}

		if (strs[strs.size() - 2] == ToAString("pos"))
		{
			m_targetType = ANIMATION_CURVE_TARGET_TYPE_POSITON;
		}

		if (strs[strs.size() - 2] == ToAString("rot"))
		{
			m_targetType = ANIMATION_CURVE_TARGET_TYPE_ROTATION;
		}

		if (strs[strs.size() - 2] == ToAString("scl"))
		{
			m_targetType = ANIMATION_CURVE_TARGET_TYPE_SCALE;
		}

		if (strs[strs.size() - 1] == ToAString("x"))
		{
			m_targetAxis = ANIMATION_CURVE_TARGET_AXIS_X;
		}

		if (strs[strs.size() - 1] == ToAString("y"))
		{
			m_targetAxis = ANIMATION_CURVE_TARGET_AXIS_Y;
		}

		if (strs[strs.size() - 1] == ToAString("z"))
		{
			m_targetAxis = ANIMATION_CURVE_TARGET_AXIS_Z;
		}

		if (strs[strs.size() - 1] == ToAString("w"))
		{
			m_targetAxis = ANIMATION_CURVE_TARGET_AXIS_W;
		}
	}

	astring& KeyframeAnimation_Imp::GetTargetName()
	{
		return m_targetName;
	}

	eAnimationCurveTargetType KeyframeAnimation_Imp::GetTargetType()
	{
		return m_targetType;
	}

	eAnimationCurveTargetAxis KeyframeAnimation_Imp::GetTargetAxis()
	{
		return m_targetAxis;
	}

	void KeyframeAnimation_Imp::AddKeyframe(const FCurveKeyframe& kf)
	{
		m_keyframes.push_back(kf);
	}

	float KeyframeAnimation_Imp::GetValue(float time)
	{
		return ModelUtils::GetKeyframeValue(time, m_keyframes);
	}
}
