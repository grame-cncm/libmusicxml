
#ifndef __exports__
#define __exports__

#if defined(WIN32) // && !defined (GCC)

# ifdef VC6
#  pragma warning (disable : 4267)
#  pragma warning (disable : 4275)
#  pragma warning (disable : 4251)
#  pragma warning (disable : 4786)

# elif defined VC2005
#  pragma warning (disable : 4251)
#  pragma warning (disable : 4275)
# endif

# ifdef LIBMUSICXML_EXPORTS
#  define EXP __declspec(dllexport)

# elif defined(LIBMUSICXML_STATIC)
#  define EXP

# else
#  define EXP __declspec(dllimport)
# endif

#else

# ifdef LIBMUSICXML_EXPORTS
#  define EXP	__attribute__ ((visibility("default")))
# else
#  define EXP
# endif

#endif

#endif

