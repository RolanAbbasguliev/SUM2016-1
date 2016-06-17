/* FILE NAME: T07ANIM.C
 * PROGRAMMER: MM3
 * DATE: 10.06.2016
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#pragma warning(disable: 4244)


#define MM3_MAX_UNITS 10000

#include "def.h"
#include "render.h"

typedef struct tagmm3UNIT mm3UNIT;

typedef struct tagmm3ANIM
{
  HWND hWnd;
  HDC hDC;
  INT W, H;
  HGLRC hGLRC;
  HBITMAP hFrame;
  mm3UNIT *Units[MM3_MAX_UNITS];
  INT NumOfUnits;
  DBL
    GlobalTime, GlobalDeltaTime,
    Time, DeltaTime,
    FPS;
  BOOL IsPause;
  INT 
    Mx, My,
    Mz, 
    Mdx, Mdy, Mdz;
  BYTE 
    Keys[256],
    KeysOld[256],
    KeysClick[256];
  BYTE
  JBut[32];                      /* Joystick button state */
  INT JPov;                      /* Joystick point-of-view control [0,1..8] */
  DBL JX, JY, JZ, JR; 
} mm3ANIM;

struct tagmm3UNIT
{
  VOID (*Init)( mm3UNIT *Uni, mm3ANIM *Ani );
  VOID (*Close)( mm3UNIT *Uni, mm3ANIM *Ani );
  VOID (*Response)( mm3UNIT *Uni, mm3ANIM *Ani );
  VOID (*Render)( mm3UNIT *Uni, mm3ANIM *Ani );
};

typedef struct
{
  mm3PRIM *Prims;
  INT NumOfPrims;
} mm3OBJ;

/* Material representation type */
typedef struct
{
  CHAR Name[300]; /* Material name */
  VEC Ka, Kd, Ks; /* Illumination coefficients */
  FLT Ph, Trans;  /* Shininess and Phong, transparency */
  INT TexW, TexH; /* Textiure image size */
  INT TexNo;      /* For load: bytes per pixel, in use: OpenGL texture no */
} mm3MTL;
extern UINT MM3_RndPrg;

/* Materials array */
#define MM3_MAX_MATERIALS 10000
extern mm3MTL MM3_RndMaterials[MM3_MAX_MATERIALS];
extern INT MM3_RndNumOfMaterials;

VOID MM3_RndSetProj( VOID );
VOID MM3_RndObjDraw( mm3OBJ *Obj );
BOOL MM3_RndObjLoad( mm3OBJ *Obj, CHAR *FileName );
VOID MM3_RndObjFree( mm3OBJ *Obj );
INT MM3_RndFindMaterial( CHAR *Name );
VOID MM3_RndLoadMaterials( CHAR *FileName );

extern mm3ANIM MM3_Anim;

VOID MM3_AnimCopyFrame( HDC hDC );
VOID MM3_AnimUnit( HWND hWnd );
VOID MM3_AnimAddUnit( mm3UNIT *Uni );
VOID MM3_AnimClose( VOID );
VOID IK3_Reasize( INT W, INT H );
VOID MM3_AnimRender( VOID );
VOID MM3_AnimDoExit( VOID );
mm3UNIT * MM3_AnimUnitCreate( INT Size );
VOID MM3_AnimFlipFullScreen( VOID );
VOID MM3_RndShaderFree( UINT Prg );
UINT MM3_RndShaderLoad( CHAR *FileNamePrefix );

#endif

/* END OF ANIM.H */
