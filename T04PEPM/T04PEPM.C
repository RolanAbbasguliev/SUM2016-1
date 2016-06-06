/* FILE NAME: T04PERM.C
 * PROGRAMMER: MM3
 * DATE: 04.06.2016
 * PURPOSE: WinAPI windowed application sample.
 */
#include <stdio.h>

#define N 3
int P[N], Parity = 0;

void SavePerm( void );
void Go( int Pos );
void Swap( int *A, int *B );

void Swap( int *A, int *B )
{
  int tmp = *A;

  Parity = !Parity;

  *A = *B;
  *B = tmp;
} /* End of Swap func*/

void Go( int Pos )
{
  int i, x, SaveParity;
  double prod = 1;

  if (Pos == N)
  {
    SavePerm();
    return;
  }    
  else
  {   
/*    Go(Pos + 1);
    SaveParity = Parity;
    x = P[0];
    for (i = Pos + 1; i < N; i++)
    {
      if(Pos != i)
      {
        Swap(&P[Pos], &P[i]);
        Go(Pos + 1);
      }
    }
    for (i = Pos + 1; i < N; i++)
      P[i - 1] = P[i];
    P[N - 1] = x;
    Parity = SaveParity;  */
    SaveParity = Parity;
    for (i = Pos; i < N; i++)
    {
      if (Pos != i)
        Swap(&P[Pos], &P[i]);
      Go(Pos + 1);
      if (Pos != i)
        Swap(&P[Pos], &P[i]);
    }
    Parity = SaveParity;
  } 
} /* End of Go func*/

/*void Go( int Pos )
{
  int i;

  if (Pos == N)
  {
    SavePerm();
    return;
  }    
  else
  {  
    for (i = Pos; i < N; i++)
    {   
      Swap(&P[Pos], &P[i]);       
      Go(Pos + 1);
      Swap(&P[Pos], &P[i]);    
    }
  } 
}*/


void SavePerm( void )
{
  FILE *F;
  int i;

  F = fopen("perm.log", "a");
  if (F == NULL)
    return;
  for (i = 0; i < N - 1; i++)
    fprintf(F, "%2i, ", P[i]);
  fprintf(F, "%2i", P[N - 1]);
  if (Parity)
    fprintf(F, " - odd\n");
  else 
    fprintf(F, " - even\n");
  fclose(F);  
} /* End of SavePerm func*/

void main( void )
{
  int i;

  for (i = 0; i < N; i++)
    P[i] = i + 1;

  Go(0);
} /* End of MAIN func*/
/* END OF T04PEPM.C */