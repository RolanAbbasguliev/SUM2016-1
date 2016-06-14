/* FILENAME: RENDER.H
 * PROGRAMMER: MM3
 * DATE: 13.06.2016
 * PURPOSE: Animation system difinitions
*/

#ifndef __RENDER_H_
#define __RENDER_H_

#include "def.h"

/* Primitive representation type */
typedef struct
{
  VEC *P;          /* Primitive points array */
  INT NumOfP;      /* Point array size */
  INT (*Edges)[2]; /* Edges array - array of point pairs */
  INT NumOfE;      /* Edges array size */
} mm3PRIM;

extern MATR MM3_RndMatrWorld, MM3_RndMatrView, MM3_RndMatrProj;
extern DBL MM3_RndProjSize, MM3_RndProjDist, MM3_RndFarClip;

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


#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */