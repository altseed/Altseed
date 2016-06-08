package asd.Particular;

import java.util.Formatter;
import javax.swing.*;
import java.awt.BorderLayout;
import java.awt.event.*;
import java.io.*;

public class Helper
{
		static public Boolean CheckInitialize()
		{
			asd.GraphicsDeviceType device = GetDefaultDevice();

			if(device == asd.GraphicsDeviceType.DirectX11)
			{
				if (!HasDLL("D3DCOMPILER_43.dll"))
				{
					JOptionPane.showMessageDialog(null, "最新のDirectXEndUserRuntimeをインストールしてください。", "Error", JOptionPane.ERROR_MESSAGE);
					return false;
				}
			}

			if (!Check45())
			{
				JOptionPane.showMessageDialog(null, ".net framework4.5をインストールしてください。", "Error", JOptionPane.ERROR_MESSAGE);
				return false;
			}

			return true;
		}

		static Boolean HasDLL(String path)
		{
			try
			{
				System.loadLibrary(path);
			}
			catch(Exception e)
			{
				return false;
			}
			return true;
		}

		static Boolean Check45()
		{
			// .netでない
			return true;
		}

		static public asd.swig.Core_Imp CreateCore()
		{
			try
			{
				return asd.swig.Core_Imp.CreateCore();
			}
			catch(java.lang.UnsatisfiedLinkError e)
			{
				JOptionPane.showMessageDialog(null, "DLLを読み込めません。", "Error", JOptionPane.ERROR_MESSAGE);
			}

			return null;
		}

		public static asd.GraphicsDeviceType GetDefaultDevice()
		{
			String os = System.getProperty("os.name").toLowerCase();

			Boolean isUnix = os.startsWith("linux") || os.startsWith("mac");
			
			if(isUnix)
			{
				return asd.GraphicsDeviceType.OpenGL;
			}

			return asd.GraphicsDeviceType.DirectX11;
		}

		public static void ThrowException(String message)
		{
			throw new RuntimeException(message);
		}

		public static void ThrowUnregisteredClassException(Object o)
		{
			Formatter fm = new Formatter();
			fm.format("未登録のクラス%sを検出しました。", o);

			throw new RuntimeException(fm.toString());
		}

		public static void ThrowUnreleasedInstanceException(int count)
		{
			Formatter fm = new Formatter();
			fm.format("未開放のインスタンスが%s個存在します。", count);

			throw new RuntimeException(fm.toString());
		}

		public static String Format(String format, Object... args)
		{
			return String.format(format, args);
		}

		public static <T extends java.lang.Comparable> int CompareTo(T v1, T v2)
		{
			return v1.compareTo(v2);
		}
}
