/* FILE NAME: T07ANIM.C
 * PROGRAMMER: MM3
 * DATE: 10.06.2016
 */

#include "anim.h"
#include <mmsystem.h> 

#pragma comment(lib, "winmm")


#define MM3_GET_JOYSTIC_AXIS(A) \
  (2.0 * (ji.dw##A##pos - jc.w##A##min) / (jc.w##A##max - jc.w##A##min - 1) - 1)

mm3ANIM MM3_Anim;

VOID MM3_Init( HWND hWnd )
{
  HDC hDC;

  memset(&MM3_Anim, 0, sizeof(mm3ANIM));

  MM3_Anim.hWnd = hWnd;
  hDC = GetDC(hWnd);
  MM3_Anim.hDC = CreateCompatibleDC(hDC);
  ReleaseDC(hWnd, hDC);
}

VOID MM3_Resize( DBL W, DBL H )
{
  HDC hDC;

  MM3_Anim.W = W;
  MM3_Anim.H = H;

  if (MM3_Anim.hFrame != NULL)
    DeleteObject(MM3_Anim.hFrame);
  hDC = GetDC(MM3_Anim.hWnd);
  MM3_Anim.hFrame = CreateCompatibleBitmap(hDC, W, H);
  ReleaseDC(MM3_Anim.hWnd, hDC);
  SelectObject(MM3_Anim.hDC, MM3_Anim.hFrame);
}

VOID MM3_Render( VOID )
{ 
  INT i;

  if (joyGetNumDevs() > 0)
  {
    JOYCAPS jc;

    /* Get joystick info */
    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;

      ji.dwSize = sizeof(JOYINFOEX);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
        /* Buttons */
        for (i = 0; i < 32; i++)
          MM3_Anim.JBut[i] = (ji.dwButtons >> i) & 1;

        /* Axes */
        MM3_Anim.JX = MM3_GET_JOYSTIC_AXIS(X);
        MM3_Anim.JY = MM3_GET_JOYSTIC_AXIS(Y);
        MM3_Anim.JZ = MM3_GET_JOYSTIC_AXIS(Z);
        MM3_Anim.JR = MM3_GET_JOYSTIC_AXIS(R);

        /* Point of view */
       MM3_Anim.JPov = ji.dwPOV == 0xFFFF ? 0 : ji.dwPOV / 4500 + 1;
      }
    }
  }

  
  for (i = 0; i < MM3_Anim.NumOfUNITs; i++)
    MM3_Anim.UNITs[i]->Response(MM3_Anim.UNITs[i], &MM3_Anim);

  SelectObject(MM3_Anim.hDC, GetStockObject(NULL_PEN));
  SelectObject(MM3_Anim.hDC, GetStockObject(DC_BRUSH));

  SetDCBrushColor(MM3_Anim.hDC, RGB(100, 155, 220));

  Rectangle(MM3_Anim.hDC, 0, 0, MM3_Anim.W + 1, MM3_Anim.H + 1);

  for (i = 0; i < MM3_Anim.NumOfUNITs; i++)
  {   
    MM3_Anim.UNITs[i]->Render(MM3_Anim.UNITs[i], &MM3_Anim);
  }

  DrawSphere(MM3_Anim.hDC, 500, 500, 350);

   /*for (i = 0; i < MM3_Anim.NumOfUNITs; i++)
  MM3_Anim.UNITs[i]->Response(MM3_Anim.UNITs[i], &MM3_Anim);

  SelectObject(MM3_Anim.hDC, GetStockObject(NULL_PEN));
  SelectObject(MM3_Anim.hDC, GetStockObject(DC_BRUSH));

  SetDCBrushColor(MM3_Anim.hDC, RGB(100, 155, 220));

  Rectangle(MM3_Anim.hDC, 0, 0, MM3_Anim.W + 1, MM3_Anim.H + 1);

  DrawSphere( MM3_Anim.hDC, 500, 500, 350 );

  for (i = 0; i < MM3_Anim.NumOfUNITs; i++)
  {
    SelectObject(MM3_Anim.hDC, GetStockObject(DC_PEN));
    SelectObject(MM3_Anim.hDC, GetStockObject(DC_BRUSH));

    SetDCPenColor(MM3_Anim.hDC, RGB(0, 0, 0));
    SetDCPenColor(MM3_Anim.hDC, RGB(255, 255, 255));
  }*/
}

VOID MM3_CopyFrame( HDC hDC )
{
  BitBlt(hDC, 0, 0, MM3_Anim.W, MM3_Anim.H, MM3_Anim.hDC, 0, 0, SRCCOPY);
}

VOID MM3_AddUNIT( MM3UNIT *Uni )
{
  if (MM3_Anim.NumOfUNITs < MM3_MAX_UNITS)
  {
    MM3_Anim.UNITs[MM3_Anim.NumOfUNITs++] = Uni;
    Uni->Init(Uni, &MM3_Anim);
  }
}

VOID MM3_Close( VOID )
{
  INT i;

  for (i = 0; i < MM3_Anim.NumOfUNITs; i++)
  {
    MM3_Anim.UNITs[i]->Close(MM3_Anim.UNITs[i], &MM3_Anim);
    free(MM3_Anim.UNITs[i]);
  }
  MM3_Anim.NumOfUNITs = 0;
  DeleteDC(MM3_Anim.hDC);
  DeleteObject(MM3_Anim.hFrame);
  memset(&MM3_Anim, 0, sizeof(mm3ANIM));
}

/* END OF ANIM.C */
