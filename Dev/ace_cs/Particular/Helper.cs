using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace.Particular
{
	class Helper
	{
		[System.Runtime.InteropServices.DllImport("kernel32")]
		private extern static int LoadLibrary(string path);

		[System.Runtime.InteropServices.DllImport("kernel32")]
		private extern static bool FreeLibrary(int path);

		static public bool CheckInitialize()
		{
			if (!HasDLL("D3DCOMPILER_43.dll"))
			{
				System.Windows.Forms.MessageBox.Show("最新のDirectXEndUserRuntimeをインストールしてください。");
				return false;
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
			catch(DllNotFoundException e)
			{
				System.Windows.Forms.MessageBox.Show("DLLを読み込めません。");
			}

			return null;
		}

		public static ace.GraphicsDeviceType GetDefaultDevice()
		{
			var os = System.Environment.OSVersion;
			var isUnix = os.Platform == PlatformID.Unix || os.Platform == PlatformID.MacOSX;
			
			if(isUnix)
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
	}
}
