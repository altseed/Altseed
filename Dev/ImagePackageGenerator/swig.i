%module PSDParser

%{
#include "PSDParser.h"
%}

//-----------------------------------------------------------------------------------
// csharp
//-----------------------------------------------------------------------------------
%include "stdint.i"
%include "std_shared_ptr.i"
%include "arrays_csharp.i"

namespace PSDParser {
	class uchar;
}

%include "swig/Lib/csharp/uchar.i"

// セキュリティチェックを外して高速化
%pragma(csharp) imclassclassmodifiers="
[System.Security.SuppressUnmanagedCodeSecurity]
public class"

// 全てのclass を unsafe partial class に。
%typemap(csclassmodifiers) SWIGTYPE "unsafe partial class"

// csharp void の定義
%typemap(ctype) void * "void *"
%typemap(imtype) void * "System.IntPtr"
%typemap(cstype) void * "System.IntPtr"
%typemap(in) void * { $1 = $input; }
%typemap(out) void * { $result = $1; }
%typemap(csin) void * "$csinput"
%typemap(csout) void * { return $imcall; }
%typemap(csvarin) void * { set { $imcall; } }
%typemap(csvarout) void * { get { return $imcall; } } 

%shared_ptr(PSDParser::Layer)
%shared_ptr(PSDParser::Document)

%include "PSDParser/PSDParser.h"

