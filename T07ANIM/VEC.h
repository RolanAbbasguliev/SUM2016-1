/* FILE NAME: T07ANIM.C
 * PROGRAMMER: MM3
 * DATE: 10.06.2016
 */

#ifndef __VEC_H_
#define __VEC_H_

#include <math.h>

#define PI 3.14159265358979323846
#define D2R(A) ((A) * PI / 180.0)


typedef DOUBLE DBL;
typedef FLOAT FLT;

typedef struct tagVEC
{
  FLT X, Y, Z;
} VEC;

typedef struct tagVEC2
{
  FLT X, Y;
} VEC2;

typedef struct tagMATR
{
  FLT A[4][4];
} MATR;

/* Start of 'VecSet' function */
__inline VEC VecSet( FLT X, FLT Y, FLT Z )
{
  VEC r = {X, Y, Z};

  return r;
}/* The end of 'VecSet' function */

/* Start of 'VecAddVec' function */
__inline VEC VecAddVec( VEC A, VEC B )
{
  return VecSet(A.X + B.X, A.Y + B.Y, A.Z + B.Z);
}/* The end of 'VecAddVec' function */

__inline VEC VecSubVec( VEC A, VEC B )
{
  return VecSet(A.X - B.X, A.Y - B.Y, A.Z - B.Z);
}
__inline VEC VecMulNum( VEC A, FLT N )
{
  return VecSet(A.X * N, A.Y * N, A.Z * N);
}
__inline VEC VecDivNum( VEC A, FLT N )
{
  return VecSet(A.X / N, A.Y / N, A.Z / N);
}
/* Start of 'VecNeg' function */
__inline VEC VecNeg( VEC A )
{
  return VecSet(-A.X, -A.Y, -A.Z);
}/* The end of 'VecNeg' function */

/* Start of 'VecDotVec' function */
__inline FLT VecDotVec( VEC A, VEC B )
{
  return A.X * B.X + A.Y * B.Y + A.Z * B.Z;
} /* End of 'VecDotVec' function */
                                                                                                                  
__inline VEC VecCrossVec( VEC A, VEC B )
{                                       
  return VecSet(A.Y * B.Z - A.Z * B.Y, A.Y * B.X - A.X* B.Z, A.X * B.Z - A.Z * B.X);
}
__inline FLT VecLen2( VEC V )
{
  return V.X * V.X + V.Y * V.Y + V.Z * V.Z;
}
__inline FLT VecLen( VEC V )
{
  return sqrt(V.X * V.X  + V.Y * V.Y + V.Z * V.Z);
}
__inline VEC VecNormalize( VEC V )
{
  FLT len = VecDotVec(V, V);

  if (len != 1 && len != 0)
    len = sqrt(len), V.X /= len, V.Y /= len, V.Z /= len;
  return V;
}
/* Start of 'PointTransform' function */
__inline VEC PointTransform( VEC V, MATR M )
{
  return VecSet(
    V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0],
    V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1],
    V.X * M.A[0][2] + V.Y * M.A[
      
      1][2] + V.Z * M.A[2][2] + M.A[3][2]);
}/* The end of 'PointTransform' function */

/* Start of 'VectorTransform' function */
__inline VEC VectorTransform( VEC V, MATR M )
{
  return VecSet(
    V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0],
    V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1],
    V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2]);
}/* The end of 'VectorTransform' function */


/* Start of 'PointTransform4' function */
__inline VEC PointTransform4( VEC V, MATR M )
{        
  FLT w = V.X * M.A[0][3] + V.Y * M.A[1][3] + V.Z * M.A[2][3] + M.A[3][3];

  return VecSet(
    (V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0]) / w,
    (V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1]) / w,
    (V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]) / w);
}/* The end of 'PointTransform4' function */

/* Start of 'MatrTranslate' function */
__inline MATR MatrTranslate( VEC D )
{
  MATR M =
  {
    {
      {  1,   0,   0,  0},
      {  0,   1,   0,  0},
      {  0,   0,   1,  0},
      {D.X, D.Y, D.Z,  1},
    }
  };

  return M;
} /* The end of 'MatrTranslate' function */

