/* FILENAME: RENDER.C
 * PROGRAMMER: MM3
 * DATE: 13.06.2016
 * PURPOSE: Animation system difinitions
*/

#include <stdio.h>
#include <string.h>

#include "anim.h"
#include "render.h"

/* Global transformation matrices */
MATR
  MM3_RndMatrWorld, /* World (object) transformation matrix */
  MM3_RndMatrView,  /* Camera view transform */
  MM3_RndMatrProj;  /* Projection transform */

/* Projection parameters */
DBL
  MM3_RndProjDist, /* Near clip plane */
  MM3_RndFarClip,  /* Far clip plane */
  MM3_RndProjSize; /* Project plane size */

/* Shader support */
UINT MM3_RndPrg;

/* Materials array */
mm3MTL MM3_RndMaterials[MM3_MAX_MATERIALS];
INT MM3_RndNumOfMaterials;

/* Setup projection function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID MM3_RndSetProj( VOID )
{
  DBL ratio_x = 1, ratio_y = 1;

  if (MM3_Anim.W >= MM3_Anim.H)
    ratio_x = (DBL)MM3_Anim.W / MM3_Anim.H;
  else
    ratio_y = (DBL)MM3_Anim.H / MM3_Anim.W;

  MM3_RndMatrProj = MatrFrustum(-ratio_x * MM3_RndProjSize / 2,
                                 ratio_x * MM3_RndProjSize / 2,
                                -ratio_y * MM3_RndProjSize / 2,
                                 ratio_y * MM3_RndProjSize / 2,
                                 MM3_RndProjDist, MM3_RndFarClip);
} /* End of 'VG4_RndSetProj' function */

/* Object draw function.
 * ARGUMENTS:
 *   - object structure pointer:
 *       vg4OBJ *Obj;
 * RETURNS: None.
 */
VOID MM3_RndObjDraw( mm3OBJ *Obj )
{
  INT i;
  INT loc, mtl;
  MATR M, MSave;

  for (i = 0; i < Obj->NumOfPrims; i++)
  {
    /* Build transform matrix */
    MSave = MM3_RndMatrWorld;
    MM3_RndMatrWorld = MatrMulMatr(MM3_RndMatrWorld, Obj->Prims[i].M);
    M = MatrMulMatr(MM3_RndMatrWorld,
      MatrMulMatr(MM3_RndMatrView, MM3_RndMatrProj));
    glLoadMatrixf(M.A[0]);
    /*
    glBegin(GL_LINES);
      glColor3d(1, 0, 0);
      glVertex3d(0, 0, 0);
      glVertex4d(1, 0, 0, 0);
      glColor3d(0, 1, 0);
      glVertex3d(0, 0, 0);
      glVertex4d(0, 1, 0, 0);
      glColor3d(0, 0, 1);
      glVertex3d(0, 0, 0);
      glVertex4d(0, 0, 1, 0);
    glEnd();
    */

    glUseProgram(MM3_RndPrg);

    mtl = Obj->Prims[i].MtlNo;
    if (mtl != -1)
    {
      if (MM3_RndMaterials[mtl].TexNo != 0)
      {
        glBindTexture(GL_TEXTURE_2D, MM3_RndMaterials[mtl].TexNo);
        if ((loc = glGetUniformLocation(MM3_RndPrg, "IsTexture")) != -1)
          glUniform1i(loc, 1);
      }
      else
      {
        if ((loc = glGetUniformLocation(MM3_RndPrg, "IsTexture")) != -1)
          glUniform1i(loc, 0);
      }
      if ((loc = glGetUniformLocation(MM3_RndPrg, "Ka")) != -1)
        glUniform3fv(loc, 1, &MM3_RndMaterials[mtl].Ka.X);
      if ((loc = glGetUniformLocation(MM3_RndPrg, "Kd")) != -1)
        glUniform3fv(loc, 1, &MM3_RndMaterials[mtl].Kd.X);
      if ((loc = glGetUniformLocation(MM3_RndPrg, "Ks")) != -1)
        glUniform3fv(loc, 1, &MM3_RndMaterials[mtl].Ks.X);
      if ((loc = glGetUniformLocation(MM3_RndPrg, "Ph")) != -1)
        glUniform1f(loc, MM3_RndMaterials[mtl].Ph);
      if ((loc = glGetUniformLocation(MM3_RndPrg, "Trans")) != -1)
        glUniform1f(loc, MM3_RndMaterials[mtl].Trans);
    }

    /* Setup global variables */                                                 
    if ((loc = glGetUniformLocation(MM3_RndPrg, "MatrWVP")) != -1)
      glUniformMatrix4fv(loc, 1, FALSE, M.A[0]);
    if ((loc = glGetUniformLocation(MM3_RndPrg, "MatrWorld")) != -1)
      glUniformMatrix4fv(loc, 1, FALSE, MM3_RndMatrWorld.A[0]);
    if ((loc = glGetUniformLocation(MM3_RndPrg, "MatrView")) != -1)
      glUniformMatrix4fv(loc, 1, FALSE, MM3_RndMatrView.A[0]);
    if ((loc = glGetUniformLocation(MM3_RndPrg, "MatrProj")) != -1)
      glUniformMatrix4fv(loc, 1, FALSE, MM3_RndMatrProj.A[0]);
    if ((loc = glGetUniformLocation(MM3_RndPrg, "Time")) != -1)
      glUniform1f(loc, MM3_Anim.Time);
    if ((loc = glGetUniformLocation(MM3_RndPrg, "PartNo")) != -1)
      glUniform1i(loc, i);

    /* Activete primitive vertex array */
    glBindVertexArray(Obj->Prims[i].VA);
    /* Activete primitive index buffer */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Obj->Prims[i].IBuf);
    /* Draw primitive */
    glDrawElements(GL_TRIANGLES, Obj->Prims[i].NumOfI, GL_UNSIGNED_INT, NULL);
    glUseProgram(0);
    MM3_RndMatrWorld = MSave;
  }
} /* End of 'VG4_RndObjDraw' function */

