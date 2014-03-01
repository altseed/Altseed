using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	/// <summary>
	/// 3×3行列を表す構造体
	/// </summary>
	[System.Runtime.InteropServices.StructLayout(System.Runtime.InteropServices.LayoutKind.Sequential)]
	public unsafe struct Matrix33
	{
		public fixed float Values[3*3];

		/// <summary>
		/// 単位行列を設定する。
		/// </summary>
		public void SetIdentity()
		{
			fixed(float* v = Values)
			{
				for (int i = 0; i < 3; ++i)
				{
					for (int j = 0; j < 3; ++j)
					{
						v[i*3+j] = i == j ? 1 : 0;
					}
				}
			}
		}

		/// <summary>
		/// 転置行列を設定する。
		/// </summary>
		public void SetTransposed()
		{
			fixed(float* v = Values)
			{
				for (int c = 0; c < 3; c++)
				{
					for (int r = 0; r < 3; r++)
					{
						float v_ = v[r*3+c];
						v[r*3+c] = v[c*3+r];
						v[c*3+r] = v_;
					}
				}
			}
		}

		/// <summary>
		/// 逆行列を設定する。
		/// </summary>
		public void SetInverted()
		{
			fixed(float* v = Values)
			{
			float a11 = v[0*3+0];
			float a12 = v[0*3+1];
			float a13 = v[0*3+2];
			float a21 = v[1*3+0];
			float a22 = v[1*3+1];
			float a23 = v[1*3+2];
			float a31 = v[2*3+0];
			float a32 = v[2*3+1];
			float a33 = v[2*3+2];

			/* 行列式の計算 */
			float b11 = +a22 * a33 - a23 * a32;
			float b12 = +a13 * a32 - a12 * a33;
			float b13 = +a12 * a23 - a13 * a22;

			float b21 = +a23 * a31 - a21 * a33;
			float b22 = +a11 * a33 - a13 * a31;
			float b23 = +a13 * a21 - a11 * a23;

			float b31 = +a21 * a32 - a22 * a31;
			float b32 = +a12 * a31 - a11 * a32;
			float b33 = +a11 * a22 - a12 * a21;

			// 行列式の逆数をかける
			float Det =
				a11 * a22 * a33 + a21 * a32 * a13 + a31 * a12 * a23 - a11 * a32 * a23 - a31 * a22 * a13 - a21 * a12 * a33;
			if ((-float.Epsilon <= Det) && (Det <= +float.Epsilon))
			{
				
			}

			float InvDet = 1.0f / Det;

			v[0*3+0] = b11 * InvDet;
			v[0*3+1] = b12 * InvDet;
			v[0*3+2] = b13 * InvDet;
			v[1*3+0] = b21 * InvDet;
			v[1*3+1] = b22 * InvDet;
			v[1*3+2] = b23 * InvDet;
			v[2*3+0] = b31 * InvDet;
			v[2*3+1] = b32 * InvDet;
			v[2*3+2] = b33 * InvDet;
			}
		}

		/// <summary>
		/// 逆行列を取得する。
		/// </summary>
		/// <returns></returns>
		Matrix33 GetInverted()
		{
			Matrix33 o = this;
			o.SetInverted();
			return o;
		}

		/// <summary>
		/// 平行移動の行列を設定する。
		/// </summary>
		/// <param name="x"></param>
		/// <param name="y"></param>
		public void SetTranslation(float x, float y)
		{
			SetIdentity();

			fixed(float* v = Values)
			{
			v[0*3+2] = x;
			v[1*3+2] = y;
			}
		}

		/// <summary>
		/// 回転行列を設定する。
		/// </summary>
		/// <param name="angle"></param>
		public void SetRotation(float angle)
		{
			SetIdentity();

			float sin = (float)Math.Sin(angle);
			float cos = (float)Math.Cos(angle);

			fixed(float* v = Values)
			{
			v[0*3+0] = cos;
			v[0*3+1] = -sin;
			v[1*3+0] = sin;
			v[1*3+1] = cos;
			}
		}

		/// <summary>
		/// 拡大・縮小行列を設定する。
		/// </summary>
		/// <param name="x"></param>
		/// <param name="y"></param>
		public void SetScale(float x, float y)
		{
			SetIdentity();

			fixed(float* v = Values)
			{
			v[0*3+0] = x;
			v[1*3+1] = y;
			}
		}

		/// <summary>
		/// 行列でベクトルを変形させる。
		/// </summary>
		/// <param name="in_">変形前ベクトル</param>
		/// <returns>変形後ベクトル</returns>
		public Vector2DF Transform2D(ref Vector2DF in_)
		{
			float* values = stackalloc float[4];

			fixed(float* v = Values)
			{
			for (int i = 0; i < 3; i++)
			{
				values[i] = 0;
				values[i] += in_.X * v[0*3+i];
				values[i] += in_.Y * v[1*3+i];
				values[i] += 1.0f * v[2*3+i];
			}
			}

			Vector2DF o;
			o.X = values[0];
			o.Y = values[1];
			return o;
		}

		/// <summary>
		/// 行列でベクトルを変形させる。
		/// </summary>
		/// <param name="in_">変形前ベクトル</param>
		/// <returns>変形後ベクトル</returns>
		public Vector3DF Transform3D(ref Vector3DF in_)
		{
			float* values = stackalloc float[4];

			fixed(float* v = Values)
			{
			for (int i = 0; i < 3; i++)
			{
				values[i] = 0;
				values[i] += in_.X * v[0*3+i];
				values[i] += in_.Y * v[1*3+i];
				values[i] += in_.Z * v[2*3+i];
			}
			}

			Vector3DF o;
			o.X = values[0];
			o.Y = values[1];
			o.Z = values[2];
			return o;
		}

		public static Matrix33 operator * (Matrix33 left, Matrix33 right)
		{
			Matrix33 result;

			for (int i = 0; i < 3; ++i)
			{
				for (int j = 0; j < 3; ++j)
				{
					result.Values[i*3+j] = 0;
					for (int k = 0; k < 3; ++k)
					{
						result.Values[i*3+j] += left.Values[i*3+k] * right.Values[k*3+j];
					}
				}
			}

			return result;
		}

		public static Vector3DF operator * (Matrix33 left, Vector3DF right)
		{
			float* elements = stackalloc float[3];
			elements[0] = 0;
			elements[1] = 0;
			elements[2] = 0;

			float* rop = stackalloc float[3];
			rop[0] = right.X;
			rop[1] = right.Y;
			rop[2] = right.Z;

			float* v = left.Values;
			{
				for (int i = 0; i < 3; ++i)
				{
					for (int k = 0; k < 3; ++k)
					{
						elements[i] += v[i * 3 + k] * rop[k];
					}
				}
			}

			Vector3DF result;
			result.X = elements[0];
			result.Y = elements[1];
			result.Z = elements[2];

			return result;
		}
	}
}
