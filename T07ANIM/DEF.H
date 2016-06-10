/* FILE NAME: T07ANIM.C
 * PROGRAMMER: MM3
 * DATE: 10.06.2016
 */

#ifndef __DEF_H_
#define __DEF_H_

#pragma warning(disable: 4244)
#pragma warning(disable: 4013)
#include <windows.h>

/* Debug memory allocation support */
#ifdef _DEBUG
#  define _CRTDBG_MAP_ALLOC
#  include <crtdbg.h>
#  define SetDbgMemHooks() \
     _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF | \
       _CRTDBG_ALLOC_MEM_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
#else /* _DEBUG */
#  define SetDbgMemHooks() ((VOID)0)
#endif /* _DEBUG */

#include <stdlib.h>
/* !!! в начале WinMain: SetDbgMemHooks(); */

typedef FLOAT FLT;
typedef DOUBLE DBL;

#include "vec.h"

#endif /* __DEF_H_ */

/* END OF DEF.H */