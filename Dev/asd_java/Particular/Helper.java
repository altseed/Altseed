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
			/*
			try
			{
				System.loadLibrary(path);
			}
			catch(Exception e)
			{
				return false;
			}
			*/
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

		public static <TContent> int CountIterable(java.lang.Iterable<TContent> contents)
		{
			int count = 0;
			for (Object content : contents) count++;
			return count;
		}

		public static asd.swig.VectorUint8 CreateVectorUint8FromArray(byte[] array)
		{
			asd.swig.VectorUint8 buf = new asd.swig.VectorUint8();
			for (byte b : array)
			{
				buf.add(b);
			}

			return buf;
		}

		public static void DisposeVectorUint8(asd.swig.VectorUint8 buf)
		{
			buf.delete();
		}

		public static byte[] CreateArrayFromVectorUint8(asd.swig.VectorUint8 buf)
		{
			byte[] dst = new byte[(int)buf.size()];
			
			for(int i = 0; i < dst.length; i++)
			{
				dst[i] = (byte)(buf.get(i) - 128);
			}

			return dst;
		}

		public static byte[] CreateArrayFromStaticFile(asd.swig.StaticFile CoreInstance)
		{
			return null;
			/*
			System.IntPtr raw = CoreInstance.GetData();
			byte[] bytes = new byte[CoreInstance.GetSize()];
			System.Runtime.InteropServices.Marshal.Copy(raw, bytes, 0, CoreInstance.GetSize());
			return bytes;
			*/
		}

		public static void CopyStreamFileToList(asd.swig.StreamFile CoreInstance, java.util.ArrayList<java.lang.Byte> buffer, int size)
		{
			/*
			swig.Accessor.StreamFile_Read_(CoreInstance, size);
            System.IntPtr raw = swig.Accessor.StreamFile_GetTempBuffer_(CoreInstance);
            byte[] bytes = new byte[swig.Accessor.StreamFile_GetTempBufferSize_(CoreInstance)];
            System.Runtime.InteropServices.Marshal.Copy(raw, bytes, 0, bytes.Length);

            buffer.Clear();
            buffer.AddRange(bytes);
			*/
        }

		public static java.lang.String ConvertUTF8(byte[] buffer)
		{
			return new String(buffer, java.nio.charset.StandardCharsets.UTF_8);
		}

		public static java.lang.String ConvertUTF8(java.util.ArrayList<Byte> buffer)
		{
			byte[] buf = new byte[(int)buffer.size()];
			for(int i = 0; i < buf.length; i++)
			{
				buf[i] = buffer.get(i);
			}

			return new String(buf, java.nio.charset.StandardCharsets.UTF_8);
		}

		public static short ConvertColor(int c)
		{
			return (short)c;
		}
}
