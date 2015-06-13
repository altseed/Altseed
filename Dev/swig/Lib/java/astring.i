/* -----------------------------------------------------------------------------
 * std_wstring.i
 *
 * Typemaps for std::wstring and const std::wstring&
 *
 * These are mapped to a Java String and are passed around by value.
 * Warning: Unicode / multibyte characters are handled differently on different 
 * OSs so the std::wstring typemaps may not always work as intended.
 *
 * To use non-const std::wstring references use the following %apply.  Note 
 * that they are passed by value.
 * %apply const std::wstring & {std::wstring &};
 * ----------------------------------------------------------------------------- */

namespace asd {

%naturalvar astring;

class astring;

// wstring
%typemap(jni) astring "jstring"
%typemap(jtype) astring "String"
%typemap(jstype) astring "String"
%typemap(javadirectorin) astring "$jniinput"
%typemap(javadirectorout) astring "$javacall"

%typemap(in) astring
%{if(!$input) {
    SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "null astring");
    return $null;
  }
  const jchar *$1_pstr = jenv->GetStringChars($input, 0);
  if (!$1_pstr) return $null;
  jsize $1_len = jenv->GetStringLength($input);
  if ($1_len) {
    $1.reserve($1_len);
    for (jsize i = 0; i < $1_len; ++i) {
      $1.push_back((wchar_t)$1_pstr[i]);
    }
  }
  jenv->ReleaseStringChars($input, $1_pstr);
 %}

%typemap(directorout) astring
%{if(!$input) {
    SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "null astring");
    return $null;
  }
  const jchar *$1_pstr = jenv->GetStringChars($input, 0);
  if (!$1_pstr) return $null;
  jsize $1_len = jenv->GetStringLength($input);
  if ($1_len) {
    $result.reserve($1_len);
    for (jsize i = 0; i < $1_len; ++i) {
      $result.push_back((wchar_t)$1_pstr[i]);
    }
  }
  jenv->ReleaseStringChars($input, $1_pstr);
 %}

%typemap(directorin,descriptor="Ljava/lang/String;") astring {
  jsize $1_len = $1.length();
  jchar *conv_buf = new jchar[$1_len];
  for (jsize i = 0; i < $1_len; ++i) {
    conv_buf[i] = (jchar)$1[i];
  }
  $input = jenv->NewString(conv_buf, $1_len);
  delete [] conv_buf;
}

%typemap(out) astring
%{jsize $1_len = $1.length();
  jchar *conv_buf = new jchar[$1_len];
  for (jsize i = 0; i < $1_len; ++i) {
    conv_buf[i] = (jchar)$1[i];
  }
  $result = jenv->NewString(conv_buf, $1_len);
  delete [] conv_buf; %}

%typemap(javain) astring "$javainput"

%typemap(javaout) astring {
    return $jnicall;
  }

//%typemap(typecheck) astring = wchar_t *;

%typemap(throws) astring
%{ std::string message($1.begin(), $1.end());
   SWIG_JavaThrowException(jenv, SWIG_JavaRuntimeException, message.c_str());
   return $null; %}

// const astring &
%typemap(jni) const astring & "jstring"
%typemap(jtype) const astring & "String"
%typemap(jstype) const astring & "String"
%typemap(javadirectorin) const astring & "$jniinput"
%typemap(javadirectorout) const astring & "$javacall"

%typemap(in) const astring &
%{if(!$input) {
    SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "null std::wstring");
    return $null;
  }
  const jchar *$1_pstr = jenv->GetStringChars($input, 0);
  if (!$1_pstr) return $null;
  jsize $1_len = jenv->GetStringLength($input);
  asd::astring $1_str;
  if ($1_len) {
    $1_str.reserve($1_len);
    for (jsize i = 0; i < $1_len; ++i) {
      $1_str.push_back((wchar_t)$1_pstr[i]);
    }
  }
  $1 = &$1_str;
  jenv->ReleaseStringChars($input, $1_pstr);
 %}

%typemap(directorout,warning=SWIGWARN_TYPEMAP_THREAD_UNSAFE_MSG) const astring & 
%{if(!$input) {
    SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "null std::astring");
    return $null;
  }
  const jchar *$1_pstr = jenv->GetStringChars($input, 0);
  if (!$1_pstr) return $null;
  jsize $1_len = jenv->GetStringLength($input);
  /* possible thread/reentrant code problem */
  static asd::astring $1_str;
  if ($1_len) {
    $1_str.reserve($1_len);
    for (jsize i = 0; i < $1_len; ++i) {
      $1_str.push_back((wchar_t)$1_pstr[i]);
    }
  }
  $result = &$1_str;
  jenv->ReleaseStringChars($input, $1_pstr); %}

%typemap(directorin,descriptor="Ljava/lang/String;") const wstring & {
  jsize $1_len = $1.length();
  jchar *conv_buf = new jchar[$1_len];
  for (jsize i = 0; i < $1_len; ++i) {
    conv_buf[i] = (jchar)($1)[i];
  }
  $input = jenv->NewString(conv_buf, $1_len);
  delete [] conv_buf;
}

%typemap(out) const astring & 
%{jsize $1_len = $1->length();
  jchar *conv_buf = new jchar[$1_len];
  for (jsize i = 0; i < $1_len; ++i) {
    conv_buf[i] = (jchar)(*$1)[i];
  }
  $result = jenv->NewString(conv_buf, $1_len);
  delete [] conv_buf; %}

%typemap(javain) const astring & "$javainput"

%typemap(javaout) const astring & {
    return $jnicall;
  }

//%typemap(typecheck) const astring & = wchar_t *;

%typemap(throws) const astring &
%{ std::string message($1.begin(), $1.end());
   SWIG_JavaThrowException(jenv, SWIG_JavaRuntimeException, message.c_str());
   return $null; %}

}

