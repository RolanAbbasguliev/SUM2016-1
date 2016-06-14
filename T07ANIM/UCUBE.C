/* FILENAME: CUBE.C
 * PROGRAMMER: DS1
 * DATE: 07.06.2016
 * PURPOSE: Sprite unit drawing module
 */

#include <string.h>

#include "anim.h"
#include "render.h"

typedef struct
{
  MM3UNIT;
  mm3PRIM Pr;
} MM3UNIT_CUBE;

VEC CubeP[] =
{
  {-1, -1,  1},
  { 1, -1,  1},
  { 1, -1, -1},
  {-1, -1, -1},
  {-1,  1,  1},
  { 1,  1,  1},
  { 1,  1, -1},
  {-1,  1, -1}
};

INT CubeE[][2] =
{
  {0, 1}, {1, 2}, {2, 3}, {3, 0},
  {4, 5}, {5, 6}, {6, 7}, {7, 4},
  {0, 4}, {1, 5}, {2, 6}, {3, 7}

  /*{0, 2}, {2, 3}, {3, 1}, {1, 0},
  {4, 6}, {6, 7}, {7, 5}, {5, 4},
  {0, 4}, {1, 5}, {2, 6}, {3, 7}*/

  /*{0, 1}, {2, 3}, {1, 7}, {3, 0},
  {4, 5}, {6, 7}, {5, 6}, {7, 4},
  {0, 4}, {1, 5}, {2, 6}, {3, 7}*/
};
mm3PRIM Cube =
{
  CubeP, sizeof(CubeP) / sizeof(CubeP[0]),
  CubeE, sizeof(CubeE) / sizeof(CubeE[0])
};

/* Unit cube initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vg4UNIT_CUBE *Uni;
 *   - animation context:
 *       vg4ANIM *Ani;
 * RETURNS: None.
 */
static VOID MM3_UnitInit( MM3UNIT_CUBE *Uni, mm3ANIM *Ani )
{
  MM3_RndPrimLoad(&Uni->Pr, "modela\\cow.g3d");
} /* End of 'VG4_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       MM3UNIT *Uni;
 *   - animation context:
 *       mm3ANIM *Ani;
 * RETURNS: None.
 */
static VOID MM3_UnitClose( MM3UNIT_CUBE *Uni, mm3ANIM *Ani )
{
  MM3_RndPrimFree(&Uni->Pr);
} /* End of 'MM3_UnitClose' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       MM3UNIT_CUBE *Uni;
 *   - animation context:
 *       mm3ANIM *Ani;
 * RETURNS: None.
 */
static VOID MM3_UnitRender( MM3UNIT_CUBE *Uni, mm3ANIM *Ani )
{
  /*DS1_RndMatrWorld = MatrixScale(1, 1, 1);/*MatrixRotate(Ani->Time, 0, 1, 0);,);*/
  /*MM3_RndPrimDraw(&Cube);*/
  MM3_RndPrimDraw(&Uni->Pr);
} /* End of 'MM3_UnitRender' function */

/* Unit cube creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (MM3UNIT *) pointer to created unit.
 */
MM3UNIT * MM3_UnitCreateCube( VOID )
{
  MM3UNIT_CUBE *Uni;

  if ((Uni = (MM3UNIT_CUBE *)MM3_UNITCreate(sizeof(MM3UNIT_CUBE))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)MM3_UnitInit;
  Uni->Render = (VOID *)MM3_UnitRender;
  Uni->Close = (VOID *)MM3_UnitClose;
  return (MM3UNIT *)Uni;
} /* End of 'MM3_UnitCreateCube' function */


/* END OF 'CUBE.C' FILE */