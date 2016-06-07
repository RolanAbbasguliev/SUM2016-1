/* FILE NAME: T05SPHR.C
 * PROGRAMMER: MM3
 * DATE: 07.06.2016
 * PURPOSE: WinAPI windowed application sample.
 */
#include <math.h>
#include <time.h>
#include <windows.h>
#include "sphere.h"

#define M 30
#define N 15

#define PI 3.14159265358979323846

#pragma warning(disable: 4244)

VOID DrawSphere( HDC hDC, INT Xc, INT Yc, INT R )
{
  INT i, j;
  DOUBLE theta, phi, phase = clock() / 5000.0;
  DOUBLE x, y;

  SelectObject(hDC, GetStockObject(DC_PEN));
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  
  SetDCPenColor(hDC, RGB(0, 0, 255));
  SetDCBrushColor(hDC, RGB(100, 155, 220));

  /* By points */
  for (i = 0; i < N; i++)
  {
    theta = i * PI / N;
    for (j = 0; j < M; j++)
    {
      phi = (j * 2 * PI / (M - 1)) + phase;
      x = Xc + R * sin(theta) * cos(phi);
      y = Yc - R * cos(theta);

      Ellipse(hDC, x - 5, y - 5, x + 5, y + 5);   
    }
  }  

  /* Horizontal */
  for (i = 0; i < N; i++)
  {
    theta = i * PI / N;
    x = Xc + R * sin(theta) * cos(phi);
    y = Yc - R * cos(theta);
    MoveToEx(hDC, x, y, NULL);
    for (j = 1; j < M; j++)
    {
      phi = j * 2 * PI / (M - 1) + phase;

      x = Xc + R * sin(theta) * cos(phi);
      y = Yc - R * cos(theta);
      LineTo(hDC, x, y);
    }
  }

  /* Vertical */
  for (j = 0; j < M; j++)
  {
    phi = j * 2 * PI / (M - 1) + phase;
    x = Xc;
    y = Yc - R;
    MoveToEx(hDC, x, y, NULL);
    for (i = 1; i < N; i++)
    {
      theta = i * PI / (N - 1);
      x = Xc + R * sin(theta) * cos(phi);
      y = Yc - R * cos(theta);
      LineTo(hDC, x, y);
    }
  }

  SetDCPenColor(hDC, RGB(0, 0, 0));
  SetDCBrushColor(hDC, RGB(0, 0, 0));
} /* End of DrawSphere func */

/* END OF SPHERE.C */