/* Start of 'MatrRotateX' function */
__inline MATR MatrScale( VEC S )
{
  MATR M =
  {
    {
      {  S.X,     0,     0,  0},
      {    0,   S.Y,     0,  0},
      {    0,     0,   S.Z,  0},
      {    0,     0,     0,  1},
    }
  };
  return M;
}

__inline MATR MatrFrustum( FLT Left, FLT Right, FLT Bottom, FLT Top, FLT Near, FLT Far )
{
  MATR m =
  {
    {
      {      2 * Near / (Right - Left),                               0,                              0,  0},
      {                              0,       2 * Near / (Top - Bottom),                              0,  0},
      {(Right + Left) / (Right - Left), (Top + Bottom) / (Top - Bottom),   -(Far + Near) / (Far - Near), -1},
      {                              0,                               0, -2 * Near * Far / (Far - Near),  0}
    }
  };

  return m;
} 
__inline MATR MatrView( VEC Loc, VEC At, VEC Up1 )
{
  VEC
    Dir = VecNormalize(VecSubVec(At, Loc)),
    Right = VecNormalize(VecCrossVec(Dir, Up1)),
    Up = VecNormalize(VecCrossVec(Right, Dir));
  MATR m =
  {
    {
      {               Right.X,                Up.X,              -Dir.X, 0},
      {               Right.Y,                Up.Y,              -Dir.Y, 0},
      {               Right.Z,                Up.Z,              -Dir.Z, 0},
      {-VecDotVec(Loc, Right), -VecDotVec(Loc, Up), VecDotVec(Loc, Dir), 1}
    }
  };
  return m;
}
/* Start of 'MatrRotateX' function */
__inline MATR MatrRotateX( FLT AngleDegree )
{        
  FLT A = D2R(AngleDegree);
  MATR M =
  {
    {
      {  1,       0,       0,   0},
      {  0,  cos(A),   sin(A),  0},
      {  0, -sin(A),   cos(A),  0},
      {  0,       0,       0,   1},
    }
  };
  return M;
} /* The end of 'MatrRotateX' function */
/* Start of 'MatrRotateY' function */
__inline MATR MatrRotateY( FLT AngleDegree )
{
  FLT A = D2R(AngleDegree);
  MATR M =
  {
    {
      {  cos(A), 0, -sin(A), 0},
      {       0, 1,       0, 0},
      {  sin(A), 0,  cos(A), 0},
      {       0, 0,       0, 1},
    }
  };
  return M;
} /* The end of 'MatrRotateY' function */
/* Start of 'MatrRotateZ' function */
__inline MATR MatrRotateZ( FLT AngleDegree )
{        
  FLT A = D2R(AngleDegree);
  MATR M =
  {
    {
      {  cos(A), 0, sin(A), 0},
      { -sin(A), 1, cos(A), 0},
      {       0, 0,      0, 0},
      {       0, 0,      0, 1},
    }
  };
  return M;
} /* The end of 'MatrRotateZ' function */
/* Start of 'MatrRotate' function */
__inline MATR MatrRotate( FLT AngleDegree, VEC R )
{
  FLT A = D2R(AngleDegree), si = sin(A), co = cos(A);
  VEC V = VecNormalize(R);
  MATR M =
  {
    {
      {co + V.X * V.X * (1 - co),        V.X * V.Y * (1 - co) - V.Z * si,  V.X * V.Z * (1 - co) + V.Y * si, 0},
      {V.Y * V.X * (1 - co) + V.Z * si,  co + V.Y * V.Y * (1 - co),        V.Y * V.Z * (1 - co) - V.X * si, 0},
      {V.Z * V.X * (1 - co) - V.Y * si,  V.Z * V.Y * (1 - co) + V.X * si,  co + V.Z * V.Z * (1 - co),       0},
      {0,                                0,                                0,                               1}
    }
  };

  return M;
}/* The end of 'MatrRotate' function */

/* Start of 'MatrIdentity' function */
__inline MATR MatrIdentity( VOID )
{
  MATR M =
  {
    {
      {1, 0, 0, 0},
      {0, 1, 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1}
    }
  };

  return M;
}/* The end of 'MatrIdentity' function */

