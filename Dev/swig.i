%module asd_core
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
#define ASD_DLLEXPORT 
#define ASD_STDCALL 

//-----------------------------------------------------------------------------------
// csharp
//-----------------------------------------------------------------------------------

// stl
%include "std_vector.i"
%template(VectorUint8) std::vector<uint8_t>;

#if SWIGCSHARP
%include "arrays_csharp.i"
%include "typemaps.i"

// 独自の定義を使用
namespace asd {
	class achar;
	class astring;
}

%include "swig/Lib/csharp/achar.i"
%include "swig/Lib/csharp/astring.i"
%include "swig/Lib/csharp/char16.i"
#endif

#if SWIGJAVA
%include "typemaps.i"
%include "arrays_java.i"

// 独自の定義を使用
namespace asd {
	class achar;
	class astring;
}
%include "swig/Lib/java/achar.i"
%include "swig/Lib/java/astring.i"
%include "swig/Lib/java/char16.i"
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
%typemap(in) void * { $1 = (void*)$input; }
%typemap(out) void * { $result = (jlong)$1; }
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

// For Tool
%apply int8_t INOUT[] { int8_t* buf }

%apply int *INOUT { int* v }
%apply float *INOUT { float* v }

%apply int INOUT[] { int* vs }
%apply float INOUT[] { float* vs }

%apply int *INOUT { int* current_item }

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
		$1 = asd::StructTranslator::Dequeue ##NAME();
	}
}

