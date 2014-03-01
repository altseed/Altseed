using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	/// <summary>
	/// 4×4行列を表す構造体
	/// </summary>
	[System.Runtime.InteropServices.StructLayout(System.Runtime.InteropServices.LayoutKind.Sequential)]
	public unsafe struct Matrix44
	{
		public fixed float Values[4 * 4];

		/// <summary>
		/// 単位行列を設定する。
		/// </summary>
		public void SetIndentity()
		{
			fixed (float* v = Values)
			{
				v[0 * 4 + 0] = 1.0f;
				v[0 * 4 + 1] = 0.0f;
				v[0 * 4 + 2] = 0.0f;
				v[0 * 4 + 3] = 0.0f;

				v[1 * 4 + 0] = 0.0f;
				v[1 * 4 + 1] = 1.0f;
				v[1 * 4 + 2] = 0.0f;
				v[1 * 4 + 3] = 0.0f;

				v[2 * 4 + 0] = 0.0f;
				v[2 * 4 + 1] = 0.0f;
				v[2 * 4 + 2] = 1.0f;
				v[2 * 4 + 3] = 0.0f;

				v[3 * 4 + 0] = 0.0f;
				v[3 * 4 + 1] = 0.0f;
				v[3 * 4 + 2] = 0.0f;
				v[3 * 4 + 3] = 1.0f;
			}
		}

		/// <summary>
		/// 転置行列を設定する。
		/// </summary>
		public void SetTransposed()
		{
			fixed (float* v = Values)
			{
				for (int c = 0; c < 4; c++)
				{
					for (int r = 0; r < 4; r++)
					{
						float v_ = v[r * 4 + c];
						v[r * 4 + c] = v[c * 4 + r];
						v[c * 4 + r] = v_;
					}
				}
			}
		}

		/// <summary>
		/// 逆行列を設定する。
		/// </summary>
		public void SetInverted()
		{
			fixed (float* v = Values)
			{
				float a11 = v[0 * 4 + 0];
				float a12 = v[0 * 4 + 1];
				float a13 = v[0 * 4 + 2];
				float a14 = v[0 * 4 + 3];
				float a21 = v[1 * 4 + 0];
				float a22 = v[1 * 4 + 1];
				float a23 = v[1 * 4 + 2];
				float a24 = v[1 * 4 + 3];
				float a31 = v[2 * 4 + 0];
				float a32 = v[2 * 4 + 1];
				float a33 = v[2 * 4 + 2];
				float a34 = v[2 * 4 + 3];
				float a41 = v[3 * 4 + 0];
				float a42 = v[3 * 4 + 1];
				float a43 = v[3 * 4 + 2];
				float a44 = v[3 * 4 + 3];

				/* 行列式の計算 */
				float b11 = +a22 * (a33 * a44 - a43 * a34) - a23 * (a32 * a44 - a42 * a34) + a24 * (a32 * a43 - a42 * a33);
				float b12 = -a12 * (a33 * a44 - a43 * a34) + a13 * (a32 * a44 - a42 * a34) - a14 * (a32 * a43 - a42 * a33);
				float b13 = +a12 * (a23 * a44 - a43 * a24) - a13 * (a22 * a44 - a42 * a24) + a14 * (a22 * a43 - a42 * a23);
				float b14 = -a12 * (a23 * a34 - a33 * a24) + a13 * (a22 * a34 - a32 * a24) - a14 * (a22 * a33 - a32 * a23);

				float b21 = -a21 * (a33 * a44 - a43 * a34) + a23 * (a31 * a44 - a41 * a34) - a24 * (a31 * a43 - a41 * a33);
				float b22 = +a11 * (a33 * a44 - a43 * a34) - a13 * (a31 * a44 - a41 * a34) + a14 * (a31 * a43 - a41 * a33);
				float b23 = -a11 * (a23 * a44 - a43 * a24) + a13 * (a21 * a44 - a41 * a24) - a14 * (a21 * a43 - a41 * a23);
				float b24 = +a11 * (a23 * a34 - a33 * a24) - a13 * (a21 * a34 - a31 * a24) + a14 * (a21 * a33 - a31 * a23);

				float b31 = +a21 * (a32 * a44 - a42 * a34) - a22 * (a31 * a44 - a41 * a34) + a24 * (a31 * a42 - a41 * a32);
				float b32 = -a11 * (a32 * a44 - a42 * a34) + a12 * (a31 * a44 - a41 * a34) - a14 * (a31 * a42 - a41 * a32);
				float b33 = +a11 * (a22 * a44 - a42 * a24) - a12 * (a21 * a44 - a41 * a24) + a14 * (a21 * a42 - a41 * a22);
				float b34 = -a11 * (a22 * a34 - a32 * a24) + a12 * (a21 * a34 - a31 * a24) - a14 * (a21 * a32 - a31 * a22);

				float b41 = -a21 * (a32 * a43 - a42 * a33) + a22 * (a31 * a43 - a41 * a33) - a23 * (a31 * a42 - a41 * a32);
				float b42 = +a11 * (a32 * a43 - a42 * a33) - a12 * (a31 * a43 - a41 * a33) + a13 * (a31 * a42 - a41 * a32);
				float b43 = -a11 * (a22 * a43 - a42 * a23) + a12 * (a21 * a43 - a41 * a23) - a13 * (a21 * a42 - a41 * a22);
				float b44 = +a11 * (a22 * a33 - a32 * a23) - a12 * (a21 * a33 - a31 * a23) + a13 * (a21 * a32 - a31 * a22);

				// 行列式の逆数をかける
				float Det = (a11 * b11) + (a12 * b21) + (a13 * b31) + (a14 * b41);
				if ((-float.Epsilon <= Det) && (Det <= +float.Epsilon))
				{
					return;
				}

				float InvDet = 1.0f / Det;

				v[0 * 4 + 0] = b11 * InvDet;
				v[0 * 4 + 1] = b12 * InvDet;
				v[0 * 4 + 2] = b13 * InvDet;
				v[0 * 4 + 3] = b14 * InvDet;
				v[1 * 4 + 0] = b21 * InvDet;
				v[1 * 4 + 1] = b22 * InvDet;
				v[1 * 4 + 2] = b23 * InvDet;
				v[1 * 4 + 3] = b24 * InvDet;
				v[2 * 4 + 0] = b31 * InvDet;
				v[2 * 4 + 1] = b32 * InvDet;
				v[2 * 4 + 2] = b33 * InvDet;
				v[2 * 4 + 3] = b34 * InvDet;
				v[3 * 4 + 0] = b41 * InvDet;
				v[3 * 4 + 1] = b42 * InvDet;
				v[3 * 4 + 2] = b43 * InvDet;
				v[3 * 4 + 3] = b44 * InvDet;
			}
		}

		/// <summary>
		/// 逆行列を取得する。
		/// </summary>
		/// <returns></returns>
		public Matrix44 GetInverted()
		{
			Matrix44 o = this;
			o.SetInverted();
			return o;
		}

		/// <summary>
		/// カメラ行列(右手系)を設定する。
		/// </summary>
		/// <param name="eye">カメラの位置</param>
		/// <param name="at">カメラの注視点</param>
		/// <param name="up">カメラの上方向</param>
		public void SetLookAtRH(ref Vector3DF eye, ref Vector3DF at, ref Vector3DF up)
		{
			// F=正面、R=右方向、U=上方向
			Vector3DF F = (eye - at).Normal;
			Vector3DF R = Vector3DF.Cross(ref up, ref F).Normal;
			Vector3DF U = Vector3DF.Cross(ref F, ref R).Normal;

			fixed (float* v = Values)
			{
				v[0 * 4 + 0] = R.X;
				v[0 * 4 + 1] = R.Y;
				v[0 * 4 + 2] = R.Z;
				v[0 * 4 + 3] = 0.0f;

				v[1 * 4 + 0] = U.X;
				v[1 * 4 + 1] = U.Y;
				v[1 * 4 + 2] = U.Z;
				v[1 * 4 + 3] = 0.0f;

				v[2 * 4 + 0] = F.X;
				v[2 * 4 + 1] = F.Y;
				v[2 * 4 + 2] = F.Z;
				v[2 * 4 + 3] = 0.0f;

				v[0 * 4 + 3] = -Vector3DF.Dot(ref R, ref eye);
				v[1 * 4 + 3] = -Vector3DF.Dot(ref U, ref eye);
				v[2 * 4 + 3] = -Vector3DF.Dot(ref F, ref eye);
				v[3 * 4 + 3] = 1.0f;
			}
		}

		/// <summary>
		/// カメラ行列(左手系)を設定する。
		/// </summary>
		/// <param name="eye">カメラの位置</param>
		/// <param name="at">カメラの注視点</param>
		/// <param name="up">カメラの上方向</param>
		public void SetLookAtLH(ref Vector3DF eye, ref Vector3DF at, ref Vector3DF up)
		{
			// F=正面、R=右方向、U=上方向
			Vector3DF F = (at - eye).Normal;
			Vector3DF R = Vector3DF.Cross(ref up, ref F).Normal;
			Vector3DF U = Vector3DF.Cross(ref F, ref R).Normal;

			fixed (float* v = Values)
			{
				v[0 * 4 + 0] = R.X;
				v[0 * 4 + 1] = R.Y;
				v[0 * 4 + 2] = R.Z;
				v[0 * 4 + 3] = 0.0f;

				v[1 * 4 + 0] = U.X;
				v[1 * 4 + 1] = U.Y;
				v[1 * 4 + 2] = U.Z;
				v[1 * 4 + 3] = 0.0f;

				v[2 * 4 + 0] = F.X;
				v[2 * 4 + 1] = F.Y;
				v[2 * 4 + 2] = F.Z;
				v[2 * 4 + 3] = 0.0f;

				v[0 * 4 + 3] = -Vector3DF.Dot(ref R, ref eye);
				v[1 * 4 + 3] = -Vector3DF.Dot(ref U, ref eye);
				v[2 * 4 + 3] = -Vector3DF.Dot(ref F, ref eye);
				v[3 * 4 + 3] = 1.0f;
			}
		}

		/// <summary>
		/// 射影行列(右手系)を設定する。
		/// </summary>
		/// <param name="ovY">Y方向への視野角(ラジアン)</param>
		/// <param name="aspect">画面のアスペクト比</param>
		/// <param name="zn">最近距離</param>
		/// <param name="zf">最遠距離</param>
		public void SetPerspectiveFovRH(float ovY, float aspect, float zn, float zf)
		{
			float yScale = 1.0f / (float)Math.Tan(ovY / 2);
			float xScale = yScale / aspect;

			fixed (float* v = Values)
			{
				v[0 * 4 + 0] = xScale;
				v[1 * 4 + 0] = 0;
				v[2 * 4 + 0] = 0;
				v[3 * 4 + 0] = 0;

				v[0 * 4 + 1] = 0;
				v[1 * 4 + 1] = yScale;
				v[2 * 4 + 1] = 0;
				v[3 * 4 + 1] = 0;

				v[0 * 4 + 2] = 0;
				v[1 * 4 + 2] = 0;
				v[2 * 4 + 2] = zf / (zn - zf);
				v[3 * 4 + 2] = -1;

				v[0 * 4 + 3] = 0;
				v[1 * 4 + 3] = 0;
				v[2 * 4 + 3] = zn * zf / (zn - zf);
				v[3 * 4 + 3] = 0;
			}
		}

		/// <summary>
		/// OpenGL用射影行列(右手系)を設定する。
		/// </summary>
		/// <param name="ovY">Y方向への視野角(ラジアン)</param>
		/// <param name="aspect">画面のアスペクト比</param>
		/// <param name="zn">最近距離</param>
		/// <param name="zf">最遠距離</param>
		public void SetPerspectiveFovRH_OpenGL(float ovY, float aspect, float zn, float zf)
		{
			float yScale = 1.0f / (float)Math.Tan(ovY / 2);
			float xScale = yScale / aspect;
			float dz = zf - zn;

			fixed (float* v = Values)
			{
				v[0 * 4 + 0] = xScale;
				v[1 * 4 + 0] = 0;
				v[2 * 4 + 0] = 0;
				v[3 * 4 + 0] = 0;

				v[0 * 4 + 1] = 0;
				v[1 * 4 + 1] = yScale;
				v[2 * 4 + 1] = 0;
				v[3 * 4 + 1] = 0;

				v[0 * 4 + 2] = 0;
				v[1 * 4 + 2] = 0;
				v[2 * 4 + 2] = -(zf + zn) / dz;
				v[3 * 4 + 2] = -1.0f;

				v[0 * 4 + 3] = 0;
				v[1 * 4 + 3] = 0;
				v[2 * 4 + 3] = -2.0f * zn * zf / dz;
				v[3 * 4 + 3] = 0.0f;
			}
		}

		/// <summary>
		/// 射影行列(左手系)を設定する。
		/// </summary>
		/// <param name="ovY">Y方向への視野角(ラジアン)</param>
		/// <param name="aspect">画面のアスペクト比</param>
		/// <param name="zn">最近距離</param>
		/// <param name="zf">最遠距離</param>
		public void SetPerspectiveFovLH(float ovY, float aspect, float zn, float zf)
		{
			float yScale = 1.0f / (float)Math.Tan(ovY / 2);
			float xScale = yScale / aspect;

			fixed (float* v = Values)
			{
				v[0 * 4 + 0] = xScale;
				v[1 * 4 + 0] = 0;
				v[2 * 4 + 0] = 0;
				v[3 * 4 + 0] = 0;

				v[0 * 4 + 1] = 0;
				v[1 * 4 + 1] = yScale;
				v[2 * 4 + 1] = 0;
				v[3 * 4 + 1] = 0;

				v[0 * 4 + 2] = 0;
				v[1 * 4 + 2] = 0;
				v[2 * 4 + 2] = zf / (zf - zn);
				v[3 * 4 + 2] = 1;

				v[0 * 4 + 3] = 0;
				v[1 * 4 + 3] = 0;
				v[2 * 4 + 3] = -zn * zf / (zf - zn);
				v[3 * 4 + 3] = 0;
			}
		}

		/// <summary>
		/// 正射影行列(右手系)を設定する。
		/// </summary>
		/// <param name="width">横幅</param>
		/// <param name="height">縦幅</param>
		/// <param name="zn">最近距離</param>
		/// <param name="zf">最遠距離</param>
		public void SetOrthographicRH(float width, float height, float zn, float zf)
		{
			fixed (float* v = Values)
			{
				v[0 * 4 + 0] = 2 / width;
				v[1 * 4 + 0] = 0;
				v[2 * 4 + 0] = 0;
				v[3 * 4 + 0] = 0;

				v[0 * 4 + 1] = 0;
				v[1 * 4 + 1] = 2 / height;
				v[2 * 4 + 1] = 0;
				v[3 * 4 + 1] = 0;

				v[0 * 4 + 2] = 0;
				v[1 * 4 + 2] = 0;
				v[2 * 4 + 2] = 1 / (zn - zf);
				v[3 * 4 + 2] = 0;

				v[0 * 4 + 3] = 0;
				v[1 * 4 + 3] = 0;
				v[2 * 4 + 3] = zn / (zn - zf);
				v[3 * 4 + 3] = 1;
			}
		}

		/// <summary>
		/// 正射影行列(左手系)を設定する。
		/// </summary>
		/// <param name="width">横幅</param>
		/// <param name="height">縦幅</param>
		/// <param name="zn">最近距離</param>
		/// <param name="zf">最遠距離</param>
		public void SetOrthographicLH(float width, float height, float zn, float zf)
		{
			fixed (float* v = Values)
			{
				v[0 * 4 + 0] = 2 / width;
				v[1 * 4 + 0] = 0;
				v[2 * 4 + 0] = 0;
				v[3 * 4 + 0] = 0;

				v[0 * 4 + 1] = 0;
				v[1 * 4 + 1] = 2 / height;
				v[2 * 4 + 1] = 0;
				v[3 * 4 + 1] = 0;

				v[0 * 4 + 2] = 0;
				v[1 * 4 + 2] = 0;
				v[2 * 4 + 2] = 1 / (zf - zn);
				v[3 * 4 + 2] = 0;

				v[0 * 4 + 3] = 0;
				v[1 * 4 + 3] = 0;
				v[2 * 4 + 3] = zn / (zn - zf);
				v[3 * 4 + 3] = 1;
			}
		}

		/// <summary>
		/// 並行移動行列を設定する。
		/// </summary>
		/// <param name="x">X方向移動量</param>
		/// <param name="y">Y方向移動量</param>
		/// <param name="z">Z方向移動量</param>
		public void SetTranslation(float x, float y, float z)
		{
			SetIndentity();

			fixed (float* v = Values)
			{
				v[0 * 4 + 3] = x;
				v[1 * 4 + 3] = y;
				v[2 * 4 + 3] = z;
			}
		}

		/// <summary>
		/// X軸回転行列(右手)を設定する。
		/// </summary>
		/// <param name="angle">X軸回転量(ラジアン)</param>
		public void SetRotationX(float angle)
		{
			float c, s;
			s = (float)Math.Sin(angle);
			c = (float)Math.Cos(angle);

			fixed (float* v = Values)
			{
				v[0 * 4 + 0] = 1.0f;
				v[1 * 4 + 0] = 0.0f;
				v[2 * 4 + 0] = 0.0f;
				v[3 * 4 + 0] = 0.0f;

				v[0 * 4 + 1] = 0.0f;
				v[1 * 4 + 1] = c;
				v[2 * 4 + 1] = s;
				v[3 * 4 + 1] = 0.0f;

				v[0 * 4 + 2] = 0.0f;
				v[1 * 4 + 2] = -s;
				v[2 * 4 + 2] = c;
				v[3 * 4 + 2] = 0.0f;

				v[0 * 4 + 3] = 0.0f;
				v[1 * 4 + 3] = 0.0f;
				v[2 * 4 + 3] = 0.0f;
				v[3 * 4 + 3] = 1.0f;
			}
		}

		/// <summary>
		/// Y軸回転行列(右手)を設定する。
		/// </summary>
		/// <param name="angle">Y軸回転量(ラジアン)</param>
		public void SetRotationY(float angle)
		{
			float c, s;
			s = (float)Math.Sin(angle);
			c = (float)Math.Cos(angle);


			fixed (float* v = Values)
			{
				v[0 * 4 + 0] = c;
				v[1 * 4 + 0] = 0.0f;
				v[2 * 4 + 0] = -s;
				v[3 * 4 + 0] = 0.0f;

				v[0 * 4 + 1] = 0.0f;
				v[1 * 4 + 1] = 1.0f;
				v[2 * 4 + 1] = 0.0f;
				v[3 * 4 + 1] = 0.0f;

				v[0 * 4 + 2] = s;
				v[1 * 4 + 2] = 0.0f;
				v[2 * 4 + 2] = c;
				v[3 * 4 + 2] = 0.0f;

				v[0 * 4 + 3] = 0.0f;
				v[1 * 4 + 3] = 0.0f;
				v[2 * 4 + 3] = 0.0f;
				v[3 * 4 + 3] = 1.0f;
			}
		}

		/// <summary>
		/// Z軸回転行列(右手)を設定する。
		/// </summary>
		/// <param name="angle">Z軸回転量(ラジアン)</param>
		public void SetRotationZ(float angle)
		{
			float c, s;
			s = (float)Math.Sin(angle);
			c = (float)Math.Cos(angle);


			fixed (float* v = Values)
			{
				v[0 * 4 + 0] = c;
				v[1 * 4 + 0] = s;
				v[2 * 4 + 0] = 0.0f;
				v[3 * 4 + 0] = 0.0f;

				v[0 * 4 + 1] = -s;
				v[1 * 4 + 1] = c;
				v[2 * 4 + 1] = 0.0f;
				v[3 * 4 + 1] = 0.0f;

				v[0 * 4 + 2] = 0.0f;
				v[1 * 4 + 2] = 0.0f;
				v[2 * 4 + 2] = 1;
				v[3 * 4 + 2] = 0.0f;

				v[0 * 4 + 3] = 0.0f;
				v[1 * 4 + 3] = 0.0f;
				v[2 * 4 + 3] = 0.0f;
				v[3 * 4 + 3] = 1.0f;
			}
		}

		/// <summary>
		/// 任意軸の反時計回転行列(右手)を設定する。
		/// </summary>
		/// <param name="axis">軸</param>
		/// <param name="angle">回転量(ラジアン)</param>
		public void SetRotationAxis(ref Vector3DF axis, float angle)
		{
			float s = (float)Math.Sin(angle);
			float c = (float)Math.Cos(angle);
			float cc = 1.0f - c;

			fixed (float* v = Values)
			{
				v[0 * 4 + 0] = cc * (axis.X * axis.X) + c;
				v[1 * 4 + 0] = cc * (axis.X * axis.Y) + (axis.Z * s);
				v[2 * 4 + 0] = cc * (axis.Z * axis.X) - (axis.Y * s);

				v[0 * 4 + 1] = cc * (axis.X * axis.Y) - (axis.Z * s);
				v[1 * 4 + 1] = cc * (axis.Y * axis.Y) + c;
				v[2 * 4 + 1] = cc * (axis.Y * axis.Z) + (axis.X * s);

				v[0 * 4 + 2] = cc * (axis.Z * axis.X) + (axis.Y * s);
				v[1 * 4 + 2] = cc * (axis.Y * axis.Z) - (axis.X * s);
				v[2 * 4 + 2] = cc * (axis.Z * axis.Z) + c;

				v[0 * 4 + 3] = 0.0f;
				v[1 * 4 + 3] = 0.0f;
				v[2 * 4 + 3] = 0.0f;
			}
		}

		/// <summary>
		/// クオータニオンを元に回転行列(右手)を設定する。
		/// </summary>
		/// <param name="x">クオータニオン</param>
		/// <param name="y">クオータニオン</param>
		/// <param name="z">クオータニオン</param>
		/// <param name="w">クオータニオン</param>
		public void SetQuaternion(float x, float y, float z, float w)
		{
			float xx = x * x;
			float yy = y * y;
			float zz = z * z;
			float xy = x * y;
			float xz = x * z;
			float yz = y * z;
			float wx = w * x;
			float wy = w * y;
			float wz = w * z;

			fixed (float* v = Values)
			{
				v[0 * 4 + 0] = 1.0f - 2.0f * (yy + zz);
				v[0 * 4 + 1] = 2.0f * (xy - wz);
				v[0 * 4 + 2] = 2.0f * (xz + wy);
				v[0 * 4 + 3] = 0.0f;

				v[1 * 4 + 0] = 2.0f * (xy + wz);
				v[1 * 4 + 1] = 1.0f - 2.0f * (xx + zz);
				v[1 * 4 + 2] = 2.0f * (yz - wx);
				v[1 * 4 + 3] = 0.0f;

				v[2 * 4 + 0] = 2.0f * (xz - wy);
				v[2 * 4 + 1] = 2.0f * (yz + wx);
				v[2 * 4 + 2] = 1.0f - 2.0f * (xx + yy);
				v[2 * 4 + 3] = 0.0f;

				v[3 * 4 + 0] = 0.0f;
				v[3 * 4 + 1] = 0.0f;
				v[3 * 4 + 2] = 0.0f;
				v[3 * 4 + 3] = 1.0f;
			}
		}

		/// <summary>
		/// 拡大行列を設定する。
		/// </summary>
		/// <param name="x">X方向拡大率</param>
		/// <param name="y">Y方向拡大率</param>
		/// <param name="z">Z方向拡大率</param>
		public void SetScale(float x, float y, float z)
		{
			SetIndentity();

			fixed (float* v = Values)
			{
				v[0 * 4 + 0] = x;
				v[1 * 4 + 1] = y;
				v[2 * 4 + 2] = z;
				v[3 * 4 + 3] = 1.0f;
			}
		}

		/// <summary>
		/// 行列でベクトルを変形させる。
		/// </summary>
		/// <param name="in_">変形前ベクトル</param>
		/// <returns>変形後ベクトル</returns>
		public Vector3DF Transform3D(ref Vector3DF in_)
		{
			float* values = stackalloc float[3];

			fixed (float* v = Values)
			{
				for (int i = 0; i < 3; i++)
				{
					values[i] = 0;
					values[i] += in_.X * v[0 * 4 + i];
					values[i] += in_.Y * v[1 * 4 + i];
					values[i] += in_.Z * v[2 * 4 + i];
					values[i] += v[3 * 4 + i];
				}
			}

			Vector3DF o;
			o.X = values[0];
			o.Y = values[1];
			o.Z = values[2];
			return o;
		}

		/// <summary>
		/// 行列でベクトルを変形させる。
		/// </summary>
		/// <param name="in_">変形前ベクトル</param>
		/// <returns>変形後ベクトル</returns>
		Vector4DF Transform4D(ref Vector4DF in_)
		{
			float* values = stackalloc float[4];

			fixed (float* v = Values)
			{
				for (int i = 0; i < 4; i++)
				{
					values[i] = 0;
					values[i] += in_.X * v[0 * 4 + i];
					values[i] += in_.Y * v[1 * 4 + i];
					values[i] += in_.Z * v[2 * 4 + i];
					values[i] += in_.W * v[3 * 4 + i];
				}
			}

			Vector4DF o;
			o.X = values[0];
			o.Y = values[1];
			o.Z = values[2];
			o.W = values[3];
			return o;
		}

		public static Matrix44 operator *(Matrix44 left, Matrix44 right)
		{
			Matrix44 o_;
			Mul(ref o_, ref left, ref right);
			return o_;
		}

		public static Vector3DF operator *(Matrix44 left, Vector3DF right)
		{
			return left.Transform3D(ref right);
		}

		public static Vector4DF operator *(Matrix44 left, Vector4DF right)
		{
			return left.Transform4D(ref right);
		}

		/// <summary>
		/// 乗算を行う。
		/// </summary>
		/// <param name="o">出力先</param>
		/// <param name="in1">行列1</param>
		/// <param name="in2">行列2</param>
		public static void Mul(ref Matrix44 o, ref Matrix44 in1, ref Matrix44 in2)
		{
			Matrix44 _in1 = in1;
			Matrix44 _in2 = in2;

			fixed (float* vo = o.Values)
			{
				fixed (float* v1 = in1.Values)
				{
					fixed (float* v2 = in2.Values)
					{
						for (int i = 0; i < 4; i++)
						{
							for (int j = 0; j < 4; j++)
							{
								float v = 0.0f;
								for (int k = 0; k < 4; k++)
								{
									v += v1[i * 4 + k] * v2[k * 4 + j];
								}
								vo[i * 4 + j] = v;
							}
						}
					}
				}
			}
		}
	}
}