/* Start of 'MatrMulMatr' function */
__inline MATR MatrMulMatr( MATR M1, MATR M2 )
{
  MATR r;
  int i, j, k;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      for (k = 0, r.A[i][j] = 0; k < 4; k++)
        r.A[i][j] += M1.A[i][k] * M2.A[k][j];
  return r;
}/* The end of 'MatrMulMatr' function */
__inline FLT Rnd0( VOID )
{
  return (FLT)rand() / RAND_MAX;
} /* End of 'Rnd0' function */

/* Random number obtain in range [-1.0 .. 1.0] function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (FLT) result random number.
 */
__inline FLT Rnd1( VOID )
{
  return 2.0 * rand() / RAND_MAX - 1;
} /* End of 'Rnd1' function */

/* Start of 'MatrTranspose' function */
__inline MATR MatrTransponse( MATR M)
{
  INT i, j;
  MATR r;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      r.A[i][j] = M.A[i][j];
  
  return r;
}/* The end of 'MatrTranspose' function */
/* Start of 'MatrDeterm3x3' function */
__inline FLT MatrDeterm3x3( FLT A11, FLT A12, FLT A13,
                   FLT A21, FLT A22, FLT A23,
                   FLT A31, FLT A32, FLT A33 )
{
  return A11 * A22 * A33 + A12 * A23 * A31 + A13 * A21 * A32 -
         A11 * A23 * A32 - A12 * A21 * A33 - A13 * A22 * A31;
}/* The end of 'MatrTranspose' function */

/* Start of 'MatrDeterm' function */
__inline FLT MatrDeterm( MATR M )
{        
  return
    M.A[0][0] * MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                              M.A[2][1], M.A[2][2], M.A[2][3],
                              M.A[3][1], M.A[3][2], M.A[3][3]) -
    M.A[0][1] * MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                              M.A[2][0], M.A[2][2], M.A[2][3],
                              M.A[3][0], M.A[3][2], M.A[3][3]) +
    M.A[0][2] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                              M.A[2][0], M.A[2][1], M.A[2][3],
                              M.A[3][0], M.A[3][1], M.A[3][3]) -
    M.A[0][3] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                              M.A[2][0], M.A[2][1], M.A[2][2],
                              M.A[3][0], M.A[3][1], M.A[3][2]);
}/* The end of 'MatrDeterm' function */

/* Start of 'MatrInverse' function */
__inline MATR MatrInverse( MATR M )
{
  FLT det = MatrDeterm(M);
  MATR r;

  if (det == 0)
    return M;
  r.A[0][0] = MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                            M.A[2][1], M.A[2][2], M.A[2][3],
                            M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][0] = -MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                             M.A[2][0], M.A[2][2], M.A[2][3],
                             M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][0] = MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                            M.A[2][0], M.A[2][1], M.A[2][3],
                            M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[3][0] = -MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                             M.A[2][0], M.A[2][1], M.A[2][2],
                             M.A[3][0], M.A[3][1], M.A[3][2]) / det;

  r.A[0][1] = -MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                             M.A[2][1], M.A[2][2], M.A[2][3],
                             M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][1] = MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                            M.A[2][0], M.A[2][2], M.A[2][3],
                            M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][1] = -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                             M.A[2][0], M.A[2][1], M.A[2][3],
                             M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[3][1] = MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                            M.A[2][0], M.A[2][1], M.A[2][2],
                            M.A[3][0], M.A[3][1], M.A[3][2]) / det;

  r.A[0][2] = MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                            M.A[1][1], M.A[1][2], M.A[1][3],
                            M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][2] = -MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                             M.A[1][0], M.A[1][2], M.A[1][3],
                             M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][2] = MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                            M.A[1][0], M.A[1][1], M.A[1][3],
                            M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[3][2] = -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                             M.A[1][0], M.A[1][1], M.A[1][2],
                             M.A[3][0], M.A[3][1], M.A[3][2]) / det;

  r.A[0][2] = MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                            M.A[1][1], M.A[1][2], M.A[1][3],
                            M.A[2][1], M.A[2][2], M.A[2][3]) / det;
  r.A[1][2] = -MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                             M.A[1][0], M.A[1][2], M.A[1][3],
                             M.A[2][0], M.A[2][2], M.A[2][3]) / det;
  r.A[2][2] = MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                            M.A[1][0], M.A[1][1], M.A[1][3],
                            M.A[2][0], M.A[2][1], M.A[2][3]) / det;
  r.A[3][2] = -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                             M.A[1][0], M.A[1][1], M.A[1][2],
                             M.A[2][0], M.A[2][1], M.A[2][2]) / det;

}/* The end of 'MatrInverse' function */
#endif

