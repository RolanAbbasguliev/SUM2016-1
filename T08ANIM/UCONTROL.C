/* FILENAME: SPRITE.C
 * PROGRAMMER: MM3
 * DATE: 15.06.2016
 * PURPOSE: Sprite UNIT drawing module
 */

#include <string.h>

#include "def.h"
#include "anim.h"
#include "render.h"

typedef struct
{
  MM3UNIT;
  VEC Pos;
} MM3UNIT_CONTROL;

/* UNIT ball initialization function.
 * ARGUMENTS:
 *   - self-pointer to UNIT object:
 *       MM3UNIT_BALL *Uni;
 *   - animation context:
 *       mm3ANIM *Ani;
 * RETURNS: None.
 */
static VOID MM3_UNITInit( MM3UNIT_CONTROL *Uni, mm3ANIM *Ani )
{
  Uni->Pos = VecSet(10, 0, 10);

} /* End of 'MM3_UNITInit' function */

static VOID MM3_UNITResponse( MM3UNIT_CONTROL *Uni, mm3ANIM *Ani )
{
  DBL r;

  if (Ani->Keys['T'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  if (Ani->Keys['Y'])
    glPolygonMode(GL_FRONT, GL_LINE);
  if (Ani->Keys['U'])
    glPolygonMode(GL_BACK, GL_LINE);
  if (Ani->Keys['C'])
    MM3_AddUNIT(MM3_UNITCreateCube()); 
  if (Ani->KeysClick[VK_ESCAPE])
    MM3_DoExit();
  if (Ani->KeysClick['P'])
    Ani->IsPause = !Ani->IsPause;

  /* Uni->Pos.Y += Ani->JY * Ani->GlobalDeltaTime; */
  Uni->Pos = PointTransform(Uni->Pos, MatrRotateX(59 * Ani->JY * Ani->GlobalDeltaTime));
  Uni->Pos = PointTransform(Uni->Pos, MatrRotateY(59 * Ani->JX * Ani->GlobalDeltaTime));  

  if (Ani->Keys[VK_LBUTTON])
  {
    Uni->Pos = PointTransform(Uni->Pos, MatrRotateY(-61 * Ani->Mdx * Ani->GlobalDeltaTime));
    Uni->Pos = PointTransform(Uni->Pos, MatrRotateX(61 * Ani->Mdy * Ani->GlobalDeltaTime));
    /*Uni->Pos = PointTransform(Uni->Pos, MatrRotateZ(61 * Ani->Mdz * Ani->GlobalDeltaTime));*/
  }

  Uni->Pos = PointTransform(Uni->Pos, MatrRotateY(100 * Ani->Keys[VK_RIGHT] * Ani->GlobalDeltaTime));
  Uni->Pos = PointTransform(Uni->Pos, MatrRotateY(-100 * Ani->Keys[VK_LEFT] * Ani->GlobalDeltaTime));
  Uni->Pos = PointTransform(Uni->Pos, MatrRotateX(100 * Ani->Keys[VK_UP] * Ani->GlobalDeltaTime));
  Uni->Pos = PointTransform(Uni->Pos, MatrRotateX(-100 * Ani->Keys[VK_DOWN] * Ani->GlobalDeltaTime));
  /*Uni->Pos = PointTransform(Uni->Pos, MatrRotateZ(59 * Ani->Keys[VK_BACK] * Ani->GlobalDeltaTime));*/

  r = VecLen(Uni->Pos);
  Uni->Pos = VecMulNum(Uni->Pos, (r + Ani->Mdz * Ani->DeltaTime * 0.1) / r);

  MM3_RndMatrView = MatrView(Uni->Pos, VecSet(0, 0, 0), VecSet(0, 1, 0));
} /* End of 'MM3_ControlModule' function */

/* UNIT deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to UNIT object:
 *       MM3UNIT *Uni;
 *   - animation context:
 *       mm3ANIM *Ani;
 * RETURNS: None.
 */
static VOID MM3_UNITClose( MM3UNIT_CONTROL *Uni, mm3ANIM *Ani )
{
} /* End of 'MM3_UNITClose' function */

/* UNIT render function.
 * ARGUMENTS:
 *   - self-pointer to UNIT object:
 *       MM3UNIT_BALL *Uni;
 *   - animation context:
 *       mm3ANIM *Ani;
 * RETURNS: None.
 */
static VOID MM3_UNITRender( MM3UNIT_CONTROL *Uni, mm3ANIM *Ani )
{

} /* End of 'MM3_UNITRender' function */

/* UNIT ball creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (MM3UNIT *) pointer to created UNIT.
 */
MM3UNIT * MM3_UNITCreateControl( VOID )
{
  MM3UNIT_CONTROL *Uni;

  if ((Uni = (MM3UNIT_CONTROL *)MM3_UNITCreate(sizeof(MM3UNIT_CONTROL))) == NULL)
    return NULL;
  /* Setup UNIT methods */
  Uni->Init = (VOID *)MM3_UNITInit;
  Uni->Response = (VOID *)MM3_UNITResponse;
  return (MM3UNIT *)Uni;
} /* End of 'MM3_UNITCreateControl' function */


/* END OF 'SPRITE.C' FILE */