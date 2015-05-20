// Original Code
// https://code.google.com/p/jirrlicht/source/browse/trunk/jni/originalSwigProject/wchar_t.i

namespace ace
{
#ifdef _WIN32
typedef wchar_t achar;
typedef std::wstring astring;
#else 
typedef char16_t achar;
typedef std::basic_string<char16_t> astring;
#endif
};

%{

int astrlen(const ::ace::achar* s)
{
	int cnt = 0;
	while(*s++) cnt++;
	return cnt;
}
%}

%typemap(jni)		::ace::achar* "jstring"
%typemap(jtype)		::ace::achar* "String"
%typemap(jstype)	::ace::achar* "String"


%typemap(in) ::ace::achar* { 
  $1 = 0;
  if ($input) {
    $1 = ($1_ltype)JCALL2(GetStringChars, jenv, $input, 0);
    if (!$1) return $null;
  }
}

%typemap(directorin, descriptor="Ljava/lang/String;") ::ace::achar* { 
  $input = 0;
  if ($1) {
    $input = JCALL1(NewString, jenv, $1);
    if (!$input) return $null;
  }
}
%typemap(freearg) ::ace::achar* { if ($1) JCALL2(ReleaseStringChars, jenv, $input, (const jchar *) $1); }
%typemap(out) ::ace::achar* { if($1) $result = JCALL2(NewString, jenv, (const jchar *) $1, astrlen ($1)); }
%typemap(javadirectorin) ::ace::achar* "$jniinput"
%typemap(javadirectorout) ::ace::achar* "$javacall"

%typemap(throws) ::ace::achar* {
  SWIG_JavaThrowException(jenv, SWIG_JavaRuntimeException, $1);
  return $null;
}
%typemap(javain) ::ace::achar* "$javainput"
%typemap(javaout) ::ace::achar* {
    return $jnicall;
  }