/* Object free function.
 * ARGUMENTS:
 *   - object structure pointer:
 *       vg4OBJ *Obj;
 * RETURNS: None.
 */
VOID MM3_RndObjFree( mm3OBJ *Obj )
{
  INT i;

  for (i = 0; i < Obj->NumOfPrims; i++)
  {
    glBindVertexArray(Obj->Prims[i].VA);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &Obj->Prims[i].VBuf);
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &Obj->Prims[i].VA);
    glDeleteBuffers(1, &Obj->Prims[i].IBuf);
  }
  free(Obj->Prims);
  memset(Obj, 0, sizeof(mm3OBJ));
} /* End of 'VG4_RndObjFree' function */

/* Material find by name function.
 * ARGUMENTS:
 *   - material name:
 *       CHAR *Name;
 * RETURNS:
 *   (INT) number of found material or -1 if no result.
 */
INT MM3_RndFindMaterial( CHAR *Name )
{
  INT i;

  for (i = 0; i < MM3_RndNumOfMaterials; i++)
    if (strcmp(Name, MM3_RndMaterials[i].Name) == 0)  /* #include <string.h> */
      return i;
  return -1;
} /* End of 'VG4_RndFindMaterial' function */

/* Material load function.
 * ARGUMENTS:
 *   - material file name:
 *       CHAR *FileName;
 * RETURNS: None.
*/
VOID MM3_RndLoadMaterials( CHAR *FileName )
{
  INT i, NumOfMaterials;
  UINT t;
  DWORD Sign;
  FILE *F;
  mm3MTL M;
  BYTE *Image;

  if ((F = fopen(FileName, "rb")) == NULL)
    return;
  /* Read and check file signature */
  fread(&Sign, 4, 1, F);
  if (Sign != *(DWORD *)"GMT")
  {
    fclose(F);
    return;
  }

  /* Read all materials */
  fread(&NumOfMaterials, 4, 1, F);
  for (i = 0; i < NumOfMaterials; i++)
  {
    if (MM3_RndNumOfMaterials >= MM3_MAX_MATERIALS)
      break;

    /* Read illumination coefficients and texture parameters */
    fread(&M, sizeof(mm3MTL), 1, F);
    
    /* Read image */
    if (M.TexW != 0 && M.TexH != 0 && M.TexNo != 0)
    {
      /* Allocate memory for texture image */
      if ((Image = malloc(M.TexW * M.TexH * M.TexNo)) == NULL)
      {
        fclose(F);
        return;
      }
      fread(Image, M.TexNo, M.TexW * M.TexH, F);
      /* Flip image */
      /*
      for (y = 0; y < M.TexH / 2; y++)
        for (x = 0; x < M.TexW * M.TexNo; x++)
        {
          INT
            first = y * M.TexW * M.TexNo + x,
            last = (M.TexH - 1 - y) * M.TexW * M.TexNo + x;
          BYTE tmp = Image[first];
          Image[first] = Image[last];
          Image[last] = tmp;
        }
      */
      glGenTextures(1, &t);
      glBindTexture(GL_TEXTURE_2D, t);
      gluBuild2DMipmaps(GL_TEXTURE_2D, M.TexNo, M.TexW, M.TexH,
        M.TexNo == 3 ? GL_BGR_EXT : GL_BGRA, GL_UNSIGNED_BYTE, Image);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glBindTexture(GL_TEXTURE_2D, 0);
      M.TexNo = t;
    }
    /* Add material to animation and OpenGL */
    MM3_RndMaterials[MM3_RndNumOfMaterials++] = M;
  }
  fclose(F);
} /* End of 'VG4_RndLoadMaterials' function */

/* END OF 'RENDER.C' FILE */