/* The end of VEC.H file */
/*
#ifndef __VEC_H_
#define __VEC_H_

#include <math.h>

#define PI 3.14159265358979323846
#define D2R(A) ((A) * PI / 180.0)

/* ������� ������������ ���� * /
typedef DOUBLE FLT;
typedef FLOAT FLT;;

/* ��� ��� ������� � ����������� * /
typedef struct tagVEC
{
  FLT X, Y, Z;
} VEC;

typedef struct tagVEC2
{
  FLT X, Y;
} VEC2;

/* ��� ��� ������� - ������ � ��������� * /
typedef struct tagMATR
{
  FLT A[4][4];
} MATR;

/* Functions: * /

__inline FLT Rnd0( VOID )
{
  return (FLT)rand() / RAND_MAX;
} /* End of 'Rnd0' function * /

__inline FLT Rnd1( VOID )
{
  return 2.0 * rand() / RAND_MAX - 1;
} /* End of 'Rnd1' function * /

__inline VEC VecSet( FLT X, FLT Y, FLT Z )
{
  VEC r = {X, Y, Z};

  return r;
} /* End of 'VecSet' func * /

__inline VEC VecAddVec( VEC A, VEC B )
{
  return VecSet(A.X + B.X, A.Y + B.Y, A.Z + B.Z);
} /* End of 'VecAddVec' func * /

__inline VEC VecSubVec( VEC A, VEC B )
{
  return VecSet(A.X - B.X, A.Y - B.Y, A.Z - B.Z);
} /* End of 'VecSubVec' func * /

__inline VEC VecMulNum( VEC A, FLT N )
{
  return VecSet(A.X * N, A.Y * N, A.Z * N);
} /* End of 'VecMulNum' func * / 

__inline VEC VecDivNum( VEC A, FLT N )
{
  return VecSet(A.X / N, A.Y / N, A.Z / N);
} /* End of 'VecDivNum' func * /

__inline VEC VecNeg( VEC A )
{
  return VecSet(-A.X, -A.Y, -A.Z);
} /* End of 'VecNeg' func * /

__inline FLT VecDotVec( VEC A, VEC B )
{
  return A.X * B.X + A.Y * B.Y + A.Z * B.Z;
} /* End of 'VecDotVec' func * /

__inline VEC VecCrossVec( VEC A, VEC B )
{
  return VecSet(A.Y * B.Z - A.Z * B.Y, A.Y * B.X - A.X * B.Y, A.X * B.Z - A.Z * B.X);
} /* End of 'VecCrossVec' func * /

__inline FLT VecLen2( VEC V )
{
  return V.X * V.X + V.Y * V.Y + V.Z * V.Z;
} /* End of 'VecLen2' func * /

__inline FLT VecLen( VEC V )
{
  return sqrt(V.X * V.X + V.Y * V.Y + V.Z * V.Z);
} /* End of 'VecLen' func * /

__inline VEC VecNormalize( VEC V )
{
  FLT len = VecDotVec(V, V);

  if (len != 1 && len != 0)
    len = sqrt(len), V.X /= len, V.Y /= len, V.Z /= len;
  return V;
} /* End of '' func * /

__inline VEC PointTransform( VEC V, MATR M )
{
  return VecSet(
    V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0],
    V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1],
    V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]);
} /* End of 'PointTransform' func * /

__inline VEC VectorTransform( VEC V, MATR M )
{
  return VecSet(
    V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0],
    V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1],
    V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2]);
} /* End of 'VectorTransform' func * /

__inline VEC PointTransform4( VEC V, MATR M )
{
  FLT w = V.X * M.A[0][3] + V.Y * M.A[1][3] + V.Z * M.A[2][3] + M.A[3][3];

  return VecSet(
    (V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0]) / w,
    (V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1]) / w,
    (V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]) / w);
} /* End of 'PointTransform4' func * /

__inline MATR MatrTranslate( VEC D )
{
  MATR M =
  {
    {
      {  1,   0,   0,  0},
      {  0,   1,   0,  0},
      {  0,   0,   1,  0},
      {D.X, D.Y, D.Z,  1},
    }
  };

  return M;
} /* End of 'MatrTranslate' func * /

__inline MATR MatrScale( VEC S )
{
  MATR M =
  {
    {
      {  S.X,  0,    0,  0},
      {  0,  S.Y,    0,  0},
      {  0,    0,  S.Z,  0},
      {  0,    0,    0,  1},
    }
  };

  return M;
} /* End of 'MatrScale' func * / 

__inline MATR MatrRotateX( FLT AngleDegree )
{
  FLT Angle = D2R(AngleDegree), si = sin(Angle), co = cos(Angle);
  MATR M =
  {
    {
      {  1,    0,   0,  0},
      {  0,   co,  si,  0},
      {  0,  -si,  co,  0},
      {  0,    0,   0,  1},
    }
  };

  return M;
} /* End of 'MatrRotateX' func * /

__inline MATR MatrRotateY( FLT AngleDegree )
{
  FLT Angle = D2R(AngleDegree), si = sin(Angle), co = cos(Angle);
  MATR M =
  {
    {
      {  co,  0,  -si,  0},
      {   0,  1,    0,  0},
      {  si,  0,   co,  0},
      {   0,  0,    0,  1},
    }
  };

  return M;
} /* End of 'MatrRotateY' func * /

__inline MATR MatrRotateZ( FLT AngleDegree )
{
  FLT Angle = D2R(AngleDegree), si = sin(Angle), co = cos(Angle);
  MATR M =
  {
    {
      {   co,  si,  0,  0},
      {  -si,  co,  0,  0},
      {    0,   0,  1,  0},
      {    0,   0,  0,  1},
    }
  };

  return M;
} /* End of 'MatrRotateZ' func * /

__inline MATR MatrRotate( FLT AngleDegree, VEC R )
{
  FLT A = D2R(AngleDegree), si = sin(A), co = cos(A);
  VEC V = VecNormalize(R);
  MATR M =
  {
    {
      {        co + V.X * V.X * (1 - co),  V.X * V.Y * (1 - co) - V.Z * si,  V.X * V.Z * (1 - co) + V.Y * si,  0},
      {  V.Y * V.X * (1 - co) + V.Z * si,        co + V.Y * V.Y * (1 - co),  V.Y * V.Z * (1 - co) - V.X * si,  0},
      {  V.Z * V.X * (1 - co) - V.Y * si,  V.Z * V.Y * (1 - co) + V.X * si,        co + V.Z * V.Z * (1 - co),  0},
      {                                0,                                0,                                0,  1}
    }
  };

  return M;
} /* End of 'MatrRotate' func * / 

__inline MATR MatrIdentity( VOID )
{
  MATR M =
  {
    {
      {1, 0, 0, 0},
      {0, 1, 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1}
    }
  };

  return M;
} /* End of 'MatrIdentity' func * /

__inline MATR MatrMulMatr( MATR M1, MATR M2 )
{
  INT i, j, k;
  MATR r;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      for (k = 0, r.A[i][j] = 0; k < 4; k++)
        r.A[i][j] += M1.A[i][k] * M2.A[k][j];
  return r;
} /* End of 'MatrMulMatr' func * /

__inline MATR MatrTranspose( MATR M )
{
  INT i, j;
  MATR r;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      r.A[i][j] = M.A[j][i];

  return r;
} /* End of 'MatrTranspose' func * /

__inline FLT MatrDeterm3x3( FLT A11, FLT A12, FLT A13,
                   FLT A21, FLT A22, FLT A23,
                   FLT A31, FLT A32, FLT A33 )
{
  return A11 * A22 * A33 + A12 * A23 * A31 + A13 * A21 * A32 -
         A11 * A23 * A32 - A12 * A21 * A33 - A13 * A22 * A31;
} /* End of 'MatrDeterm3x3' func * /

__inline FLT MatrDeterm( MATR M )
{
  return
    M.A[0][0] * MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                              M.A[2][1], M.A[2][2], M.A[2][3],
                              M.A[3][1], M.A[3][2], M.A[3][3]) -
    M.A[0][1] * MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                              M.A[2][0], M.A[2][2], M.A[2][3],
                              M.A[3][0], M.A[3][2], M.A[3][3]) +
    M.A[0][2] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                              M.A[2][0], M.A[2][1], M.A[2][3],
                              M.A[3][0], M.A[3][1], M.A[3][3]) -
    M.A[0][3] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                              M.A[2][0], M.A[2][1], M.A[2][2],
                              M.A[3][0], M.A[3][1], M.A[3][2]);
} /* End of 'MatrDeterm' func * /

__inline MATR MatrInverse( MATR M )
{
  FLT det = MatrDeterm(M);
  MATR r;

  if (det == 0)
    return M;
  r.A[0][0] = MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                            M.A[2][1], M.A[2][2], M.A[2][3],
                            M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][0] = -MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                             M.A[2][0], M.A[2][2], M.A[2][3],
                             M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][0] = MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                            M.A[2][0], M.A[2][1], M.A[2][3],
                            M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[3][0] = -MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                             M.A[2][0], M.A[2][1], M.A[2][2],
                             M.A[3][0], M.A[3][1], M.A[3][2]) / det;

  r.A[0][1] = -MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                             M.A[2][1], M.A[2][2], M.A[2][3],
                             M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][1] = MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                            M.A[2][0], M.A[2][2], M.A[2][3],
                            M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][1] = -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                             M.A[2][0], M.A[2][1], M.A[2][3],
                             M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[3][1] = MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                            M.A[2][0], M.A[2][1], M.A[2][2],
                            M.A[3][0], M.A[3][1], M.A[3][2]) / det;

  r.A[0][2] = MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                            M.A[1][1], M.A[1][2], M.A[1][3],
                            M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][2] = -MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                             M.A[1][0], M.A[1][2], M.A[1][3],
                             M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][2] = MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                            M.A[1][0], M.A[1][1], M.A[1][3],
                            M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[3][2] = -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                             M.A[1][0], M.A[1][1], M.A[1][2],
                             M.A[3][0], M.A[3][1], M.A[3][2]) / det;

  r.A[0][2] = MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                            M.A[1][1], M.A[1][2], M.A[1][3],
                            M.A[2][1], M.A[2][2], M.A[2][3]) / det;
  r.A[1][2] = -MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                             M.A[1][0], M.A[1][2], M.A[1][3],
                             M.A[2][0], M.A[2][2], M.A[2][3]) / det;
  r.A[2][2] = MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                            M.A[1][0], M.A[1][1], M.A[1][3],
                            M.A[2][0], M.A[2][1], M.A[2][3]) / det;
  r.A[3][2] = -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                             M.A[1][0], M.A[1][1], M.A[1][2],
                             M.A[2][0], M.A[2][1], M.A[2][2]) / det;

} /* End of 'MatrInverse' func * /

__inline MATR MatrView( VEC Loc, VEC At, VEC Up1 )
{
  VEC
    Dir = VecNormalize(VecSubVec(At, Loc)),
    Right = VecNormalize(VecCrossVec(Dir, Up1)),
    Up = VecNormalize(VecCrossVec(Right, Dir));
  MATR m =
  {
    {
      {                 Right.X,                 Up.X,               -Dir.X,  0},
      {                 Right.Y,                 Up.Y,               -Dir.Y,  0},
      {                 Right.Z,                 Up.Z,               -Dir.Z,  0},
      {  -VecDotVec(Loc, Right),  -VecDotVec(Loc, Up),  VecDotVec(Loc, Dir),  1}
    }
  };

  return m;
} /* End of 'MatrView' function * /

__inline MATR MatrFrustum( FLT Left, FLT Right, FLT Bottom, FLT Top, FLT Near, FLT Far )
{
  MATR m =
  {
    {
      {        2 * Near / (Right - Left),                                0,                               0,   0},
      {                                0,        2 * Near / (Top - Bottom),                               0,   0},
      {  (Right + Left) / (Right - Left),  (Top + Bottom) / (Top - Bottom),    -(Far + Near) / (Far - Near),  -1},
      {                                0,                                0,  -2 * Near * Far / (Far - Near),   0}
    }
  };

  return m;
}

#endif

/* END OF VEC.H */