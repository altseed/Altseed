/* -----------------------------------------------------------------------------
 * achar.i
 *
 * Typemaps for the achar_t type
 * These are mapped to a C# String and are passed around by value.
 *
 * Support code for wide strings can be turned off by defining SWIG_CSHARP_NO_WSTRING_HELPER
 *
 * ----------------------------------------------------------------------------- */

 /*
	You may copy, modify, distribute, and make derivative works based on
	this software, in source code or object code form, without
	restriction. If you distribute the software to others, you may do
	so according to the terms of your choice. This software is offered as
	is, without warranty of any kind.
 */

#if !defined(SWIG_CSHARP_NO_WSTRING_HELPER)
#if !defined(SWIG_CSHARP_WSTRING_HELPER_)
#define SWIG_CSHARP_WSTRING_HELPER_
%insert(runtime) %{
/* Callback for returning strings to C# without leaking memory */
typedef void * (SWIGSTDCALL* SWIG_CSharpWStringHelperCallback)(const ::ace::achar *);
static SWIG_CSharpWStringHelperCallback SWIG_csharp_wstring_callback = NULL;
%}

%pragma(csharp) imclasscode=%{
  protected class SWIGWStringHelper {

    public delegate string SWIGWStringDelegate(System.IntPtr message);
    static SWIGWStringDelegate wstringDelegate = new SWIGWStringDelegate(CreateWString);

    [global::System.Runtime.InteropServices.DllImport("$dllimport", EntryPoint="SWIGRegisterWStringCallback_$module")]
    public static extern void SWIGRegisterWStringCallback_$module(SWIGWStringDelegate wstringDelegate);

    static string CreateWString([global::System.Runtime.InteropServices.MarshalAs(global::System.Runtime.InteropServices.UnmanagedType.LPWStr)]global::System.IntPtr cString) {
      return System.Runtime.InteropServices.Marshal.PtrToStringUni(cString);
    }

    static SWIGWStringHelper() {
      SWIGRegisterWStringCallback_$module(wstringDelegate);
    }
  }

  static protected SWIGWStringHelper swigWStringHelper = new SWIGWStringHelper();
%}

%insert(runtime) %{
#ifdef __cplusplus
extern "C"
#endif
SWIGEXPORT void SWIGSTDCALL SWIGRegisterWStringCallback_$module(SWIG_CSharpWStringHelperCallback callback) {
  SWIG_csharp_wstring_callback = callback;
}
%}
#endif // SWIG_CSHARP_WSTRING_HELPER_
#endif // SWIG_CSHARP_NO_WSTRING_HELPER


// ::ace::achar
%typemap(ctype) ::ace::achar "::ace::achar"
%typemap(imtype) ::ace::achar "char"
%typemap(cstype) ::ace::achar "char"

%typemap(csin) ::ace::achar "$csinput"
%typemap(csout, excode=SWIGEXCODE) ::ace::achar {
    char ret = $imcall;$excode
    return ret;
  }
%typemap(csvarin, excode=SWIGEXCODE2) ::ace::achar %{
    set {
      $imcall;$excode
    } %}
%typemap(csvarout, excode=SWIGEXCODE2) ::ace::achar %{
    get {
      char ret = $imcall;$excode
      return ret;
    } %}

%typemap(in) ::ace::achar %{ $1 = ($1_ltype)$input; %}
%typemap(out) ::ace::achar %{ $result = (::ace::achar)$1; %}

//%typemap(typecheck) ::ace::achar = char;

// ::ace::achar *
%typemap(ctype) ::ace::achar * "::ace::achar *"
%typemap(imtype, inattributes="[global::System.Runtime.InteropServices.MarshalAs(global::System.Runtime.InteropServices.UnmanagedType.LPWStr)]", out="System.IntPtr" ) ::ace::achar * "string"
%typemap(cstype) ::ace::achar * "string"

%typemap(csin) ::ace::achar * "$csinput"
%typemap(csout, excode=SWIGEXCODE) ::ace::achar * {
    string ret = System.Runtime.InteropServices.Marshal.PtrToStringUni($imcall);$excode
    return ret;
  }
%typemap(csvarin, excode=SWIGEXCODE2) ::ace::achar * %{
    set {
      $imcall;$excode
    } %}
%typemap(csvarout, excode=SWIGEXCODE2) ::ace::achar * %{
    get {
      string ret = $imcall;$excode
      return ret;
    } %}

%typemap(in) ::ace::achar * %{ $1 = ($1_ltype)$input; %}
%typemap(out) ::ace::achar * %{ $result = (::ace::achar *)$1; %}

//%typemap(typecheck) ::ace::achar * = char *;
