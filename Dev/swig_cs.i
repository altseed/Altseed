%module ace_core_cs
%{

#include "dll.h"

%}

//-----------------------------------------------------------------------------------
// common
//-----------------------------------------------------------------------------------
%include "stdint.i"
%include "std_vector.i"
%include "std_string.i"

// swig上ではなし
#define ACE_DLLEXPORT 
#define ACE_STDCALL 

// 通常の定義
//namespace ace {
//	typedef wchar_t achar;
//	typedef std::wstring astring;
//}
//%include "wchar.i"
//%include "std_wstring.i"

// 独自の定義を使用
namespace ace {
	class achar;
	class astring;
}

//-----------------------------------------------------------------------------------
// csharp
//-----------------------------------------------------------------------------------

#if SWIGCSHARP
%include "arrays_csharp.i"
#endif

#if SWIGJAVA
%include "arrays_java.i"
#endif


#if SWIGCSHARP
%include "swig/Lib/csharp/achar.i"
%include "swig/Lib/csharp/astring.i"
#endif


// セキュリティチェックを外して高速化
%pragma(csharp) imclassclassmodifiers="
[System.Security.SuppressUnmanagedCodeSecurity]
unsafe class"

// 全てのclass を unsafe partial class に変更
%typemap(csclassmodifiers) SWIGTYPE "unsafe partial class"

%pragma(csharp) moduleclassmodifiers="unsafe class"

//-----------------------------------------------------------------------------------
// void*定義
//-----------------------------------------------------------------------------------
#if SWIGCSHARP
%typemap(ctype) void * "void *"
%typemap(imtype) void * "System.IntPtr"
%typemap(cstype) void * "System.IntPtr"
%typemap(in) void * { $1 = $input; }
%typemap(out) void * { $result = $1; }
%typemap(csin) void * "$csinput"
%typemap(csout) void * { return $imcall; }
%typemap(csvarin) void * "set { $imcall; }"
%typemap(csvarout) void * "get { return $imcall; }" 
#endif


#if SWIGJAVA
%typemap(jni) void * "void *"
%typemap(jtype) void * "com.sun.jna.ptr.IntByReference"
%typemap(jstype) void * "com.sun.jna.ptr.IntByReference"
%typemap(in) void * { $1 = $input; }
%typemap(out) void * { $result = $1; }
%typemap(javasin) void * "$csinput"
%typemap(javasout) void * { return $imcall; }
#endif

//-----------------------------------------------------------------------------------
// 構造体定義
//-----------------------------------------------------------------------------------

#if SWIGCSHARP

%define STRUCT_OBJECT( CTYPE, RCTYPE, CSTYPE )
%ignore CTYPE;

// 一切何も付いていないときの処理がおかしい可能性あり
%typemap(ctype)		CTYPE	"RCTYPE"
%typemap(imtype)	CTYPE	"CSTYPE"
%typemap(cstype)	CTYPE	"CSTYPE"
%typemap(in)		CTYPE	{ assert(sizeof(CTYPE)==sizeof(RCTYPE)); memcpy(&$1, &$input, sizeof(RCTYPE)); }
%typemap(out)		CTYPE	{ assert(sizeof(CTYPE)==sizeof(RCTYPE)); memcpy(&$result, &$1, sizeof(RCTYPE)); }
%typemap(csin)		CTYPE	"$csinput"
%typemap(csout)		CTYPE	{ return $imcall; }
%typemap(csvarin)	CTYPE	{ set { $imcall; } }
%typemap(csvarout)	CTYPE	{ get { return $imcall; } } 

%typemap(ctype)		CTYPE* "void*"
%typemap(imtype)	CTYPE* "ref CSTYPE"
%typemap(cstype)	CTYPE* "ref CSTYPE"
%typemap(in)		CTYPE* "$1 = (CTYPE*)$input;"
%typemap(csin)		CTYPE* "ref $csinput"


%typemap(ctype)		const CTYPE* "void*"
%typemap(imtype)	const CTYPE* "ref CSTYPE"
%typemap(cstype)	const CTYPE* "ref CSTYPE"
%typemap(in)		const CTYPE* "$1 = (CTYPE*)$input;"
%typemap(csin)		const CTYPE* "ref $csinput"

%typemap(ctype)		CTYPE& "void*"
%typemap(imtype)	CTYPE& "ref CSTYPE"
%typemap(cstype)	CTYPE& "ref CSTYPE"
%typemap(in)		CTYPE& "$1 = (CTYPE*)$input;"
%typemap(csin)		CTYPE& "ref $csinput"

%typemap(ctype)		const CTYPE& "void*"
%typemap(imtype)	const CTYPE& "ref CSTYPE"
%typemap(cstype)	const CTYPE& "ref CSTYPE"
%typemap(in)		const CTYPE& "$1 = (CTYPE*)$input;"
%typemap(csin)		const CTYPE& "ref $csinput"
%enddef

#endif


#if SWIGJAVA

%define STRUCT_OBJECT( CTYPE, RCTYPE, CSTYPE )
%ignore CTYPE;

