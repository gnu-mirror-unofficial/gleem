#ifndef _GLEEM_DLL_H
#define _GLEEM_DLL_H

#ifdef WIN32
// Hacks
// Disable the "identifier too long" warning
# pragma warning (disable : 4786)
// Disable the "DLL export" warning (for template members I don't care
// about anyway))
# pragma warning (disable : 4251)
// Disable the "bool coersion" warning in mstring.h
# pragma warning (disable : 4800)

# ifdef BUILDING_GLEEM_DLL
#  define GLEEMDLL __declspec(dllexport)
# else
#  define GLEEMDLL __declspec(dllimport)
# endif

#else
# define GLEEMDLL
#endif

#endif  // #defined _GLEEM_DLL_H
