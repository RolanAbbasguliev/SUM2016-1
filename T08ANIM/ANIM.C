/* FILE NAME: T07ANIM.C
 * PROGRAMMER: MM3
 * DATE: 10.06.2016
 */

#pragma comment(lib, "winmm")
#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")
#pragma comment(lib, "glew32s")

#include <stdio.h>

#include "anim.h"

#include <mmsystem.h>

#include "vec.h"

/*#pragma comment(lib, "winmm")*/

#include "render.h"

#define MM3_GET_JOYSTICK_AXIS(A) \
  (2.0 * (ji.dw##A##pos - jc.w##A##min) / (jc.w##A##max - jc.w##A##min - 1) - 1)

/* Timer local data */
static UINT64
  MM3_StartTime,    /* Start program time */
  MM3_OldTime,      /* Time from program start to previous frame */
  MM3_OldTimeFPS,   /* Old time FPS measurement */
  MM3_PauseTime,    /* Time during pause period */
  MM3_TimePerSec,   /* Timer resolution */
  MM3_FrameCounter; /* Frames counter */

/* Global mouse wheel delta value */
INT MM3_MouseWheel = 0;

/* Global animation context */
mm3ANIM MM3_Anim;

/* Global system info on display */
BOOL MM3_IsSysInfo = 0, MM3_IsKeyInfo = 0;

/* Animation system initialization function.
 * ARGUMENTS:
 *   - window:
 *       HWND hWnd;
 * RETURNS: None.
 */
VOID MM3_AnimInit( HWND hWnd )
{
  INT i;
  PIXELFORMATDESCRIPTOR pfd = {0};
  LARGE_INTEGER t;

  /* Store window and create memory device context */
  MM3_Anim.hWnd = hWnd;
  MM3_Anim.hDC = GetDC(hWnd);
  /* OpenGL init: pixel format setup */
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;
  i = ChoosePixelFormat(MM3_Anim.hDC, &pfd);
  DescribePixelFormat(MM3_Anim.hDC, i, sizeof(pfd), &pfd);
  SetPixelFormat(MM3_Anim.hDC, i, &pfd);

  /* OpenGL init: setup rendering context */
  MM3_Anim.hGLRC = wglCreateContext(MM3_Anim.hDC);
  wglMakeCurrent(MM3_Anim.hDC, MM3_Anim.hGLRC);

  /* OpenGL init: setup extensions: GLEW library */
  if (glewInit() != GLEW_OK ||
      !(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
  {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(MM3_Anim.hGLRC);
    ReleaseDC(MM3_Anim.hWnd, MM3_Anim.hDC);
    exit(0);
  }
  /* OpenGL specific initialization */
  glClearColor(0.3, 0.5, 0.7, 1);
  glEnable(GL_DEPTH_TEST);
  /* glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); */

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  /*
  memset(&MM3_Anim, 0, sizeof(mm3ANIM));
  MM3_Anim.hWnd = hWnd;
  hDC = GetDC(hWnd);
  MM3_Anim.hDC = CreateCompatibleDC(hDC);

  ReleaseDC(hWnd, hDC);
  MM3_Anim.NumOfUnits = 0;
  */
  /* Timer initialization */
  QueryPerformanceFrequency(&t);
  MM3_TimePerSec = t.QuadPart;
  QueryPerformanceCounter(&t);
  MM3_StartTime = MM3_OldTime = MM3_OldTimeFPS = t.QuadPart;
  MM3_PauseTime = 0;

  MM3_RndMatrView = MatrView(VecSet(0, 1, 0), VecSet(0, 0, 0), VecSet(0, 1, 0));
  MM3_RndMatrWorld = MatrIdentity();
  MM3_RndMatrProj = MatrFrustum(-1, 1, -1, 1, 1, 100);

  MM3_RndPrg = MM3_RndShaderLoad("a");

} /* End of 'MM3_AnimInit' function */

/* Animation system initialization function.
 * ARGUMENTS:
 *   - Animation parameters:
 *       mm3ANIM *Ani;
 * RETURNS: None.
 */
VOID MM3_AnimAddUnit( mm3UNIT *Uni )
{
  if (MM3_Anim.NumOfUnits >= MM3_MAX_UNITS)
    return;
  MM3_Anim.Units[MM3_Anim.NumOfUnits++] = Uni;
  Uni->Init(Uni, &MM3_Anim);
} /* End of 'MM3_AnimAddUnit' function */

/* Animation system deinitialization function.
 * ARGUMENTS: None;
 * RETURNS: None.
 */
VOID MM3_AnimClose( VOID )
{
  /* Delete rendering context */
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(MM3_Anim.hGLRC);

  /* Delete GDI data */
  ReleaseDC(MM3_Anim.hWnd, MM3_Anim.hDC);
  /*int i;

  for (i = 0; i < MM3_Anim.NumOfUnits; i++)
  {
    MM3_Anim.UNITs[i]->Close(MM3_Anim.UNITs[i], &MM3_Anim);
    free(MM3_Anim.UNITs[i]);
  }
  MM3_Anim.NumOfUnits = 0;

  DeleteObject(MM3_Anim.hFrame);
  DeleteDC(MM3_Anim.hDC);*/
  MM3_RndShaderFree(MM3_RndPrg);
} /* End of 'MM3_AnimClose' function */

/* Animation system resize function.
 * ARGUMENTS:
 *   - width, height window size:
 *       INT W, H;
 * RETURNS: None.
 */
VOID MM3_AnimResize( INT w, INT h )
{
  HDC hDC;

  MM3_Anim.W = w;
  MM3_Anim.H = h;

  if (MM3_Anim.hFrame != NULL)
    DeleteObject(MM3_Anim.hFrame);

  glViewport(0, 0, w, h);

  hDC = GetDC(MM3_Anim.hWnd);
  MM3_Anim.hFrame = CreateCompatibleBitmap(hDC, MM3_Anim.W, MM3_Anim.H);
  ReleaseDC(MM3_Anim.hWnd, hDC);
  SelectObject(MM3_Anim.hDC, MM3_Anim.hFrame);

  MM3_RndSetProj();
} /* End of 'MM3_AnimResize' function */

/* Double buffer frame copy function.
 * ARGUMENTS:
 *   - window:
 *       HWND hWnd;
 * RETURNS: None.
 */
VOID MM3_AnimCopyFrame( HDC hDC )
{
  SwapBuffers(MM3_Anim.hDC);
  /*BitBlt(hDC, 0, 0, MM3_Anim.W, MM3_Anim.H, MM3_Anim.hDC, 0, 0, SRCCOPY);*/
} /* End of 'MM3_AnimCopyFrame' function */

/* Animation system render function.
 * ARGUMENTS: None;
 * RETURNS: None.
 */
VOID MM3_AnimRender( VOID )
{   
  int i;
  CHAR StrBuf[200];
  LARGE_INTEGER t;
  POINT pt;
  static VEC tr = {0, 0, 0};
  /*** Obtain input system state ***/

  /*** Handle timer ***/
  MM3_FrameCounter++;
  QueryPerformanceCounter(&t);
  /* Global time */
  MM3_Anim.GlobalTime = (DBL)(t.QuadPart - MM3_StartTime) / MM3_TimePerSec;
  MM3_Anim.GlobalDeltaTime = (DBL)(t.QuadPart - MM3_OldTime) / MM3_TimePerSec;
  /* Time with pause */
  if (MM3_Anim.IsPause)
  {
    MM3_Anim.DeltaTime = 0;
    MM3_PauseTime += t.QuadPart - MM3_OldTime;
  }
  else
  {
    MM3_Anim.Time = (DBL)(t.QuadPart - MM3_StartTime - MM3_PauseTime) / MM3_TimePerSec;
    MM3_Anim.DeltaTime = MM3_Anim.GlobalDeltaTime;
  }
  /* FPS */
  if (t.QuadPart - MM3_OldTimeFPS > MM3_TimePerSec)
  {
    CHAR str[100];

    MM3_Anim.FPS = MM3_FrameCounter * MM3_TimePerSec / (DBL)(t.QuadPart - MM3_OldTimeFPS);
    MM3_OldTimeFPS = t.QuadPart;
    MM3_FrameCounter = 0;
    sprintf(str, "FPS: %.5f", MM3_Anim.FPS);
    SetWindowText(MM3_Anim.hWnd, str);
  }
  MM3_OldTime = t.QuadPart;

  /* Mouse */
  GetCursorPos(&pt);
  ScreenToClient(MM3_Anim.hWnd, &pt);
  MM3_Anim.Mdx = pt.x - MM3_Anim.Mx;
  MM3_Anim.Mdy = pt.y - MM3_Anim.My;
  MM3_Anim.Mx = pt.x;
  MM3_Anim.My = pt.y;
  MM3_Anim.Mdz = MM3_MouseWheel;
  MM3_Anim.Mz += MM3_MouseWheel;
  MM3_MouseWheel = 0;

  /* Keyboard */
  GetKeyboardState(MM3_Anim.Keys);
  for (i = 0; i < 256; i++)
  {
    MM3_Anim.Keys[i] >>= 7;
    if (!MM3_Anim.KeysOld[i] && MM3_Anim.Keys[i])
      MM3_Anim.KeysClick[i] = TRUE;
    else
      MM3_Anim.KeysClick[i] = FALSE;
  }
  memcpy(MM3_Anim.KeysOld, MM3_Anim.Keys, 256);    

  /* Joystick * /
  if (joyGetNumDevs() > 0)
  {
    JOYCAPS jc;

    /* Get joystick info * /
    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(JOYCAPS))== JOYERR_NOERROR)
    {
      JOYINFOEX ji;
      ji.dwSize = sizeof(JOYINFOEX);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYS TICKID1, &ji) == JOYERR_NOERROR)
      {
        /* Buttons * /
        for (i = 0; i < 32; i++)
          MM3_Anim.JBut[i] = (ji.dwButtons >> i) & 1;
        /* Axes * /
        MM3_Anim.JX = (2.0 * (ji.dwXpos - jc.wXmin) / (jc.wXmax - jc.wXmin - 1) - 1);
        MM3_Anim.JX = MM3_GET_JOYSTICK_AXIS(X);
        MM3_Anim.JY = MM3_GET_JOYSTICK_AXIS(Y);
        MM3_Anim.JZ = MM3_GET_JOYSTICK_AXIS(Z);
        MM3_Anim.JR = MM3_GET_JOYSTICK_AXIS(R);
        /* Point of view * /
        MM3_Anim.JPov = ji.dwPOV == 0xFFFF ? 0 : ji.dwPOV / 4500 + 1;
      }
    }
  }*/
  
  tr = VecAddVec(tr, VecSet(MM3_Anim.JX, MM3_Anim.JY, MM3_Anim.JZ));
  /*MM3_MatrView = MatrixTranslate(MM3_Anim.JX, MM3_Anim.JY, MM3_Anim.JZ);*/

  /*** Send response to all UNITs ***/
  for (i = 0; i < MM3_Anim.NumOfUnits; i++)
    MM3_Anim.Units[i]->Response(MM3_Anim.Units[i], &MM3_Anim);

  /*** Clear frame ***/
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  /*hPen = SelectObject(MM3_Anim.hDC, GetStockObject(DC_PEN));
  hBr = SelectObject(MM3_Anim.hDC, GetStockObject(DC_BRUSH));
  SetDCPenColor(MM3_Anim.hDC, RGB(0, 150, 0));
  SetDCBrushColor(MM3_Anim.hDC, RGB(100, 150, 200));  

  Rectangle(MM3_Anim.hDC, 0, 0, MM3_Anim.W, MM3_Anim.H);*/
  /*
  SetDCPenColor(MM3_Anim.hDC, RGB(255, 255, 255));
  SetDCBrushColor(MM3_Anim.hDC, RGB(255, 255, 255));
  SelectObject(MM3_Anim.hDC, hPen);
  SelectObject(MM3_Anim.hDC, hBr);
  */
  /*** Render all UNIT ***/
  for (i = 0; i < MM3_Anim.NumOfUnits; i++)
  {
    MM3_RndMatrWorld = MatrIdentity();
    MM3_Anim.Units[i]->Render(MM3_Anim.Units[i], &MM3_Anim);
  }
  /*for (i = 0; i < MM3_Anim.NumOfUnits; i++)
  {
    SelectObject(MM3_Anim.hDC, GetStockObject(DC_PEN));
    SelectObject(MM3_Anim.hDC, GetStockObject(DC_BRUSH));
    SetDCPenColor(MM3_Anim.hDC, RGB(255, 255, 255));
    SetDCBrushColor(MM3_Anim.hDC, RGB(0, 0, 0));
    MM3_Anim.UNITs[i]->Render(MM3_Anim.UNITs[i], &MM3_Anim);
  }*/

  if (MM3_IsSysInfo)
  {
    SetBkMode(MM3_Anim.hDC, TRANSPARENT);
    TextOut(MM3_Anim.hDC, 0, 0, StrBuf, sprintf(StrBuf, "Input info: "
      "Mx: %6d; Mdx: %6d; My: %6d; Mdy: %6d; Mz: %6d; Mdz: %6d; "
      "Jx: %5.3lf; Jy: %5.3lf; Jz: %5.3lf; Jr: %5.3lf; JPov: %2d.", 
      MM3_Anim.Mx, MM3_Anim.Mdx, MM3_Anim.My, MM3_Anim.Mdy, MM3_Anim.Mz, MM3_Anim.Mdz,
      MM3_Anim.JX, MM3_Anim.JY, MM3_Anim.JZ, MM3_Anim.JR, MM3_Anim.JPov));
    SetBkMode(MM3_Anim.hDC, OPAQUE);
  }
  if (MM3_IsKeyInfo)
  {
    INT j;
    SetBkMode(MM3_Anim.hDC, TRANSPARENT);
    for (j = 0; j < 256; j++)
    {
      sprintf(StrBuf, "KeyNum: %d - %d;", j, MM3_Anim.Keys[j]);
      TextOut(MM3_Anim.hDC, 120 * (j / 20), 100 + (j * 10 + 10) % 200, StrBuf, strlen(StrBuf));
    }
    SetBkMode(MM3_Anim.hDC, OPAQUE);
  }

  glFinish();
} /* End of 'MM3_AnimRender' function */

/* Animation system exit function.
 * ARGUMENTS: None;
 * RETURNS: None.
 */
VOID MM3_DoExit( VOID )
{
  DestroyWindow(MM3_Anim.hWnd);
} /* End of 'MM3_AnimClose' function */

/* END OF 'ANIM.C' FILE 
VOID MM3_AnimCopyFrame( HDC hDC );
VOID MM3_AnimUnit( HWND hWnd );
VOID MM3_AnimAddUnit( mm3UNIT *Uni );
VOID MM3_AnimClose( VOID );
VOID IK3_Reasize( INT W, INT H );
VOID MM3_AnimRender( VOID );
VOID MM3_AnimDoExit( VOID );*/