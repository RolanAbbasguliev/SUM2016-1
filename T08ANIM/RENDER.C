/* FILENAME: RENDER.C
 * PROGRAMMER: MM3
 * DATE: 13.06.2016
 * PURPOSE: Animation system difinitions
*/

#include "anim.h"

#include "vec.h"

#include "render.h"

DBL MM3_RndProjSize = 1, MM3_RndProjDist = 1, MM3_RndFarClip = 100;

UINT MM3_RndPrg;

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
  INT loc;
  MATR M;

  /* Build transform matrix */
  M = MatrMulMatr(MM3_RndMatrWorld,
    MatrMulMatr(MM3_RndMatrView, MM3_RndMatrProj));
  glLoadMatrixf(M.A[0]);

  glUseProgram(MM3_RndPrg);

  /* Setup global variables */
  if ((loc = glGetUniformLocation(MM3_RndPrg, "MatrWorld")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, MM3_RndMatrWorld.A[0]);
  if ((loc = glGetUniformLocation(MM3_RndPrg, "MatrView")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, MM3_RndMatrView.A[0]);
  if ((loc = glGetUniformLocation(MM3_RndPrg, "MatrProj")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, MM3_RndMatrProj.A[0]);
  if ((loc = glGetUniformLocation(MM3_RndPrg, "Time")) != -1)
    glUniform1f(loc, MM3_Anim.Time);


  /* Activete primitive vertex array */
  glBindVertexArray(Pr->VA);
  /* Activete primitive index buffer */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
  /* Draw primitive */
  glDrawElements(GL_TRIANGLES, Pr->NumOfI, GL_UNSIGNED_INT, NULL);
  glUseProgram(0);
} /* End of 'MM3_RndPrimDraw' function */

/* Primitive free function.
 * ARGUMENTS:
 *   - primtive to free:
 *       mm3PRIM *Pr;
 * RETURNS: None.
 */
VOID MM3_RndPrimFree( mm3PRIM *Pr )
{
  glBindVertexArray(Pr->VA);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &Pr->VBuf);
  glBindVertexArray(0);
  glDeleteVertexArrays(1, &Pr->VA);
  glDeleteBuffers(1, &Pr->IBuf);
  memset(Pr, 0, sizeof(mm3PRIM));
} /* End of 'MM3_RndPrimFree' function */

/* Primitive draw function.
 * ARGUMENTS:
 *   - primtive to draw:
 *       mm3PRIM *Pr;
 * RETURNS: None.
 * /
VOID MM3_RndPrimDraw( mm3PRIM *Pr )
{
  INT i;
  MATR M;

  /* Build transform matrix * /
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


  /* Draw all lines * /
  glBegin(GL_TRIANGLES);
  for (i = 0; i < Pr->NumOfI; i++)
  {
    glColor3fv(&Pr->V[Pr->I[i]].C.X);
    glVertex3fv(&Pr->V[Pr->I[i]].P.X);
  }
  glEnd();
} /* End of 'MM3_RndPrimDraw' function * /

VOID MM3_RndPrimFree( mm3PRIM *Pr )
{
  if (Pr->V != NULL)
    free(Pr->V);
  if (Pr->I != NULL)
    free(Pr->I);
  memset(Pr, 0, sizeof(mm3PRIM));
} /* End of 'MM3_RndPrimFree' function */
/* Object draw function.
 * ARGUMENTS:
 *   - object structure pointer:
 *       mm3OBJ *Obj;
 * RETURNS: None.
 */
VOID MM3_RndObjDraw( mm3OBJ *Obj )
{
  INT i;
  INT loc;
  MATR M, MSave;

  for (i = 0; i < Obj->NumOfPrims; i++)
  {
    /* Build transform matrix */
    MSave = MM3_RndMatrWorld;
    MM3_RndMatrWorld = MatrMulMatr(MM3_RndMatrWorld, Obj->Prims[i].M);
    M = MatrMulMatr(MM3_RndMatrWorld,
      MatrMulMatr(MM3_RndMatrView, MM3_RndMatrProj));
    glLoadMatrixf(M.A[0]);

    glUseProgram(MM3_RndPrg);

    /* Setup global variables */
    if ((loc = glGetUniformLocation(MM3_RndPrg, "MatrWVP")) != -1)
      glUniformMatrix4fv(loc, 1, FALSE, M.A[0]);
    if ((loc = glGetUniformLocation(MM3_RndPrg, "MatrWorld")) != -1)
      glUniformMatrix4fv(loc, 1, FALSE, MM3_RndMatrWorld.A[0]);
    if ((loc = glGetUniformLocation(MM3_RndPrg, "MatrView")) != -1)
      glUniformMatrix4fv(loc, 1, FALSE, MM3_RndMatrView.A[0]);
    if ((loc = glGetUniformLocation(MM3_RndPrg, "MatrProj")) != -1)
      glUniformMatrix4fv(loc, 1, FALSE, MM3_RndMatrProj.A[0]);
    if ((loc = glGetUniformLocation(MM3_RndPrg, "Time")) != -1)
      glUniform1f(loc, MM3_Anim.Time);
    if ((loc = glGetUniformLocation(MM3_RndPrg, "PartNo")) != -1)
      glUniform1i(loc, i);

    /* Activete primitive vertex array */
    glBindVertexArray(Obj->Prims[i].VA);
    /* Activete primitive index buffer */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Obj->Prims[i].IBuf);
    /* Draw primitive */
    glDrawElements(GL_TRIANGLES, Obj->Prims[i].NumOfI, GL_UNSIGNED_INT, NULL);
    glUseProgram(0);
    MM3_RndMatrWorld = MSave;
  }
} /* End of 'MM3_RndObjDraw' function */

/* Object free function.
 * ARGUMENTS:
 *   - object structure pointer:
 *       mm3OBJ *Obj;
 * RETURNS: None.
 */
VOID MM3_RndObjFree( mm3OBJ *Obj )
{
  INT i;

  for (i = 0; i < Obj->NumOfPrims; i++)
  {
    glBindVertexArray(Obj->Prims[i].VA);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &Obj->Prims[i].VBuf);
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &Obj->Prims[i].VA);
    glDeleteBuffers(1, &Obj->Prims[i].IBuf);
  }
  free(Obj->Prims);
  memset(Obj, 0, sizeof(mm3OBJ));
} /* End of 'MM3_RndObjFree' function */

/* END OF 'RENDER.C' FILE */
