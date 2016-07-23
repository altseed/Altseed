// Original Code
// https://code.google.com/p/jirrlicht/source/browse/trunk/jni/originalSwigProject/wchar_t.i

namespace asd
{
#ifdef _WIN32

#ifdef _CHAR16T
	typedef wchar_t achar;
	typedef std::wstring astring;
#else
	typedef char16_t achar;
	typedef std::basic_string<char16_t> astring;
#endif
#else 
typedef char16_t achar;
typedef std::basic_string<char16_t> astring;
#endif
};

%{

int astrlen(const ::asd::achar* s)
{
	int cnt = 0;
	while(*s++) cnt++;
	return cnt;
}
%}

%typemap(jni)		::asd::achar* "jstring"
%typemap(jtype)		::asd::achar* "String"
%typemap(jstype)	::asd::achar* "String"


%typemap(in) ::asd::achar* { 
  $1 = 0;
  if ($input) {
    $1 = ($1_ltype)JCALL2(GetStringChars, jenv, $input, 0);
    if (!$1) return $null;
  }
}

%typemap(directorin, descriptor="Ljava/lang/String;") ::asd::achar* { 
  $input = 0;
  if ($1) {
    $input = JCALL1(NewString, jenv, $1);
    if (!$input) return $null;
  }
}
%typemap(freearg) ::asd::achar* { if ($1) JCALL2(ReleaseStringChars, jenv, $input, (const jchar *) $1); }
%typemap(out) ::asd::achar* { if($1) $result = JCALL2(NewString, jenv, (const jchar *) $1, astrlen ($1)); }
%typemap(javadirectorin) ::asd::achar* "$jniinput"
%typemap(javadirectorout) ::asd::achar* "$javacall"

%typemap(throws) ::asd::achar* {
  SWIG_JavaThrowException(jenv, SWIG_JavaRuntimeException, $1);
  return $null;
}
%typemap(javain) ::asd::achar* "$javainput"
%typemap(javaout) ::asd::achar* {
    return $jnicall;
  }
