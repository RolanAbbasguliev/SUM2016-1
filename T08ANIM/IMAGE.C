/* FILE NAME: IMAGE.C
 * PROGRAMMER: MM3
 * DATE: 13.06.2016
 * PURPOSE: Image handle functions.
 */

#include "image.h"

/* Load image function.
 * ARGUMENTS:
 *   - image store data pointer:
 *       mm3IMG *Im;
 *   - image file name (*.BMP):
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL MM3_ImageLoad( mm3IMG *Im, CHAR *FileName )
{
  HDC hDC, hMemDC, hMemDC1;
  HBITMAP hBm;
  BITMAP bm;
  BITMAPINFOHEADER bmih;

  memset(Im, 0, sizeof(mm3IMG));

  /* Load image from file */
  hBm = LoadImage(NULL, FileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBm == NULL)
    return FALSE;

  /* Create contextes */
  hDC = GetDC(NULL);
  hMemDC = CreateCompatibleDC(hDC);
  hMemDC1 = CreateCompatibleDC(hDC);
  GetObject(hBm, sizeof(bm), &bm);
  Im->W = bm.bmWidth;
  Im->H = bm.bmHeight;
  SelectObject(hMemDC, hBm);
  ReleaseDC(NULL, hDC);

  /* Build DIB section */
  memset(&bmih, 0, sizeof(bmih));
  bmih.biSize = sizeof(BITMAPINFOHEADER);
  bmih.biBitCount = 32;                   /* BGRx - DWORD */
  bmih.biPlanes = 1;                      /* always */
  bmih.biCompression = BI_RGB;            /* 0 - no compression */
  bmih.biWidth = bm.bmWidth;              /* width */
  bmih.biHeight = -bm.bmHeight;           /* height + first coded row is 0 */
  bmih.biSizeImage = bm.bmWidth * bm.bmHeight * 4;  /* image size in bytes */

  Im->hImage = CreateDIBSection(NULL, (BITMAPINFO *)&bmih, DIB_RGB_COLORS,
                 (VOID **)&Im->Bits, NULL, 0);
  SelectObject(hMemDC1, Im->hImage);

  /* Draw load image to DIB */
  BitBlt(hMemDC1, 0, 0, Im->W, Im->H, hMemDC, 0, 0, SRCCOPY);

  DeleteDC(hMemDC);
  DeleteDC(hMemDC1);
  return TRUE;
} /* End of 'MM3_ImageLoad' function */

/* Free image function.
 * ARGUMENTS:
 *   - image store data pointer:
 *       mm3IMG *Im;
 * RETURNS: None.
 */
VOID MM3_ImageFree( mm3IMG *Im )
{
  if (Im->hImage != NULL)
    DeleteObject(Im->hImage);
  memset(Im, 0, sizeof(mm3IMG));
} /* End of 'MM3_ImageFree' function */

/* Get image pixel color function.
 * ARGUMENTS:
 *   - image store data pointer:
 *       mm3IMG *Im;
 *   - pixel coordinates:
 *       INT X, Y;
 * RETURNS:
 *   (DWORD) pixel color.
 */
DWORD MM3_ImageGetP( mm3IMG *Im, INT X, INT Y )
{
  if (Im->hImage != NULL && X >= 0 && X < Im->W && Y >= 0 && Y < Im->H)
    return Im->Bits[Y * Im->W + X];
  return 0;
} /* End of 'MM3_ImageGetP' function */

/* END OF 'IMAGE.C' FILE */

