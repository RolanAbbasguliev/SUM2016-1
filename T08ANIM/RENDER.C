/* FILENAME: RENDER.C
 * PROGRAMMER: MM3
 * DATE: 13.06.2016
 * PURPOSE: Animation system difinitions
*/

#include "anim.h"

#include "vec.h"

#include "render.h"

DBL MM3_RndProjSize = 1, MM3_RndProjDist = 1, MM3_RndFarClip = 100;

/* Setup projection function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID MM3_RndSetProj( VOID )
{
  DBL ratio_x = 1, ratio_y = 1;

  if (MM3_Anim.W >= MM3_Anim.H)
    ratio_x = (DBL)MM3_Anim.W / MM3_Anim.H;
  else
    ratio_y = (DBL)MM3_Anim.H / MM3_Anim.W;

  MM3_RndMatrProj = MatrFrustum(-ratio_x * MM3_RndProjSize / 2,
                                 ratio_x * MM3_RndProjSize / 2,
                                -ratio_y * MM3_RndProjSize / 2,
                                 ratio_y * MM3_RndProjSize / 2,
                                 MM3_RndProjDist, MM3_RndFarClip);
  /*MatrOrtho*/
} /* End of 'MM3_RndSetProj' function */


/* Primitive draw function.
 * ARGUMENTS:
 *   - primtive to draw:
 *       mm3PRIM *Pr;
 * RETURNS: None.
 */
VOID MM3_RndPrimDraw( mm3PRIM *Pr )
{
  INT i;
  MATR M;

  /* Build transform matrix */
  M = MatrMulMatr(MM3_RndMatrWorld,
    MatrMulMatr(MM3_RndMatrView, MM3_RndMatrProj));
  glLoadMatrixf(M.A[0]);

  glBegin(GL_LINES);
    glColor3d(1, 0, 0);
    glVertex3d(0, 0, 0);
    glVertex4d(1, 0, 0, 0);
    glColor3d(0, 1, 0);
    glVertex3d(0, 0, 0);
    glVertex4d(0, 1, 0, 0);
    glColor3d(0, 0, 1);
    glVertex3d(0, 0, 0);
    glVertex4d(0, 0, 1, 0);
  glEnd();


  /* Draw all lines */
  glBegin(GL_TRIANGLES);
  for (i = 0; i < Pr->NumOfI; i++)
  {
    glColor3fv(&Pr->V[Pr->I[i]].C.X);
    glVertex3fv(&Pr->V[Pr->I[i]].P.X);
  }
  glEnd();
} /* End of 'MM3_RndPrimDraw' function */

VOID MM3_RndPrimFree( mm3PRIM *Pr )
{
  if (Pr->V != NULL)
    free(Pr->V);
  if (Pr->I != NULL)
    free(Pr->I);
  memset(Pr, 0, sizeof(mm3PRIM));
} /* End of 'MM3_RndPrimFree' function */


/* END OF 'RENDER.C' FILE */