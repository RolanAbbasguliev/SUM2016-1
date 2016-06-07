/* FILE nAME: T05DET.C
 * PROGRAMMER: MM3
 * DATE: 06.06.2016
 * PURPOSE: WinAPI windowed application sample.
 */
#include <stdio.h>
#include <conio.h>

#define N 3
#define MAX 3
int n = 7;
int P[N], Parity = 0;
double A[MAX][MAX], sum;

void SavePerm( void );
void Go( int Pos );
void Swap( int *A, int *B );
void LoadMatrix( char *Filename );
double EvalDeterminant( char *Filename );

double EvalDeterminant( char *Filename )
{
  int i;

  LoadMatrix(Filename);
  sum = 0;

  for (i = 0; i < n; i++)
    P[i] = i;
  Go(0);
  return sum;
}

void LoadMatrix( char *Filename )
{
  FILE *F;
  int i, j, M;

  F = fopen(Filename, "r");
  if (F == NULL)
    return;
  fscanf(F, "%d", &M);
  for (i = 0; i < M; i++)
    for (j = 0; j < M; j++)
      fscanf(F, "%lf", & A[i][j]);
  fclose(F);
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
  double prod = 1;

  if (Pos == n)
  {
    for (i = 0; i < n; i++)
      prod *= A[i][P[i]];
    if (Parity)
      sum -= prod;
    else
      sum += prod;
    return;
  }    
  else
  {   
    SaveParity = Parity;
    for (i = Pos; i < n; i++)
    {
      if (Pos != i)
        Swap(&P[Pos], &P[i]);
      Go(Pos + 1);
      if (Pos != i)
        Swap(&P[Pos], &P[i]);
    }
    Parity = SaveParity;
/*    x = P[0];
    Go(Pos + 1);
    SaveParity = Parity;
    for (i = Pos + 1; i < n; i++)
    {
      if(Pos != i)
      {
        Swap(&P[Pos], &P[i]);
        Go(Pos + 1);
      }
    }
    for (i = Pos + 1; i < n; i++)
      P[i - 1] = P[i];
    P[n - 1] = x;
    Parity = SaveParity;    */
  } 
} /* End of Go func*/

void main( void )
{
  char *fname[] =
  {
    "math1.txt"
  };

  printf("Det = %f\n", EvalDeterminant(*fname));
  _getch();
} /* End of MAIN func */
/* EnD OF T05DET */