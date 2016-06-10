using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd.Particular
{
	class Helper
	{
		[System.Runtime.InteropServices.DllImport("kernel32")]
		private extern static int LoadLibrary(string path);

		[System.Runtime.InteropServices.DllImport("kernel32")]
		private extern static bool FreeLibrary(int path);

		static public bool CheckInitialize()
		{
			var device = GetDefaultDevice();

			if (device == GraphicsDeviceType.DirectX11)
			{
				if (!HasDLL("D3DCOMPILER_43.dll"))
				{
					System.Windows.Forms.MessageBox.Show("最新のDirectXEndUserRuntimeをインストールしてください。");
					return false;
				}
			}

			if (!Check45())
			{
				System.Windows.Forms.MessageBox.Show(".net framework4.5をインストールしてください。");
				return false;
			}

			return true;
		}

		static bool HasDLL(string path)
		{
			try
			{
				var dll = LoadLibrary(path);
				if (dll != 0)
				{
					FreeLibrary(dll);
					return true;
				}
				return false;
			}
			catch
			{
				// Windows以外の場合を想定
				return true;
			}
		}

		static bool Check45()
		{
			try
			{
				var ThreadID = System.Environment.CurrentManagedThreadId;
			}
			catch (System.Exception e)
			{
				return false;
			}

			return true;
		}

		static public swig.Core_Imp CreateCore()
		{
			try
			{
				return swig.Core_Imp.CreateCore();
			}
			catch (DllNotFoundException e)
			{
				System.Windows.Forms.MessageBox.Show("DLLを読み込めません。");
			}

			return null;
		}

		public static asd.GraphicsDeviceType GetDefaultDevice()
		{
			var os = System.Environment.OSVersion;
			var isUnix = os.Platform == PlatformID.Unix || os.Platform == PlatformID.MacOSX;

			if (isUnix)
			{
				return GraphicsDeviceType.OpenGL;
			}

			return GraphicsDeviceType.DirectX11;
		}

		public static void ThrowException(string message)
		{
			throw new Exception(message);
		}

		public static void ThrowUnregisteredClassException(object o)
		{
			throw new Exception(string.Format("未登録のクラス{0}を検出しました。", o));
		}

		public static void ThrowUnreleasedInstanceException(int count)
		{
			throw new Exception(string.Format("未開放のインスタンスが{0}個存在します。", count));
		}

		public static String Format(String format, params Object[] args)
		{
			return String.Format(format, args);
		}

		public static int CompareTo<T>(T v1, T v2) where T : System.IComparable<T>
		{
			return v1.CompareTo(v2);
		}

		public static int CountIterable<TContent>(System.Collections.Generic.IEnumerable<TContent> contents)
		{
			return contents.Count();
		}

		public static asd.swig.VectorUint8 CreateVectorUint8FromArray(byte[] array)
		{
			var buf = new asd.swig.VectorUint8();
			foreach (var b in array)
			{
				buf.Add(b);
			}

			return buf;
		}

		public static void DisposeVectorUint8(asd.swig.VectorUint8 buf)
		{
			buf.Dispose();
		}

		public static byte[] CreateArrayFromVectorUint8(asd.swig.VectorUint8 buf)
		{
			byte[] dst = new byte[buf.Count];
			buf.CopyTo(dst);
			return dst;
		}

		public static unsafe byte[] CreateArrayFromStaticFile(asd.swig.StaticFile CoreInstance)
		{
			System.IntPtr raw = CoreInstance.GetData();
			byte[] bytes = new byte[CoreInstance.GetSize()];
			System.Runtime.InteropServices.Marshal.Copy(raw, bytes, 0, CoreInstance.GetSize());
			return bytes;
		}

		public static unsafe void CopyStreamFileToList(asd.swig.StreamFile CoreInstance, List<byte> buffer, int size)
		{
			swig.Accessor.StreamFile_Read_(CoreInstance, size);
            System.IntPtr raw = swig.Accessor.StreamFile_GetTempBuffer_(CoreInstance);
            byte[] bytes = new byte[swig.Accessor.StreamFile_GetTempBufferSize_(CoreInstance)];
            System.Runtime.InteropServices.Marshal.Copy(raw, bytes, 0, bytes.Length);

            buffer.Clear();
            buffer.AddRange(bytes);
        }
	}
}
