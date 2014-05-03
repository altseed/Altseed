/* -----------------------------------------------------------------------------
 * std_wstring.i
 *
 * Typemaps for std::wstring and const std::wstring&
 * These are mapped to a C# String and are passed around by value.
 *
 * To use non-const std::wstring references use the following %apply.  Note 
 * that they are passed by value.
 * %apply const std::wstring & {std::wstring &};
 * ----------------------------------------------------------------------------- */

/*
	You may copy, modify, distribute, and make derivative works based on
	this software, in source code or object code form, without
	restriction. If you distribute the software to others, you may do
	so according to the terms of your choice. This software is offered as
	is, without warranty of any kind.
*/

namespace ace {

%naturalvar astring;

class astring;

// astring
%typemap(ctype, out="void *") astring "::ace::achar *"
%typemap(imtype, inattributes="[global::System.Runtime.InteropServices.MarshalAs(UnmanagedType.LPWStr)]") astring "string"
%typemap(cstype) astring "string"
%typemap(csdirectorin) astring "$iminput"
%typemap(csdirectorout) astring "$cscall"

%typemap(in, canthrow=1) astring 
%{ if (!$input) {
    SWIG_CSharpSetPendingExceptionArgument(SWIG_CSharpArgumentNullException, "null astring", 0);
    return $null;
   }
   $1.assign($input); %}
%typemap(out) astring %{ $result = SWIG_csharp_wstring_callback($1.c_str()); %}

%typemap(directorout, canthrow=1) astring 
%{ if (!$input) {
    SWIG_CSharpSetPendingExceptionArgument(SWIG_CSharpArgumentNullException, "null astring", 0);
    return $null;
   }
   $result.assign($input); %}

%typemap(directorin) astring %{ $input = SWIG_csharp_wstring_callback($1.c_str()); %}

%typemap(csin) astring "$csinput"
%typemap(csout, excode=SWIGEXCODE) astring {
    string ret = $imcall;$excode
    return ret;
  }

//%typemap(typecheck) astring = achar *;

%typemap(throws, canthrow=1) astring
%{ std::string message($1.begin(), $1.end());
   SWIG_CSharpSetPendingException(SWIG_CSharpApplicationException, message.c_str());
   return $null; %}

// const astring &
%typemap(ctype, out="void *") const astring & "wchar_t *"
%typemap(imtype, inattributes="[global::System.Runtime.InteropServices.MarshalAs(global::System.Runtime.InteropServices.UnmanagedType.LPWStr)]") const astring & "string"  
%typemap(cstype) const astring & "string"

%typemap(csdirectorin) const astring & "$iminput"
%typemap(csdirectorout) const astring & "$cscall"

%typemap(in, canthrow=1) const astring &
%{ if (!$input) {
    SWIG_CSharpSetPendingExceptionArgument(SWIG_CSharpArgumentNullException, "null astring", 0);
    return $null;
   }
   ::ace::astring $1_str($input);
   $1 = &$1_str; %}
%typemap(out) const astring & %{ $result = SWIG_csharp_wstring_callback($1->c_str()); %}

%typemap(csin) const astring & "$csinput"
%typemap(csout, excode=SWIGEXCODE) const astring & {
    string ret = $imcall;$excode
    return ret;
  }

%typemap(directorout, canthrow=1, warning=SWIGWARN_TYPEMAP_THREAD_UNSAFE_MSG) const astring &
%{ if (!$input) {
    SWIG_CSharpSetPendingExceptionArgument(SWIG_CSharpArgumentNullException, "null astring", 0);
    return $null;
   }
   /* possible thread/reentrant code problem */
   static ace::astring $1_str;
   $1_str = $input;
   $result = &$1_str; %}

%typemap(directorin) const astring & %{ $input = SWIG_csharp_wstring_callback($1.c_str()); %}

%typemap(csvarin, excode=SWIGEXCODE2) const astring & %{
    set {
      $imcall;$excode
    } %}
%typemap(csvarout, excode=SWIGEXCODE2) const astring & %{
    get {
      string ret = $imcall;$excode
      return ret;
    } %}

//%typemap(typecheck) const astring & = wchar_t *;

%typemap(throws, canthrow=1) const astring &
%{ std::string message($1.begin(), $1.end());
   SWIG_CSharpSetPendingException(SWIG_CSharpApplicationException, message.c_str());
   return $null; %}

}

