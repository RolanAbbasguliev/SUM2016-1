/* FILE NAME: T07ANIM.C
 * PROGRAMMER: MM3
 * DATE: 10.06.2016
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "def.h"

#define MM3_MAX_UNITS 10

typedef struct tagMM3UNIT MM3UNIT;

typedef struct tagMM3ANIM
{
  HWND hWnd; 
  HDC hDC; 
  INT W, H;
  HBITMAP hFrame;
  MM3UNIT *UNITs[MM3_MAX_UNITS];
  INT NumOfUNITs;
}MM3ANIM;

struct tagMM3UNIT
{
  VOID (* Init)( MM3UNIT *Uni, MM3ANIM *Ani );
  VOID (* Close)( MM3UNIT *Uni, MM3ANIM *Ani );
  VOID (* Response)( MM3UNIT *Uni, MM3ANIM *Ani );
  VOID (* Render)( MM3UNIT *Uni, MM3ANIM *Ani );
};

VOID MM3_Init( HWND hWnd );
VOID MM3_Resize( DBL W, DBL H );
VOID MM3_Render( VOID );
VOID MM3_CopyFrame( HDC hDC );
VOID MM3_AddUNIT( MM3UNIT *Uni );
VOID MM3_Close( VOID );

#endif

/* END OF ANIM.H */
