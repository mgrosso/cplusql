AC_DEFUN([AC_CXX_NAMESPACES],
[AC_CACHE_CHECK(whether the compiler implements namespaces,
ac_cv_cxx_namespaces,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([namespace Outer { namespace Inner { int i = 0; }}],
                [using namespace Outer::Inner; return i;],
 ac_cv_cxx_namespaces=yes, ac_cv_cxx_namespaces=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_namespaces" = yes; then
  AC_DEFINE(HAVE_NAMESPACES,,[define if the compiler implements namespaces])
fi
])

AC_DEFUN([AC_CXX_HAVE_HASH_MAP],
[AC_CACHE_CHECK(whether the compiler has hash_map,
ac_cv_cxx_have_hash_map,
[AC_REQUIRE([AC_CXX_NAMESPACES])
  AC_LANG_SAVE
  AC_LANG_CPLUSPLUS
  AC_TRY_COMPILE([#include <hash_map>
#ifdef HAVE_NAMESPACES
using namespace std;
#endif],[hash_map<int, int> t; return 0;],
  ac_cv_cxx_have_hash_map=yes, ac_cv_cxx_have_hash_map=no)
  AC_LANG_RESTORE
])
if test "$ac_cv_cxx_have_hash_map" = yes; then
   AC_DEFINE(HAVE_HASH_MAP,,[define if the compiler has hash_map])
fi
])

AC_DEFUN([AC_CXX_HAVE_EXT_HASH_MAP],
[AC_CACHE_CHECK(whether the compiler has ext/hash_map,
ac_cv_cxx_have_ext_hash_map,
[AC_REQUIRE([AC_CXX_NAMESPACES])
  AC_LANG_SAVE
  AC_LANG_CPLUSPLUS
  AC_TRY_COMPILE([#include <ext/hash_map>
#ifdef HAVE_NAMESPACES
using namespace std;
#endif],[hash_map<int, int> t; return 0;],
  ac_cv_cxx_have_ext_hash_map=yes, ac_cv_cxx_have_ext_hash_map=no)
  AC_LANG_RESTORE
])
if test "$ac_cv_cxx_have_ext_hash_map" = yes; then
   AC_DEFINE(HAVE_EXT_HASH_MAP,,[define if the compiler has ext/hash_map])
fi
])

AC_DEFUN([AC_CXX_HAVE_GNU_CXX_EXT_HASH_MAP],
[AC_CACHE_CHECK(whether the compiler has ext/hash_map that requires __gnu_cxx namespace,
ac_cv_cxx_have_gnu_cxx_ext_hash_map,
[AC_REQUIRE([AC_CXX_NAMESPACES])
  AC_LANG_SAVE
  AC_LANG_CPLUSPLUS
  AC_TRY_COMPILE([#include <ext/hash_map>
using namespace std;
using namespace __gnu_cxx ;
],[hash_map<int, int> t; return 0;],
  ac_cv_cxx_have_gnu_cxx_ext_hash_map=yes, ac_cv_cxx_have_gnu_cxx_ext_hash_map=no)
  AC_LANG_RESTORE
])
if test "$ac_cv_cxx_have_gnu_cxx_ext_hash_map" = yes; then
   AC_DEFINE(HAVE_EXT_HASH_MAP,,[define if the compiler has ext/hash_map])
   AC_DEFINE(HAVE_GNU_CXX_HASH_MAP,,[define if the compiler needs namespace __gnu_cxx for hash_map])
fi
])

AC_DEFUN([AC_CXX_HAVE_GNU_GLOB],
[AC_CACHE_CHECK(whether glob.h defines the gnu extensions GLOB_TILDE and GLOB_BRACE,
ac_cv_cxx_have_gnu_glob,
[AC_LANG_SAVE
  AC_LANG_CPLUSPLUS
  AC_TRY_COMPILE([#include <glob.h>
#define SOMEGLOB_OPT GLOB_BRACE | GLOB_TILDE
],[int x=SOMEGLOB_OPT ; return 0;],
  ac_cv_cxx_have_gnu_glob=yes, ac_cv_cxx_have_gnu_glob=no)
  AC_LANG_RESTORE
])
if test "$ac_cv_cxx_have_gnu_glob" = yes; then
   AC_DEFINE(HAVE_GNU_GLOB,,[define if glob.h has gnu extensions GLOB_TILDE and GLOB_BRACE])
fi
])

AC_DEFUN([AC_CXX_HAVE_CTIME_R],
[AC_CACHE_CHECK(whether ctime_r is supported,
ac_cv_cxx_have_ctime_r,
[AC_LANG_SAVE
  AC_LANG_CPLUSPLUS
  AC_TRY_COMPILE([#include <time.h>
],[time_t t=time(NULL);
char buf[52]; 
ctime_r(&t, buf);
return 0;],
  ac_cv_cxx_have_ctime_r=yes, ac_cv_cxx_have_ctime_r=no)
  AC_LANG_RESTORE
])
if test "$ac_cv_cxx_have_ctime_r" = yes; then
   AC_DEFINE(HAVE_CTIME_R,,[define if ctime_r works.])
fi
])

