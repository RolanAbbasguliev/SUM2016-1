/* FILE NAME: U_BALL.C
 * PROGRAMMER: MM3
 * DATE: 10.06.2016
 * PURPOSE: Ball UNIT sample.
 */

#include <time.h>

#include "anim.h"

/* Ball UNIT representation type */
typedef struct
{
  MM3UNIT; 
  VEC Pos; 
  VEC Shift; 
  DBL TimerShift; 
  DBL TimerSpeed; 
  COLORREF Color; 
} MM3UNIT_BALL;

static VOID MM3_UNITInit( MM3UNIT_BALL *Uni, MM3ANIM *Ani )
{
  Uni->Pos = VecSet(Rnd0() * 1024, Rnd0() * 768, 0);
  Uni->TimerShift = Rnd1() * 59;
  Uni->TimerSpeed = Rnd1() * 8;
  Uni->Color = RGB(Rnd0() * 255, Rnd0() * 255, Rnd0() * 255);
} /* End of 'MM3_UNITInit' function */

static VOID MM3_UNITResponse( MM3UNIT_BALL *Uni, MM3ANIM *Ani )
{
  DBL t = Uni->TimerSpeed * clock() / 1000.0 + Uni->TimerShift;

  Uni->Shift = VecSet(30 * sin(t), 30 * cos(t), 0);
} /* End of 'MM3_UNITResponse' function */

static VOID MM3_UNITRender( MM3UNIT_BALL *Uni, MM3ANIM *Ani )
{
  VEC p = VecAddVec(Uni->Pos, Uni->Shift);

  SetDCBrushColor(Ani->hDC, Uni->Color);
  Ellipse(Ani->hDC, p.X - 10, p.Y - 10, p.X + 10, p.Y + 10);
} /* End of 'MM3_UNITRender' function */

MM3UNIT * MM3_UNITCreateBall( VOID )
{
  MM3UNIT_BALL *Uni;

  if ((Uni = (MM3UNIT_BALL *)MM3_UNITCreate(sizeof(MM3UNIT_BALL))) == NULL)
    return NULL;
  /* Setup UNIT methods */
  Uni->Init = (VOID *)MM3_UNITInit;
  Uni->Response = (VOID *)MM3_UNITResponse;
  Uni->Render = (VOID *)MM3_UNITRender;
  return (MM3UNIT *)Uni;
} /* End of 'MM3_UNITCreateBall' function */

/* END OF 'U_BALL.C' FILE */
