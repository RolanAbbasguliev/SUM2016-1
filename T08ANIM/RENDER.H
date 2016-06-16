/* FILENAME: RENDER.H
 * PROGRAMMER: MM3
 * DATE: 13.06.2016
 * PURPOSE: Animation system difinitions
*/

#ifndef __RENDER_H_
#define __RENDER_H_

#include "def.h"

typedef struct tagmm3COLOR
{
  FLT X, Y, Z, A;
} mm3COLOR; 

MATR MM3_RndMatrWorld, 
     MM3_RndMatrView, 
     MM3_RndMatrProj;

/* Primitive representation type */
typedef struct
{
  VEC  P;  /* Vertex position */
  VEC2 T;  /* Vertex texture coordinates */
  VEC  N;  /* Normal at vertex */
  VEC4 C;  /* Vertex color */
} mm3VERTEX;

/* Primitive representation type */
typedef struct
{
  /* OpenGL buffers */
  INT
    VA,   /* Vertex array */
    VBuf, /* Vertex buffer */
    IBuf; /* Index buffer */
  INT NumOfI;       /* Facets index array size */
  MATR M; /* Primitive transformation matrix */
/*  mm3VERTEX *V;     /* Primitive vertex array * /
  INT NumOfV;       /* Point array size * /
  INT *I;           /* Facets index array * /
  INT NumOfI;       /* Facets index array size */
} mm3PRIM;

typedef struct
{
  mm3PRIM *Prims;
  INT NumOfPrims;
} mm3OBJ;

extern MATR MM3_RndMatrWorld, MM3_RndMatrView, MM3_RndMatrProj;
extern DBL MM3_RndProjSize, MM3_RndProjDist, MM3_RndFarClip;
extern UINT MM3_RndPrg;
/* Setup projection function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID MM3_RndSetProj( VOID );

/* Primitive draw function.
 * ARGUMENTS:
 *   - primtive to draw:
 *       mm3PRIM *Pr;
 * RETURNS: None.
 */
VOID MM3_RndPrimDraw( mm3PRIM *Pr );

BOOL MM3_RndPrimLoad( mm3PRIM *Pr, CHAR *FileName );

VOID MM3_RndSetProj( VOID );
VOID MM3_RndObjDraw( mm3OBJ *Obj );
BOOL MM3_RndObjLoad( mm3OBJ *Obj, CHAR *FileName );
VOID MM3_RndObjFree( mm3OBJ *Obj );
UINT MM3_RndShaderLoad( CHAR *FileNamePrefix );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */      