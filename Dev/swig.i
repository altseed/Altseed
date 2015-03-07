%module ace_core
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

//-----------------------------------------------------------------------------------
// csharp
//-----------------------------------------------------------------------------------

#if SWIGCSHARP
%include "arrays_csharp.i"


// 独自の定義を使用
namespace ace {
	class achar;
	class astring;
}

%include "swig/Lib/csharp/achar.i"
%include "swig/Lib/csharp/astring.i"
#endif

#if SWIGJAVA
%include "arrays_java.i"

// 独自の定義を使用
namespace ace {
	class achar;
	class astring;
}
%include "swig/Lib/java/achar.i"
%include "swig/Lib/java/astring.i"
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
%typemap(jni) void * "jlong"
%typemap(jtype) void * "long"
%typemap(jstype) void * "long"
%typemap(in) void * { $1 = $input; }
%typemap(out) void * { $result = $1; }
%typemap(javain) void * "$javainput"
%typemap(javaout) void * { return $jnicall; }
#endif

//-----------------------------------------------------------------------------------
// 構造体定義
//-----------------------------------------------------------------------------------

#if SWIGCSHARP

%define STRUCT_OBJECT( CTYPE, RCTYPE, CSTYPE, NAME )
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

%define STRUCT_OBJECT( CTYPE, RCTYPE, CSTYPE, NAME )

%ignore CTYPE;

%typemap(javain)	CTYPE, CTYPE*, CTYPE&, const CTYPE&
%{
StructTranslator.Enqueue_##NAME($javainput)
%}

//------------------------ CTYPE ------------------------
%typemap(jni) CTYPE		"jobject"
%typemap(jtype) CTYPE	"CSTYPE"
%typemap(jstype) CTYPE	"CSTYPE"

%typemap(in) CTYPE { 
	$1 = ##CTYPE ();
	if ($input != nullptr) {
		$1 = ace::StructTranslator::Dequeue ##NAME();
	}
}

