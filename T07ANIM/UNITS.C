/* FILE NAME: T07ANIM.C
 * PROGRAMMER: MM3
 * DATE: 10.06.2016
 */

#include "anim.h"

static VOID MM3_UNITInit( MM3UNIT *Uni, MM3ANIM *Ani )
{
} /* End of 'MM3UNITInit' function */

static VOID MM3_UNITClose( MM3UNIT *Uni, MM3ANIM *Ani )
{
} /* End of 'MM3UNITClose' function */

static VOID MM3_UNITResponse( MM3UNIT *Uni, MM3ANIM *Ani )
{
} /* End of 'MM3UNITResponse' function */

static VOID MM3_UNITRender( MM3UNIT *Uni, MM3ANIM *Ani )
{
} /* End of 'MM3UNITRender' function */

MM3UNIT * MM3_UNITCreate( INT Size )
{
  MM3UNIT *Uni;

  /* Memory allocation */
  if (Size < sizeof(MM3UNIT) ||
      (Uni = malloc(Size)) == NULL)
    return NULL;
  memset(Uni, 0, Size);
  /* Setup UNIT methods */
  Uni->Init = MM3_UNITInit;
  Uni->Close = MM3_UNITClose;
  Uni->Response = MM3_UNITResponse;
  Uni->Render = MM3_UNITRender;
  return Uni;
} /* End of 'MM3ANIMUNITCreate' function */

/* END OF UNITS.C */
