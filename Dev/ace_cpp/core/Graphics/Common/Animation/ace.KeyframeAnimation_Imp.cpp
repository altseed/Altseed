
#include "ace.KeyframeAnimation_Imp.h"

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

	const double DEF_NaN = std::numeric_limits<double>::quiet_NaN();
	const double M_PI = 3.14159265358979;

	static float sqrt3(float value)
	{
		if (value == 0)
		{
			return 0;
		}
		else if (value > 0)
		{
			return (float) pow(value, 1.0f / 3.0f);
		}
		else
		{
			return -(float) pow(-value, 1.0f / 3.0f);
		}
	}

	static void NormalizeValues(float k1[], float k1rh[], float k2lh[], float k2[])
	{
		float h1[2];
		float h2[2];

		// 傾きを求める
		h1[0] = k1[0] - k1rh[0];
		h1[1] = k1[1] - k1rh[1];

		h2[0] = k2[0] - k2lh[0];
		h2[1] = k2[1] - k2lh[1];

		// キーフレーム間の長さ
		auto len = k2[0] - k1[0];

		// キー1の右手の長さ
		auto lenR = abs(h1[0]);

		// キー2の左手の長さ
		auto lenL = abs(h2[0]);

		// 長さがない
		if (lenR == 0 && lenL == 0) return;

		// 手が重なっている場合、補正
		if ((lenR + lenL) > len)
		{
			auto f = len / (lenR + lenL);

			k1rh[0] = (k1[0] - f * h1[0]);
			k1rh[1] = (k1[1] - f * h1[1]);

			k2lh[0] = (k2[0] - f * h2[0]);
			k2lh[1] = (k2[1] - f * h2[1]);
		}
	}

	static void QuadraticFormula(float a, float b, float c, float& r1, float& r2)
	{
		r1 = DEF_NaN;
		r2 = DEF_NaN;

		if (a != 0.0)
		{
			float p = b * b - 4 * a * c;

			if (p > 0)
			{
				p = sqrt(p);
				r1 = (-b - p) / (2 * a);
				r2 = (-b + p) / (2 * a);
			}
			else if (p == 0)
			{
				r1 = -b / (2 * a);
			}
		}
		else if (b != 0.0)
		{
			r1 = -c / b;
		}
		else if (c == 0.0f)
		{
			r1 = 0.0f;
		}
	}

	static bool CalcT(float frame, float k1X, float k1rhX, float k2lhX, float k2X, float& r)
	{
		auto isValid = [](float v) -> bool
		{
			if (_isnan(v)) return false;

			auto small_ = -0.00001f;
			auto big_ = 1.000001f;
			return (v >= small_) && (v <= big_);
		};

		auto c3_ = k2X - k1X + 3.0f * (k1rhX - k2lhX);
		auto c2_ = 3.0f * (k1X - 2.0f * k1rhX + k2lhX);
		auto c1_ = 3.0f * (k1rhX - k1X);
		auto c0_ = k1X - frame;

		if (c3_ != 0.0)
		{
			auto c2 = c2_ / c3_;
			auto c1 = c1_ / c3_;
			auto c0 = c0_ / c3_;

			auto p = c1 / 3.0f - c2 * c2 / (3.0f * 3.0f);
			auto q = (2.0f * c2 * c2 * c2 / (3.0f*3.0f*3.0f) - c2 / 3.0f * c1 + c0) / 2.0f;
			auto p3q2 = q * q + p * p * p;

			if (p3q2 > 0.0)
			{
				auto t = sqrt(p3q2);
				auto u = sqrt3(-q + t);
				auto v = sqrt3(-q - t);
				r = u + v - c2 / 3.0f;

				if (isValid(r)) return true;
				return false;
			}
			else if (p3q2 == 0.0)
			{
				auto u = sqrt3(-q);
				auto v = sqrt3(-q);
				r = u + v - c2 / 3.0f;
				if (isValid(r)) return true;

				u = -sqrt3(-q);
				v = -sqrt3(-q);
				r = u + v - c2 / 3.0f;
				if (isValid(r)) return true;

				return false;
			}
			else
			{
				// ビエタの解
				auto phi = acos(-q / sqrt(-(p * p * p)));
				auto pd3 = cos(phi / 3);
				auto rmp = sqrt(-p);

				r = 2.0f * rmp * cos(phi / 3) - c2 / 3.0f;
				if (isValid(r)) return true;

				auto q2 = sqrt(3 - 3 * pd3 * pd3);

				//r = -rmp * (pd3 + q2) - a;
				r = 2.0f * rmp * cos(phi / 3 + 2.0 * M_PI / 3.0) - c2 / 3.0f;
				if (isValid(r)) return true;

				//r = -rmp * (pd3 - q2) - a;
				r = 2.0f * rmp * cos(phi / 3 + 4.0 * M_PI / 3.0) - c2 / 3.0f;
				if (isValid(r)) return true;
				return false;
			}
		}
		else
		{
			// 2次方程式のケース
			float r1;
			float r2;
			QuadraticFormula(c2_, c1_, c0_, r1, r2);

			r = r1;
			if (isValid(r)) return true;

			r = r2;
			if (isValid(r)) return true;

			r = 0.0f;
			return false;
		}
	}

	static float CalcBezierValue(float k1Y, float k1rhY, float k2lhY, float k2Y, float t)
	{
		auto c0 = k1Y;
		auto c1 = 3.0f * (k1rhY - k1Y);
		auto c2 = 3.0f * (k1Y - 2.0f * k1rhY + k2lhY);
		auto c3 = k2Y - k1Y + 3.0f * (k1rhY - k2lhY);

		return c0 + t * c1 + t * t * c2 + t * t * t * c3;
	}


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

		auto strs = split(ToAString(name), ToAString("."));

		// ボーン向け設定
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

	const achar* KeyframeAnimation_Imp::GetTargetName()
	{
		return m_targetName.c_str();
	}

	eAnimationCurveTargetType KeyframeAnimation_Imp::GetTargetType()
	{
		return m_targetType;
	}

	eAnimationCurveTargetAxis KeyframeAnimation_Imp::GetTargetAxis()
	{
		return m_targetAxis;
	}

	void KeyframeAnimation_Imp::AddKeyframe(Keyframe kf)
	{
		m_keyframes.push_back(kf);
	}


	float KeyframeAnimation_Imp::GetValue(float frame)
	{
		if (m_keyframes.size() == 0) return 0;

		if (m_keyframes[0].KeyValue.X > frame) return m_keyframes[0].KeyValue.Y;

		if (m_keyframes[m_keyframes.size() - 1].KeyValue.X < frame) return m_keyframes[m_keyframes.size() - 1].KeyValue.Y;

		for (int32_t i = 0; i < m_keyframes.size() - 1; i++)
		{
			if (m_keyframes[i].KeyValue.X <= frame && frame <= m_keyframes[i + 1].KeyValue.X)
			{
				float k1[2];
				float k1rh[2];
				float k2lh[2];
				float k2[2];

				k1[0] = m_keyframes[i].KeyValue.X;
				k1[1] = m_keyframes[i].KeyValue.Y;

				k1rh[0] = m_keyframes[i].RightHandle.X;
				k1rh[1] = m_keyframes[i].RightHandle.Y;

				k2lh[0] = m_keyframes[i + 1].LeftHandle.X;
				k2lh[1] = m_keyframes[i + 1].LeftHandle.Y;

				k2[0] = m_keyframes[i + 1].KeyValue.X;
				k2[1] = m_keyframes[i + 1].KeyValue.Y;

				NormalizeValues(k1, k1rh, k2lh, k2);
				float t = 0;
				auto getT = CalcT(frame, k1[0], k1rh[0], k2lh[0], k2[0], t);
				if (getT)
				{
					return CalcBezierValue(k1[1], k1rh[1], k2lh[1], k2[1], t);
				}
				else
				{
					return 0;
				}
			}
		}

		return 0;
	}
}