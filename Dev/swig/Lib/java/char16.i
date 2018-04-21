// Original Code
// https://code.google.com/p/jirrlicht/source/browse/trunk/jni/originalSwigProject/wchar_t.i

%{

int char16_len(const char16_t* s)
{
	int cnt = 0;
	while(*s++) cnt++;
	return cnt;
}
%}

%typemap(jni)		char16_t* "jstring"
%typemap(jtype)		char16_t* "String"
%typemap(jstype)	char16_t* "String"


%typemap(in) char16_t* { 
  $1 = 0;
  if ($input) {
    $1 = ($1_ltype)JCALL2(GetStringChars, jenv, $input, 0);
    if (!$1) return $null;
  }
}

%typemap(directorin, descriptor="Ljava/lang/String;") char16_t* { 
  $input = 0;
  if ($1) {
    $input = JCALL1(NewString, jenv, $1);
    if (!$input) return $null;
  }
}
%typemap(freearg) char16_t* { if ($1) JCALL2(ReleaseStringChars, jenv, $input, (const jchar *) $1); }
%typemap(out) char16_t* { if($1) $result = JCALL2(NewString, jenv, (const jchar *) $1, char16_len ($1)); }
%typemap(javadirectorin) char16_t* "$jniinput"
%typemap(javadirectorout) char16_t* "$javacall"

%typemap(throws) char16_t* {
  SWIG_JavaThrowException(jenv, SWIG_JavaRuntimeException, $1);
  return $null;
}
%typemap(javain) char16_t* "$javainput"
%typemap(javaout) char16_t* {
    return $jnicall;
  }
