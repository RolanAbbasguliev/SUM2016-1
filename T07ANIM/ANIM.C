/* FILE NAME: T07ANIM.C
 * PROGRAMMER: MM3
 * DATE: 10.06.2016
 */

#include "anim.h"

MM3ANIM mm3_Anim;

VOID MM3_Init( HWND hWnd )
{
  HDC hDC;

  mm3_Anim.hWnd = hWnd;
  hDC = GetDC(hWnd);
  mm3_Anim.hDC = CreateCompatibleDC(hDC);
  ReleaseDC(hWnd, hDC);
}

VOID MM3_Resize( DBL W, DBL H )
{
  HDC hDC;

  mm3_Anim.W = W;
  mm3_Anim.H = H;

  if (mm3_Anim.hFrame != NULL)
    DeleteObject(mm3_Anim.hFrame);
  hDC = GetDC(mm3_Anim.hWnd);
  mm3_Anim.hFrame = CreateCompatibleBitmap(hDC, W, H);
  ReleaseDC(mm3_Anim.hWnd, hDC);
  SelectObject(mm3_Anim.hDC, mm3_Anim.hFrame);
}

VOID MM3_Render( VOID )
{ 
  INT i;

  for (i = 0; i < mm3_Anim.NumOfUNITs; i++)
    mm3_Anim.UNITs[i]->Response(mm3_Anim.UNITs[i], &mm3_Anim);

  SelectObject(mm3_Anim.hDC, GetStockObject(NULL_PEN));
  SelectObject(mm3_Anim.hDC, GetStockObject(DC_BRUSH));

  SetDCBrushColor(mm3_Anim.hDC, RGB(100, 155, 220));

  Rectangle(mm3_Anim.hDC, 0, 0, mm3_Anim.W + 1, mm3_Anim.H + 1);

  for (i = 0; i < mm3_Anim.NumOfUNITs; i++)
  {
    SelectObject(mm3_Anim.hDC, GetStockObject(NULL_PEN));
    SelectObject(mm3_Anim.hDC, GetStockObject(DC_BRUSH));

    SetDCPenColor(mm3_Anim.hDC, RGB(0, 0, 0));
    SetDCPenColor(mm3_Anim.hDC, RGB(255, 255, 255));
  }
} 


VOID MM3_CopyFrame( HDC hDC )
{
  BitBlt(hDC, 0, 0, mm3_Anim.W, mm3_Anim.H, mm3_Anim.hDC, 0, 0, SRCCOPY);
}

VOID MM3_AddUNIT( MM3UNIT *Uni )
{
  if (mm3_Anim.NumOfUNITs < MM3_MAX_UNITS)
  {
    mm3_Anim.UNITs[mm3_Anim.NumOfUNITs++] = Uni;
    Uni->Init(Uni, &mm3_Anim);
  }
}

VOID MM3_Close( VOID )
{
  INT i;

  for (i = 0; i < mm3_Anim.NumOfUNITs; i++)
  {
    mm3_Anim.UNITs[i]->Close(mm3_Anim.UNITs[i], &mm3_Anim);
    free(mm3_Anim.UNITs[i]);
  }
  mm3_Anim.NumOfUNITs = 0;
}

/* END OF ANIM.C */
