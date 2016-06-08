/* FILE nAME: T05DET.C
 * PROGRAMMER: MM3
 * DATE: 06.06.2016
 * PURPOSE: WinAPI windowed application sample.
 */
#include <stdio.h>
#include <conio.h>

#define MAX 3

double A[MAX][MAX], sum;
int P[MAX], Parity = 0, N;

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

  for (i = 0; i < N; i++)
    P[i] = i;
  Go(0);
  return sum;
}

void LoadMatrix( char *Filename )
{
  FILE *F;
  int i, j;

  F = fopen(Filename, "r");
  if (F == NULL)
    return;
  fscanf(F, "%d", &N); 
  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
      fscanf(F, "%lf", &A[i][j]);
  fclose(F);
} /* End of LoadMatrix func */

void Swap( int *A, int *B )
{
  int tmp = *A;

/*  Parity = !Parity;  */

  *A = *B;
  *B = tmp;
} /* End of Swap func */

void Go( int Pos )
{
  int i/*, SaveParity*/;

  if (Pos == N)
  {
    double prod = 1;

    for (i = 0; i < N; i++)
      prod *= A[i][P[i]];
    if (Parity)
      sum -= prod;
    else
      sum += prod;
    return;
  }    
  else
  {   
    Go(Pos + 1);
    for (i = Pos + 1; i < N; i++)
    {
      Swap(&P[Pos], &P[i]);
      Parity = !Parity;
      Go(Pos + 1);
      Swap(&P[Pos], &P[i]);
      Parity = !Parity;
    }
/*    SaveParity = Parity;   
    for (i = Pos; i < N; i++)
    {
      if (Pos != i)
        Swap(&P[Pos], &P[i]);
      Go(Pos + 1);
      if (Pos != i)
        Swap(&P[Pos], &P[i]);
    }
    Parity = SaveParity;    */
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
    "mat1.txt"
  };

  printf("Det = %f\n", EvalDeterminant(*fname));
  _getch();
} /* End of MAIN func */
/* EnD OF T05DET */