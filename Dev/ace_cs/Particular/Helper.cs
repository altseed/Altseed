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
	}
}
