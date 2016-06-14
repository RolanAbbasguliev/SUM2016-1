/* FILE NAME: T07ANIM.C
 * PROGRAMMER: MM3
 * DATE: 10.06.2016
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "def.h"

#define MM3_MAX_UNITS 100

typedef struct tagMM3UNIT MM3UNIT;

typedef struct tagmm3ANIM
{
  HWND hWnd;                       
  HDC hDC;                         
  INT W, H;                        
  HBITMAP hFrame;                  
  MM3UNIT *UNITs[MM3_MAX_UNITS];   
  INT NumOfUNITs;                  
  /* Timer data */
  DBL GlobalTime, GlobalDeltaTime, 
      Time, DeltaTime,             
      FPS;                         
  BOOL IsPause;                    
  /* Input system */
  INT Mx, My,                      
      Mz,                          
      Mdx, Mdy, Mdz;               
  BYTE Keys[256],                  
       KeysOld[256],               
       KeysClick[256],             
       JBut[32];                   
  INT JPov;                      
  DBL JX, JY, JZ, JR;              
        
}mm3ANIM;

struct tagMM3UNIT
{
  VOID (* Init)( MM3UNIT *Uni, mm3ANIM *Ani );
  VOID (* Close)( MM3UNIT *Uni, mm3ANIM *Ani );
  VOID (* Response)( MM3UNIT *Uni, mm3ANIM *Ani );
  VOID (* Render)( MM3UNIT *Uni, mm3ANIM *Ani );
};

VOID MM3_Init( HWND hWnd );
VOID MM3_Resize( INT w, INT h );
VOID MM3_Render( VOID );
VOID MM3_CopyFrame( HDC hDC );
VOID MM3_AddUNIT( MM3UNIT *Uni );
VOID MM3_Close( VOID );
VOID MM3_DoExit( VOID );

MM3UNIT * MM3_UNITCreate( INT Size );

extern mm3ANIM MM3_Anim;

#endif

/* END OF ANIM.H */
