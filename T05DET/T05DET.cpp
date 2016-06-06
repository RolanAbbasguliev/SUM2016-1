/* FILE NAME: T05DET.C
 * PROGRAMMER: MM3
 * DATE: 06.06.2016
 * PURPOSE: WinAPI windowed application sample.
 */
#include <stdio.h>

#define N 7
#define MAX 4

int P[N], Parity = 0;
double A[MAX][MAX], sum = 0;

void SavePerm( void );
void Go( int Pos );
void Swap( int *A, int *B );
void LoadMatrix( char *FileName );

void LoadMatrix( char *FileName )
{

} /* End of LoadMatrix func */

void Swap( int *A, int *B )
{
  int tmp = *A;

  Parity = !Parity;

  *A = *B;
  *B = tmp;
} /* End of Swap func */

void Go( int Pos )
{
  int i, x, SaveParity;
  double prod = 0;

  if (Pos == N)
  {
    for (i = Pos; i < MAX; i++)
      prod *= A[i][P[i]];
    if (Parity)
      sum += prod;
    else
      sum -= prod;
    return;
  }    
  else
  {  
    x = P[0];
    Go(Pos + 1);
    SaveParity = Parity;
    for (i = Pos + 1; i < N; i++)
    {
      Swap(&P[Pos], &P[i]);
      Go(Pos + 1);
    }
    for (i = Pos + 1; i < N; i++)
      P[i - 1] = P[i];
    P[N - 1] = x;
    Parity = SaveParity;
  } 
} /* End of Go func*/

double EvalDeterminant( char *FileName )
{
  int i;

  LoadMatrix(FileName);
  sum = 0;

  for (i = 0; i < N; i++)
    P[i] = i;
  Go(0);
  return sum;
}

void main( void )
{
  char fname[] =
  {
    "math1.txt"
  };

  printf("%f", EvalDeterminant(fname));
} /* End of MAIN func */
/* END OF T05DET */