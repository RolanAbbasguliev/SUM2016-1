/* FILENAME: RENDER.H
 * PROGRAMMER: MM3
 * DATE: 13.06.2016
 * PURPOSE: Animation system difinitions
*/

#ifndef __RENDER_H_
#define __RENDER_H_

#include "def.h"

/* Primitive representation type */
/* Vertex representation type */
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
  INT VA;     /* Primitive vertex array */
  INT VBuf;   /* Point array size */
  INT IBuf;    /* Facets index array */
  INT NumOfI;/* Facets index array size */
  MATR M; /* Primitive transformation matrix */
  INT MtlNo; /* Material number */             

} mm3PRIM;
/* Ball unit representation type */

extern MATR MM3_RndMatrWorld, MM3_RndMatrView, MM3_RndMatrProj;
extern DBL MM3_RndProjSize, MM3_RndProjDist, MM3_RndFarClip;

VOID MM3_RndSetProj( VOID );

#endif /* __RENDER_H_ */


/* END OF 'RENDER.H' FILE */      