// 一切何も付いていないときの処理がおかしい可能性あり
%typemap(jni)		CTYPE	"CTYPE"
%typemap(imtype)	CTYPE	"CSTYPE"
%typemap(cstype)	CTYPE	"CSTYPE"
%typemap(in)		CTYPE	{ assert(sizeof(CTYPE)==sizeof(RCTYPE)); memcpy(&$1, &$input, sizeof(RCTYPE)); }
%typemap(out)		CTYPE	{ assert(sizeof(CTYPE)==sizeof(RCTYPE)); memcpy(&$result, &$1, sizeof(RCTYPE)); }
%typemap(csin)		CTYPE	"$csinput"
%typemap(csout)		CTYPE	{ return $imcall; }
%typemap(csvarin)	CTYPE	{ set { $imcall; } }
%typemap(csvarout)	CTYPE	{ get { return $imcall; } } 

%typemap(ctype)		CTYPE* "void*"
%typemap(imtype)	CTYPE* "ref CSTYPE"
%typemap(cstype)	CTYPE* "ref CSTYPE"
%typemap(in)		CTYPE* "$1 = (CTYPE*)$input;"
%typemap(csin)		CTYPE* "ref $csinput"


%typemap(ctype)		const CTYPE* "void*"
%typemap(imtype)	const CTYPE* "ref CSTYPE"
%typemap(cstype)	const CTYPE* "ref CSTYPE"
%typemap(in)		const CTYPE* "$1 = (CTYPE*)$input;"
%typemap(csin)		const CTYPE* "ref $csinput"

%typemap(ctype)		CTYPE& "void*"
%typemap(imtype)	CTYPE& "ref CSTYPE"
%typemap(cstype)	CTYPE& "ref CSTYPE"
%typemap(in)		CTYPE& "$1 = (CTYPE*)$input;"
%typemap(csin)		CTYPE& "ref $csinput"

%typemap(ctype)		const CTYPE& "void*"
%typemap(imtype)	const CTYPE& "ref CSTYPE"
%typemap(cstype)	const CTYPE& "ref CSTYPE"
%typemap(in)		const CTYPE& "$1 = (CTYPE*)$input;"
%typemap(csin)		const CTYPE& "ref $csinput"
%enddef

#endif


//-----------------------------------------------------------------------------------
// 構造体定義
//-----------------------------------------------------------------------------------
STRUCT_OBJECT( ace::Vector2DF, ace::Vector2DF_R, ace.Vector2DF )
STRUCT_OBJECT( ace::Vector2DI, ace::Vector2DI_R, ace.Vector2DI )
STRUCT_OBJECT( ace::Vector3DF, ace::Vector3DF_R, ace.Vector3DF )
STRUCT_OBJECT( ace::Vector4DF, ace::Vector4DF_R, ace.Vector4DF )
STRUCT_OBJECT( ace::RectI, ace::RectI_R, ace.RectI )
STRUCT_OBJECT( ace::RectF, ace::RectF_R, ace.RectF )
STRUCT_OBJECT( ace::Matrix33, ace::Matrix33_R, ace.Matrix33 )
STRUCT_OBJECT( ace::Matrix44, ace::Matrix44_R, ace.Matrix44 )
STRUCT_OBJECT( ace::FCurveKeyframe, ace::FCurveKeyframe_R, ace.FCurveKeyframe )
STRUCT_OBJECT( ace::TextureLockInfomation, ace::TextureLockInfomation_R, ace.TextureLockInfomation )

STRUCT_OBJECT( ace::Color, ace::Color_R, ace.Color )

//-----------------------------------------------------------------------------------
// Dispose無視
//-----------------------------------------------------------------------------------
%define DISABLE_DISPOSE( SWIGTYPE )
%typemap(csdestruct, methodname="Dispose", methodmodifiers="public") SWIGTYPE
{
	lock(this)
	{
		System.GC.SuppressFinalize(this);
	}
}

%typemap(csdestruct_derived, methodname="Dispose", methodmodifiers="public") SWIGTYPE
{
	lock(this)
	{
		System.GC.SuppressFinalize(this);
	}
}

%enddef

//-----------------------------------------------------------------------------------
// IReference,ReferenceObject向け拡張
//-----------------------------------------------------------------------------------
%typemap(csout) int ace::IReference::Release()
%{
{
	int result = $imcall;
	swigCMemOwn = false;
	swigCPtr = new System.Runtime.InteropServices.HandleRef(null, System.IntPtr.Zero);
	return result;
}
%}

%typemap(csout) int ace::ReferenceObject::Release()
%{
{
	int result = $imcall;
	swigCMemOwn = false;
	swigCPtr = new System.Runtime.InteropServices.HandleRef(null, System.IntPtr.Zero);
	return result;
}
%}

DISABLE_DISPOSE( ace::IReference )
DISABLE_DISPOSE( ace::ReferenceObject )

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
%define CPP_OBJECT( CTYPE )
/* 未開放バグが怖いが、ラッパークラスとswigのクラスのGCが同時に動作したときに、swigのクラスのGCを止める手段がないため */
DISABLE_DISPOSE( CTYPE )
%typemap(cscode) CTYPE
%{
    public System.IntPtr GetPtr()
    {
    return  swigCPtr.Handle;
    }
%}
%enddef


//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------

%include "swig_main.i"

