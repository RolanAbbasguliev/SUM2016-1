/* FILENAME: RENDER.C
 * PROGRAMMER: MM3
 * DATE: 13.06.2016
 * PURPOSE: Animation system difinitions
*/

#include "anim.h"

#include "vec.h"

#include "render.h"

MATR MM3_RndMatrWorld, 
     MM3_RndMatrView, 
     MM3_RndMatrProj;

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
  POINT *pts;

  /* Build transform matrix */
  M = MatrMulMatr(MM3_RndMatrWorld,
    MatrMulMatr(MM3_RndMatrView, MM3_RndMatrProj));

  /* Transform all points */
  pts = malloc(sizeof(POINT) * Pr->NumOfP);
  if (pts == NULL)
    return;
  SelectObject(MM3_Anim.hDC, GetStockObject(DC_PEN));
  SetDCPenColor(MM3_Anim.hDC, RGB(255, 255, 255));

  for (i = 0; i < Pr->NumOfP; i++)
  {
    /* NDC */
    VEC p = PointTransform4(Pr->P[i], M);

    pts[i].x = (INT)((p.X + 1) * MM3_Anim.W / 2);
    pts[i].y = (INT)((-p.Y + 1) * MM3_Anim.H / 2);
  }

  /* Draw all lines */
  for (i = 0; i < Pr->NumOfE; i++)
  {
    INT n0 = Pr->Edges[i][0], n1 = Pr->Edges[i][1];

    SetDCPenColor(MM3_Anim.hDC, RGB(i * 255 / Pr->NumOfE, 0, 255 - i * 255 / Pr->NumOfE));

    MoveToEx(MM3_Anim.hDC, pts[n0].x, pts[n0].y, NULL);
    LineTo(MM3_Anim.hDC, pts[n1].x, pts[n1].y);
  }
  free(pts);
} /* End of 'MM3_RndPrimDraw' function */

VOID MM3_RndPrimFree( mm3PRIM *Pr )
{
  if (Pr->P != NULL)
    free(Pr->P);
  if (Pr->Edges != NULL)
    free(Pr->Edges);
  memset(Pr, 0, sizeof(mm3PRIM));
} /* End of 'DS1_RndPrimFree' function */


/* END OF 'RENDER.C' FILE */