%typemap(out) CTYPE { ::ace::StructTranslator::Enqueue ##NAME($1); }

%typemap(javaout)	CTYPE { return StructTranslator.Dequeue_##NAME($jnicall); }

//------------------------ CTYPE* ------------------------
%typemap(jni) CTYPE*		"jobject"
%typemap(jtype) CTYPE*		"CSTYPE"
%typemap(jstype) CTYPE*		"CSTYPE"

%typemap(in) CTYPE* { 
	$1 = ##CTYPE ();
	if ($input != nullptr) {
		$1 = ace::StructTranslator::Dequeue ##NAME();
	}
}

%typemap(out) CTYPE* { ::ace::StructTranslator::Enqueue ##NAME($1); }

%typemap(javaout)	CTYPE* { return StructTranslator.Dequeue_##NAME($jnicall); }

//------------------------ const CTYPE* ------------------------
%typemap(jni)		const CTYPE*		"jobject"
%typemap(jtype)		const CTYPE*		"CSTYPE"
%typemap(jstype)	const CTYPE*		"CSTYPE"

%typemap(in) const CTYPE* { 
	$1 = ##CTYPE ();
	if ($input != nullptr) {
		$1 = ace::StructTranslator::Dequeue ##NAME();
	}
}

%typemap(out) const CTYPE* { ::ace::StructTranslator::Enqueue ##NAME($1); }

%typemap(javaout)	const CTYPE* { return StructTranslator.Dequeue_##NAME($jnicall); }

//------------------------ CTYPE& ------------------------
%typemap(jni)		CTYPE&		"jobject"
%typemap(jtype)		CTYPE&		"CSTYPE"
%typemap(jstype)	CTYPE&		"CSTYPE"

%typemap(in) CTYPE& { 
	$1 = ##CTYPE ();
	if ($input != nullptr) {
		$1 = ace::StructTranslator::Dequeue ##NAME();
	}
}

%typemap(out) CTYPE& { ::ace::StructTranslator::Enqueue ##NAME($1); }

%typemap(javaout)	CTYPE& { return StructTranslator.Dequeue_##NAME($jnicall); }


//------------------------ const CTYPE& ------------------------
%typemap(jni)		const CTYPE&		"jobject"
%typemap(jtype)		const CTYPE&		"CSTYPE"
%typemap(jstype)	const CTYPE&		"CSTYPE"

%typemap(in) const CTYPE& { 
	$1 = nullptr;
	if ($input != nullptr) {
		$1 = ace::StructTranslator::Dequeue_##NAME();
	}
}

%typemap(out) const CTYPE& { ::ace::StructTranslator::Enqueue ##NAME($1); }

%typemap(javaout)	const CTYPE& { return StructTranslator.Dequeue_##NAME($jnicall); }

%enddef

%typemap(javacode) ace::StructTranslator
%{
	public static ace.Vector2DF Dequeue_Vector2DF(ace.Vector2DF v) {
		return new ace.Vector2DF(StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat());
	}

	public static ace.Vector2DF Enqueue_Vector2DF(ace.Vector2DF v) {
		StructTranslator.EnqueueVector2DF(v.X, v.Y);
		return v;
	}

	public static ace.Vector3DF Dequeue_Vector3DF(ace.Vector3DF v) {
		return new ace.Vector3DF(StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat());
	}

	public static ace.Vector3DF Enqueue_Vector3DF(ace.Vector3DF v) {
		StructTranslator.EnqueueVector3DF(v.X, v.Y, v.Z);
		return v;
	}

	public static ace.Vector4DF Dequeue_Vector4DF(ace.Vector4DF v) {
		return new ace.Vector4DF(StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat());
	}

	public static ace.Vector4DF Enqueue_Vector4DF(ace.Vector4DF v) {
		StructTranslator.EnqueueVector4DF(v.X, v.Y, v.Z, v.W);
		return v;
	}

	public static ace.Vector2DI Dequeue_Vector2DI(ace.Vector2DI v) {
		return new ace.Vector2DI(StructTranslator.DequeueInt(), StructTranslator.DequeueInt());
	}

	public static ace.Vector2DI Enqueue_Vector2DI(ace.Vector2DI v) {
		StructTranslator.EnqueueVector2DI(v.X, v.Y);
		return v;
	}

	public static ace.Color Dequeue_Color(ace.Color v) {
		return new ace.Color((byte)StructTranslator.DequeueInt(), (byte)StructTranslator.DequeueInt(), (byte)StructTranslator.DequeueInt(), (byte)StructTranslator.DequeueInt());
	}

	public static ace.Color Enqueue_Color(ace.Color v) {
		StructTranslator.EnqueueColor(v.R, v.G, v.B, v.A);
		return v;
	}

	public static ace.RectF Dequeue_RectF(ace.RectF v) {
		return new ace.RectF(StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat());
	}

	public static ace.RectF Enqueue_RectF(ace.RectF v) {
		StructTranslator.EnqueueRectF(v.X, v.Y, v.Width, v.Height);
		return v;
	}

	public static ace.RectI Dequeue_RectI(ace.RectI v) {
		return new ace.RectI(StructTranslator.DequeueInt(), StructTranslator.DequeueInt(), StructTranslator.DequeueInt(), StructTranslator.DequeueInt());
	}

	public static ace.RectI Enqueue_RectI(ace.RectI v) {
		StructTranslator.EnqueueRectI(v.X, v.Y, v.Width, v.Height);
		return v;
	}

	public static ace.Matrix44 Dequeue_Matrix44(ace.Matrix44 v) {
		return new ace.Matrix44(
		StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat(),
		StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat(),
		StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat(),
		StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat());
	}

	public static ace.Matrix44 Enqueue_Matrix44(ace.Matrix44 v) {
		StructTranslator.EnqueueMatrix44(
		v.Values[0], v.Values[1], v.Values[2], v.Values[3], 
		v.Values[4], v.Values[5], v.Values[6], v.Values[7], 
		v.Values[8], v.Values[9], v.Values[10], v.Values[11],
		v.Values[12], v.Values[13], v.Values[14], v.Values[15]);
		return v;
	}

	public static ace.FCurveKeyframe Dequeue_FCurveKeyframe(ace.FCurveKeyframe v) {
		return new ace.FCurveKeyframe(StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat(), StructTranslator.DequeueInt());
	}

	public static ace.FCurveKeyframe Enqueue_FCurveKeyframe(ace.FCurveKeyframe v) {
		StructTranslator.EnqueueFCurveKeyframe(v.KeyValue.X, v.KeyValue.Y, v.LeftHandle.X, v.LeftHandle.Y, v.RightHandle.X, v.RightHandle.Y, v.InterpolationType.getID());
		return v;
	}
%}

#endif


//-----------------------------------------------------------------------------------
// 構造体定義
//-----------------------------------------------------------------------------------
STRUCT_OBJECT( ace::Vector2DF, ace::Vector2DF_R, ace.Vector2DF, Vector2DF )
STRUCT_OBJECT( ace::Vector2DI, ace::Vector2DI_R, ace.Vector2DI, Vector2DI )
STRUCT_OBJECT( ace::Vector3DF, ace::Vector3DF_R, ace.Vector3DF, Vector3DF )
STRUCT_OBJECT( ace::Vector4DF, ace::Vector4DF_R, ace.Vector4DF, Vector4DF )
STRUCT_OBJECT( ace::RectI, ace::RectI_R, ace.RectI, RectI )
STRUCT_OBJECT( ace::RectF, ace::RectF_R, ace.RectF, RectF )
STRUCT_OBJECT( ace::Matrix33, ace::Matrix33_R, ace.Matrix33, Matrix33 )
STRUCT_OBJECT( ace::Matrix44, ace::Matrix44_R, ace.Matrix44, Matrix44 )
STRUCT_OBJECT( ace::FCurveKeyframe, ace::FCurveKeyframe_R, ace.FCurveKeyframe, FCurveKeyframe )

STRUCT_OBJECT( ace::Color, ace::Color_R, ace.Color, Color )

//-----------------------------------------------------------------------------------
// Dispose無視
//-----------------------------------------------------------------------------------

#if SWIGCSHARP

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

#endif

#if SWIGJAVA

%define DISABLE_DISPOSE( SWIGTYPE )

%typemap(javadestruct, methodname="delete", methodmodifiers="public") SWIGTYPE
{
}

%typemap(javadestruct_derived, methodname="delete", methodmodifiers="public") SWIGTYPE
{
}

%enddef

#endif

//-----------------------------------------------------------------------------------
// IReference,ReferenceObject向け拡張
//-----------------------------------------------------------------------------------

#if SWIGCSHARP

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

#endif

#if SWIGJAVA

%typemap(javaout) int ace::IReference::Release()
%{
{
	int result = $jnicall;
	swigCMemOwn = false;
	swigCPtr = 0;
	return result;
}
%}

%typemap(javaout) int ace::ReferenceObject::Release()
%{
{
	int result = $jnicall;
	swigCMemOwn = false;
	swigCPtr = 0;
	return result;
}
%}

#endif

DISABLE_DISPOSE( ace::IReference )
DISABLE_DISPOSE( ace::ReferenceObject )

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------

#if SWIGCSHARP

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

#endif


#if SWIGJAVA

%define CPP_OBJECT( CTYPE )
/* 未開放バグが怖いが、ラッパークラスとswigのクラスのGCが同時に動作したときに、swigのクラスのGCを止める手段がないため */
DISABLE_DISPOSE( CTYPE )
%typemap(javacode) CTYPE
%{
    public long GetPtr()
    {
    return  swigCPtr;
    }
%}
%enddef

#endif

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------

%include "swig_main.i"

