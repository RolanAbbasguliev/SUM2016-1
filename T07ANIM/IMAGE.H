/* FILE NAME: IMAGE.C
 * PROGRAMMER: MM3
 * DATE: 13.06.2016
 * PURPOSE: Image handle functions.
 */

#ifndef __IMAGE_H_
#define __IMAGE_H_

#include "def.h"

/* Image represetation type */
typedef struct
{
  HBITMAP hImage; /* Image bitmap handle */
  INT W, H;       /* Image size */
  DWORD *Bits;    /* Image pixel color data */
} mm3IMG;

/* Load image function.
 * ARGUMENTS:
 *   - image store data pointer:
 *       mm3IMG *Im;
 *   - image file name (*.BMP):
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL MM3_ImageLoad( mm3IMG *Im, CHAR *FileName );

/* Free image function.
 * ARGUMENTS:
 *   - image store data pointer:
 *       mm3IMG *Im;
 * RETURNS: None.
 */
VOID MM3_ImageFree( mm3IMG *Im );

/* Get image pixel color function.
 * ARGUMENTS:
 *   - image store data pointer:
 *       mm3IMG *Im;
 *   - pixel coordinates:
 *       INT X, Y;
 * RETURNS:
 *   (DWORD) pixel color.
 */
DWORD MM3_ImageGetP( mm3IMG *Im, INT X, INT Y );


#endif /* __IMAGE_H_ */

/* END OF 'IMAGE.H' FILE */