%typemap(out) CTYPE { ::asd::StructTranslator::Enqueue ##NAME($1); }

%typemap(javaout)	CTYPE { return StructTranslator.Dequeue_##NAME($jnicall); }

//------------------------ CTYPE* ------------------------
%typemap(jni) CTYPE*		"jobject"
%typemap(jtype) CTYPE*		"CSTYPE"
%typemap(jstype) CTYPE*		"CSTYPE"

%typemap(in) CTYPE* { 
	$1 = ##CTYPE ();
	if ($input != nullptr) {
		$1 = asd::StructTranslator::Dequeue ##NAME();
	}
}

%typemap(out) CTYPE* { ::asd::StructTranslator::Enqueue ##NAME($1); }

%typemap(javaout)	CTYPE* { return StructTranslator.Dequeue_##NAME($jnicall); }

//------------------------ const CTYPE* ------------------------
%typemap(jni)		const CTYPE*		"jobject"
%typemap(jtype)		const CTYPE*		"CSTYPE"
%typemap(jstype)	const CTYPE*		"CSTYPE"

%typemap(in) const CTYPE* { 
	$1 = ##CTYPE ();
	if ($input != nullptr) {
		$1 = asd::StructTranslator::Dequeue ##NAME();
	}
}

%typemap(out) const CTYPE* { ::asd::StructTranslator::Enqueue ##NAME($1); }

%typemap(javaout)	const CTYPE* { return StructTranslator.Dequeue_##NAME($jnicall); }

//------------------------ CTYPE& ------------------------
%typemap(jni)		CTYPE&		"jobject"
%typemap(jtype)		CTYPE&		"CSTYPE"
%typemap(jstype)	CTYPE&		"CSTYPE"

%typemap(in) CTYPE& { 
	$1 = ##CTYPE ();
	if ($input != nullptr) {
		$1 = asd::StructTranslator::Dequeue ##NAME();
	}
}

%typemap(out) CTYPE& { ::asd::StructTranslator::Enqueue ##NAME($1); }

%typemap(javaout)	CTYPE& { return StructTranslator.Dequeue_##NAME($jnicall); }


//------------------------ const CTYPE& ------------------------
%typemap(jni)		const CTYPE&		"jobject"
%typemap(jtype)		const CTYPE&		"CSTYPE"
%typemap(jstype)	const CTYPE&		"CSTYPE"

%typemap(in) const CTYPE& { 
	$1 = nullptr;
	if ($input != nullptr) {
		$1 = asd::StructTranslator::Dequeue_##NAME();
	}
}

%typemap(out) const CTYPE& { ::asd::StructTranslator::Enqueue ##NAME($1); }

%typemap(javaout)	const CTYPE& { return StructTranslator.Dequeue_##NAME($jnicall); }

%enddef

%typemap(javacode) asd::StructTranslator
%{
	public static asd.Vector2DF Dequeue_Vector2DF(asd.Vector2DF v) {
		return new asd.Vector2DF(StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat());
	}

	public static asd.Vector2DF Enqueue_Vector2DF(asd.Vector2DF v) {
		StructTranslator.EnqueueVector2DF(v.X, v.Y);
		return v;
	}

	public static asd.Vector3DF Dequeue_Vector3DF(asd.Vector3DF v) {
		return new asd.Vector3DF(StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat());
	}

	public static asd.Vector3DF Enqueue_Vector3DF(asd.Vector3DF v) {
		StructTranslator.EnqueueVector3DF(v.X, v.Y, v.Z);
		return v;
	}

	public static asd.Vector4DF Dequeue_Vector4DF(asd.Vector4DF v) {
		return new asd.Vector4DF(StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat());
	}

	public static asd.Vector4DF Enqueue_Vector4DF(asd.Vector4DF v) {
		StructTranslator.EnqueueVector4DF(v.X, v.Y, v.Z, v.W);
		return v;
	}

	public static asd.Vector2DI Dequeue_Vector2DI(asd.Vector2DI v) {
		return new asd.Vector2DI(StructTranslator.DequeueInt(), StructTranslator.DequeueInt());
	}

	public static asd.Vector2DI Enqueue_Vector2DI(asd.Vector2DI v) {
		StructTranslator.EnqueueVector2DI(v.X, v.Y);
		return v;
	}

	public static asd.Color Dequeue_Color(asd.Color v) {
		return new asd.Color((short)StructTranslator.DequeueInt(), (short)StructTranslator.DequeueInt(), (short)StructTranslator.DequeueInt(), (short)StructTranslator.DequeueInt());
	}

	public static asd.Color Enqueue_Color(asd.Color v) {
		StructTranslator.EnqueueColor(v.R, v.G, v.B, v.A);
		return v;
	}

	public static asd.RectF Dequeue_RectF(asd.RectF v) {
		return new asd.RectF(StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat());
	}

	public static asd.RectF Enqueue_RectF(asd.RectF v) {
		StructTranslator.EnqueueRectF(v.X, v.Y, v.Width, v.Height);
		return v;
	}

	public static asd.RectI Dequeue_RectI(asd.RectI v) {
		return new asd.RectI(StructTranslator.DequeueInt(), StructTranslator.DequeueInt(), StructTranslator.DequeueInt(), StructTranslator.DequeueInt());
	}

	public static asd.RectI Enqueue_RectI(asd.RectI v) {
		StructTranslator.EnqueueRectI(v.X, v.Y, v.Width, v.Height);
		return v;
	}

	public static asd.Matrix33 Dequeue_Matrix33(asd.Matrix33 v) {
		return new asd.Matrix33(
		StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat(),
		StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat(),
		StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat());
	}

	public static asd.Matrix33 Enqueue_Matrix33(asd.Matrix33 v) {
		StructTranslator.EnqueueMatrix33(
		v.Values[0], v.Values[1], v.Values[2],
		v.Values[3], v.Values[4], v.Values[5],
		v.Values[6], v.Values[7], v.Values[8]);
		return v;
	}

	public static asd.Matrix44 Dequeue_Matrix44(asd.Matrix44 v) {
		return new asd.Matrix44(
		StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat(),
		StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat(),
		StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat(),
		StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat());
	}

	public static asd.Matrix44 Enqueue_Matrix44(asd.Matrix44 v) {
		StructTranslator.EnqueueMatrix44(
		v.Values[0], v.Values[1], v.Values[2], v.Values[3], 
		v.Values[4], v.Values[5], v.Values[6], v.Values[7], 
		v.Values[8], v.Values[9], v.Values[10], v.Values[11],
		v.Values[12], v.Values[13], v.Values[14], v.Values[15]);
		return v;
	}

	public static asd.FCurveKeyframe Dequeue_FCurveKeyframe(asd.FCurveKeyframe v) {
		return new asd.FCurveKeyframe(StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat(), StructTranslator.DequeueFloat(), StructTranslator.DequeueInt());
	}

	public static asd.FCurveKeyframe Enqueue_FCurveKeyframe(asd.FCurveKeyframe v) {
		StructTranslator.EnqueueFCurveKeyframe(v.KeyValue.X, v.KeyValue.Y, v.LeftHandle.X, v.LeftHandle.Y, v.RightHandle.X, v.RightHandle.Y, v.Interpolation.swigValue());
		return v;
	}
%}

// For Tool
%apply int8_t[] { int8_t* buf }

%apply int *INOUT { int* v }
%apply float *INOUT { float* v }

%apply int[] {int * vs};
%apply float[] {float * vs};

%apply int *INOUT { int* current_item }

#endif


//-----------------------------------------------------------------------------------
// 構造体定義
//-----------------------------------------------------------------------------------
STRUCT_OBJECT( asd::Vector2DF, asd::Vector2DF_R, asd.Vector2DF, Vector2DF )
STRUCT_OBJECT( asd::Vector2DI, asd::Vector2DI_R, asd.Vector2DI, Vector2DI )
STRUCT_OBJECT( asd::Vector3DF, asd::Vector3DF_R, asd.Vector3DF, Vector3DF )
STRUCT_OBJECT( asd::Vector4DF, asd::Vector4DF_R, asd.Vector4DF, Vector4DF )
STRUCT_OBJECT( asd::RectI, asd::RectI_R, asd.RectI, RectI )
STRUCT_OBJECT( asd::RectF, asd::RectF_R, asd.RectF, RectF )
STRUCT_OBJECT( asd::Matrix33, asd::Matrix33_R, asd.Matrix33, Matrix33 )
STRUCT_OBJECT( asd::Matrix44, asd::Matrix44_R, asd.Matrix44, Matrix44 )
STRUCT_OBJECT( asd::FCurveKeyframe, asd::FCurveKeyframe_R, asd.FCurveKeyframe, FCurveKeyframe )

STRUCT_OBJECT( asd::Color, asd::Color_R, asd.Color, Color )

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

%typemap(csout) int asd::IReference::Release()
%{
{
	int result = $imcall;
	swigCMemOwn = false;
	swigCPtr = new System.Runtime.InteropServices.HandleRef(null, System.IntPtr.Zero);
	return result;
}
%}

%typemap(csout) int asd::ReferenceObject::Release()
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

%typemap(javaout) int asd::IReference::Release()
%{
{
	int result = $jnicall;
	swigCMemOwn = false;
	swigCPtr = 0;
	return result;
}
%}

%typemap(javaout) int asd::ReferenceObject::Release()
%{
{
	int result = $jnicall;
	swigCMemOwn = false;
	swigCPtr = 0;
	return result;
}
%}

#endif

DISABLE_DISPOSE( asd::IReference )
DISABLE_DISPOSE( asd::ReferenceObject )

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

